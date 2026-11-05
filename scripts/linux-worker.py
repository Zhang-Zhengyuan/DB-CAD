#!/usr/bin/env python3
"""Build and package the native Linux worker using an authorized local SDK.

No downloads, cross compilation, SDK modification or service lifecycle actions.
The same entry point supports the DB-CAD and AI-DBCAD source layouts.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import shutil
import subprocess
import sys
import tarfile
import tempfile

ROOT = Path(__file__).resolve().parents[1]
MACHINES = {"x86_64": 62, "aarch64": 183}
SYSTEM_LIBS = ("linux-vdso", "ld-linux", "libc.so", "libm.so", "libdl.so",
               "libpthread.so", "librt.so", "libstdc++.so", "libgcc_s.so", "libresolv.so")


def run(*args, **kwargs):
    return subprocess.run([str(a) for a in args], check=True, **kwargs)


def host():
    machine = platform.machine()
    if sys.platform != "linux" or machine not in MACHINES:
        raise ValueError("Run on Linux x86_64 or aarch64 with a matching Linux SDK; macOS/Windows SDKs cannot be used.")
    return machine


def elf(path, machine):
    with Path(path).open("rb") as stream:
        header = stream.read(20)
    if len(header) < 20 or header[:6] != b"\x7fELF\x02\x01" or int.from_bytes(header[18:20], "little") != MACHINES[machine]:
        raise ValueError(f"Not a matching Linux 64-bit ELF ({machine}): {path}")


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def copy_runtime(source, destination, ancestors=()):
    """Materialize SDK symlinks; fail on conflicting ACIS/InterOp filenames."""
    resolved = source.resolve()
    if resolved in ancestors:
        raise ValueError(f"Cyclic SDK directory link: {source}")
    ancestors = (*ancestors, resolved)
    destination.mkdir(parents=True, exist_ok=True)
    for item in sorted(source.iterdir()):
        if not item.exists():
            raise ValueError(f"Dangling SDK symlink: {item}")
        target = destination / item.name
        if item.is_dir():
            copy_runtime(item, target, ancestors)
        elif target.exists():
            if not target.is_file() or digest(item) != digest(target):
                raise ValueError(f"Conflicting runtime file: {item.name}; use matching SDK releases")
        else:
            shutil.copy2(item, target, follow_symlinks=True)


def wrapper(target):
    return f'''#!/bin/sh
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
BIN="$HERE/runtime/code/bin"
export LD_LIBRARY_PATH="$BIN${{LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}}"
export PATH="$BIN:$PATH"
# ACIS requires at least a 4 MiB stack; fail if the host hard limit forbids it.
STACK=$(ulimit -s)
if [ "$STACK" != unlimited ] && [ "$STACK" -lt 4096 ]; then ulimit -s 4096; fi
exec "$BIN/{target}.bin" "$@"
'''


def library(directory, names):
    for name in names:
        candidate = directory / name
        if candidate.is_file():
            return candidate
    raise ValueError(f"Missing Linux SDK library in {directory}: expected one of {', '.join(names)}")


def build(args):
    machine = host()
    cad = ROOT / "systems/dbcad/CAD_DB"
    if not cad.is_dir():
        cad = ROOT / "CAD_DB"
    acis = args.acis_root.resolve()
    acis_bin = args.acis_bin.resolve()
    interop = args.interop_root.resolve()
    interop_bin = args.interop_bin.resolve()
    license_header = args.license_header.resolve() if args.license_header else cad / "spatial_license.h"
    json_header = args.json_include.resolve() / "vendor/nlohmann/json.hpp"
    for required in (acis / "include/kernapi.hxx", interop / "include/SPAIDocument.h", license_header, json_header):
        if not required.is_file():
            raise ValueError(f"Missing input: {required}")
    libs = [library(acis_bin, ["libSpaACIS.so", "libSPAAcisDs.so"])]
    libs += [library(interop_bin, [f"lib{name}.so"]) for name in ("SPAIInterop", "SPAIAcis", "SPAIopUtilities")]
    for lib in libs:
        elf(lib, machine)
    local_license = cad / "spatial_license.h"
    if local_license.exists() and digest(local_license) != digest(license_header):
        raise ValueError("--license-header differs from the source-local license header; select one consistent authorized license")
    # Explicit, fresh output prevents stale binaries from being mistaken for a new build.
    output = args.output.resolve() if args.output else ROOT / "build/linux" / args.configuration
    if output.exists():
        raise ValueError(f"Output already exists; choose a new --output directory: {output}")
    for source in (acis_bin, interop_bin):
        if output.is_relative_to(source):
            raise ValueError("Output must be outside SDK input directories")
    output.mkdir(parents=True)
    include = output / "include"
    (include / "acis").mkdir(parents=True)
    (include / "acis/include").symlink_to(acis / "include", target_is_directory=True)
    # This private header stays in the ignored build directory and is never packaged.
    shutil.copy2(license_header, include / "spatial_license.h")
    bin_dir = output / "runtime/code/bin"
    for source in (interop_bin, acis_bin):
        copy_runtime(source, bin_dir)
    for name in ("resources", "reffiles"):
        source = interop_bin.parent.parent / name
        if source.is_dir():
            copy_runtime(source, output / "runtime" / name)
    flags = ["-std=c++20", "-fPIC", "-pthread", "-DSPA_NO_AUTO_LINK",
             "-O2" if args.configuration == "Release" else "-O0"]
    if args.configuration == "Debug":
        flags += ["-g"]
    flags += ["-I" + str(p) for p in (include, cad, args.json_include.resolve(), interop / "include")]
    objects = []
    sources = ["acis.cpp", "common.cpp", "kernel/mesh_extractor.cpp", "kernel/glb_writer.cpp",
               "worker/main.cpp", "worker/session.cpp", "worker/entity_graph_codec.cpp"]
    for source in sources:
        obj = output / (source.replace("/", "_") + ".o")
        run(args.cxx, *flags, "-c", cad / source, "-o", obj)
        objects.append(obj)
    # Link against the staged SDK so its transitive dependencies are also resolved.
    link = ["-pthread", "-L" + str(bin_dir), "-Wl,-rpath,$ORIGIN", "-Wl,-rpath-link," + str(bin_dir)]
    staged_libs = [bin_dir / lib.name for lib in libs]
    run(args.cxx, *objects, *link, *staged_libs, "-ldl", "-o", bin_dir / "dbcad_worker.bin")
    run(args.cxx, *flags, ROOT / "scripts/tests/mesh_tests.cpp", *objects[:4], *link, *staged_libs,
        "-ldl", "-o", bin_dir / "mesh_tests.bin")
    for target in ("dbcad_worker", "mesh_tests"):
        path = output / target
        path.write_text(wrapper(target))
        path.chmod(0o755)
        elf(bin_dir / (target + ".bin"), machine)
    metadata = {"schema": "dbcad.linux-build/1", "configuration": args.configuration,
                "architecture": machine, "source_commit": run("git", "-C", ROOT, "rev-parse", "HEAD", capture_output=True, text=True).stdout.strip(),
                "source_dirty": bool(run("git", "-C", ROOT, "status", "--porcelain", capture_output=True, text=True).stdout),
                "compiler": run(args.cxx, "--version", capture_output=True, text=True).stdout.splitlines()[0],
                "sdk_libraries": {lib.name: digest(lib) for lib in libs},
                "libc": list(platform.libc_ver()),
                "source_files": {str(p.relative_to(ROOT)): digest(p)
                                 for folder in (cad, cad / "worker", cad / "kernel")
                                 for p in sorted(folder.iterdir())
                                 if p.is_file() and p.suffix in (".cpp", ".h", ".hxx", ".inc")
                                 and p.name != "spatial_license.h"}}
    (output / "build.json").write_text(json.dumps(metadata, indent=2) + "\n")
    print(f"Built {output}; run check before distribution.")


def dependencies(directory, machine):
    bin_dir = directory / "runtime/code/bin"
    env = dict(os.environ, LD_LIBRARY_PATH=str(bin_dir))
    env.pop("LD_PRELOAD", None)
    for item in bin_dir.rglob("*"):
        if not item.is_file():
            continue
        with item.open("rb") as stream:
            is_elf = stream.read(4) == b"\x7fELF"
        if not is_elf:
            continue
        elf(item, machine)
        result = subprocess.run(["ldd", str(item)], env=env, capture_output=True, text=True)
        listing = result.stdout + result.stderr
        if result.returncode and "statically linked" not in listing and "not a dynamic executable" not in listing:
            raise ValueError(f"Cannot inspect dependencies of {item.name}: {listing}")
        if "not found" in listing:
            raise ValueError(f"Unresolved runtime dependencies for {item.name}: {listing}")
        for line in listing.splitlines():
            if "=>" not in line:
                continue
            name, location = line.split("=>", 1)
            if name.strip().startswith(SYSTEM_LIBS):
                continue
            resolved = Path(location.strip().split(" (", 1)[0])
            if not resolved.resolve().is_relative_to(directory.resolve()):
                raise ValueError(f"External non-system dependency: {line.strip()}; stage its Linux runtime in the SDK bin directory")


def check(directory):
    machine = host()
    directory = directory.resolve()
    elf(directory / "runtime/code/bin/dbcad_worker.bin", machine)
    dependencies(directory, machine)
    run(directory / "dbcad_worker", "selftest", timeout=180)
    mesh = directory / "mesh_tests"
    if mesh.exists():
        run(mesh, timeout=180)
    # Exercise persistent stdin/stdout transport, including negative command handling.
    with tempfile.TemporaryDirectory(prefix="dbcad-linux-check-") as scratch:
        messages = [{"id": 1, "op": "ping"}, {"id": 2, "op": "open", "spool": scratch, "components": []},
                    {"id": 3, "op": "exec", "command": {"action": "invalid.linux-check", "params": {}}},
                    {"id": 4, "op": "close"}]
        result = run(directory / "dbcad_worker", "serve", input="".join(json.dumps(m) + "\n" for m in messages),
                     capture_output=True, text=True, timeout=60)
        replies = [json.loads(line) for line in result.stdout.splitlines() if line.strip()]
        if [(r.get("id"), r.get("ok")) for r in replies] != [(1, True), (2, True), (3, False), (4, True)]:
            raise ValueError(f"Unexpected worker serve protocol: {replies}")
    print("Linux dependencies, selftest, mesh (if present) and serve checks passed. STEP/full-stack checks remain separate.")


def package(args):
    host()
    source = args.directory.resolve()
    output = args.output.resolve()
    if output.exists():
        raise ValueError(f"Package already exists: {output}")
    if output.is_relative_to(source):
        raise ValueError("Archive must be outside the build directory")
    # Test the relocated copy without build headers, objects or original SDK paths.
    with tempfile.TemporaryDirectory(prefix="dbcad-package-") as scratch:
        root = Path(scratch) / "dbcad-worker-linux"
        root.mkdir()
        shutil.copytree(source / "runtime", root / "runtime", symlinks=False)
        for name in ("dbcad_worker", "mesh_tests", "build.json"):
            shutil.copy2(source / name, root / name)
        check(root)
        (root / "validation.json").write_text(json.dumps({
            "architecture": platform.machine(), "libc": list(platform.libc_ver()),
            "passed": ["dependencies", "selftest", "mesh_tests", "serve"],
            "not_checked": ["STEP round trip", "FastAPI/Neo4j/browser integration"]}, indent=2) + "\n")
        (root / "README.txt").write_text("Linux ACIS worker runtime. Start: ./dbcad_worker serve\n"
            "Gateway: CAD_DB_WORKER_EXE=<absolute path>/dbcad_worker\n"
            "Requires the recorded architecture and compatible Linux glibc/libstdc++.\n"
            "Only distribute under your Spatial runtime/license authorization.\n"
            "Validated: dependencies, selftest, mesh_tests, serve. STEP/full stack not implied.\n")
        files = {str(p.relative_to(root)): digest(p) for p in sorted(root.rglob("*")) if p.is_file()}
        (root / "SHA256.json").write_text(json.dumps(files, indent=2) + "\n")
        output.parent.mkdir(parents=True, exist_ok=True)
        with tarfile.open(output, "w:gz") as archive:
            archive.add(root, arcname=root.name)
    print(f"Validated runtime archive: {output}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    b = sub.add_parser("build")
    b.add_argument("--configuration", choices=("Release", "Debug"), default="Release")
    for name in ("acis-root", "acis-bin", "interop-root", "interop-bin"):
        b.add_argument("--" + name, type=Path, required=True)
    b.add_argument("--license-header", type=Path)
    b.add_argument("--json-include", type=Path, default=ROOT / "_deps")
    b.add_argument("--cxx", default=os.environ.get("CXX", "g++"))
    b.add_argument("--output", type=Path)
    c = sub.add_parser("check")
    c.add_argument("directory", type=Path)
    p = sub.add_parser("package")
    p.add_argument("directory", type=Path)
    p.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    try:
        if args.command == "build":
            build(args)
        elif args.command == "check":
            check(args.directory)
        else:
            package(args)
    except (ValueError, OSError, subprocess.SubprocessError) as exc:
        parser.exit(1, f"linux-worker: {exc}\n")


if __name__ == "__main__":
    main()
