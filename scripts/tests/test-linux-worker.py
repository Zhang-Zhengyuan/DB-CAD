"""Packaging/platform regression tests. These fixtures do not implement ACIS."""
import argparse
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tarfile
import tempfile
import unittest
from unittest.mock import patch

SPEC = importlib.util.spec_from_file_location("linux_worker", Path(__file__).parents[1] / "linux-worker.py")
worker = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(worker)


class RuntimeTests(unittest.TestCase):
    def test_non_linux_host_fails_before_build(self):
        with patch.object(worker.sys, "platform", "darwin"):
            with self.assertRaisesRegex(ValueError, "Linux"):
                worker.build(argparse.Namespace())

    def test_sdk_links_are_materialized_and_conflicts_fail(self):
        with tempfile.TemporaryDirectory() as scratch:
            root = Path(scratch)
            source = root / "sdk"
            source.mkdir()
            (source / "lib.so.1").write_bytes(b"first")
            (source / "lib.so").symlink_to(source / "lib.so.1")
            out = root / "out"
            worker.copy_runtime(source, out)
            self.assertFalse((out / "lib.so").is_symlink())
            self.assertEqual((out / "lib.so").read_bytes(), b"first")
            (source / "lib.so.1").write_bytes(b"different")
            with self.assertRaisesRegex(ValueError, "Conflicting"):
                worker.copy_runtime(source, out)

    def test_wrong_format_and_architecture_are_rejected(self):
        with tempfile.TemporaryDirectory() as scratch:
            path = Path(scratch) / "library"
            for header in (b"MZ" + bytes(20), b"\xcf\xfa\xed\xfe" + bytes(20),
                           b"\x7fELF\x02\x01" + bytes(12) + (183).to_bytes(2, "little")):
                path.write_bytes(header)
                with self.assertRaisesRegex(ValueError, "ELF"):
                    worker.elf(path, "x86_64")

    def test_dangling_sdk_link_is_rejected(self):
        with tempfile.TemporaryDirectory() as scratch:
            root = Path(scratch)
            (root / "sdk").mkdir()
            (root / "sdk/missing.so").symlink_to(root / "missing")
            with self.assertRaisesRegex(ValueError, "Dangling"):
                worker.copy_runtime(root / "sdk", root / "out")

    @unittest.skipUnless(sys.platform == "linux" and shutil.which("c++"), "Linux C++ compiler required")
    def test_posix_file_io_unicode_and_failure(self):
        cad = worker.ROOT / "systems/dbcad/CAD_DB"
        if not cad.exists():
            cad = worker.ROOT / "CAD_DB"
        with tempfile.TemporaryDirectory() as scratch:
            root = Path(scratch)
            source = root / "file_io.cpp"
            source.write_text('''#include "worker/file_io.h"
int main(int argc, char** argv) {
    const std::filesystem::path path(argv[1]);
    FILE* out = dbcad::worker::openFile(path, true);
    if (!out) return 1;
    std::fputs("payload", out); std::fclose(out);
    FILE* in = dbcad::worker::openFile(path, false);
    if (!in) return 2;
    const int first = std::fgetc(in); std::fclose(in);
    if (first != 'p') return 3;
    return dbcad::worker::openFile(path / "missing", false) ? 4 : 0;
}
''')
            subprocess.run(["c++", "-std=c++20", "-I" + str(cad), str(source), "-o", str(root / "file_io")], check=True)
            subprocess.run([str(root / "file_io"), str(root / "中文模型.sat")], check=True)

    @unittest.skipUnless(sys.platform == "linux" and shutil.which("cc"), "Linux ELF compiler required")
    def test_runtime_relocation_missing_library_and_package_gate(self):
        # A real ELF linked to a tiny private .so exercises loader behavior.
        # The package gate is replaced only here: this is not a geometry test.
        with tempfile.TemporaryDirectory() as scratch:
            root = Path(scratch)
            source = root / "source"
            source.mkdir()
            (source / "library.c").write_text("int answer(void) { return 42; }\n")
            (source / "main.c").write_text("extern int answer(void); int main(void) { return answer() == 42 ? 0 : 1; }\n")
            subprocess.run(["cc", "-shared", "-fPIC", str(source / "library.c"), "-o", str(source / "libfixture.so")], check=True)
            build = root / "build"
            binaries = build / "runtime/code/bin"
            binaries.mkdir(parents=True)
            (binaries / "libfixture.so").symlink_to(source / "libfixture.so")
            subprocess.run(["cc", str(source / "main.c"), "-L" + str(source), "-lfixture",
                            "-Wl,-rpath,$ORIGIN", "-o", str(binaries / "dbcad_worker.bin")], check=True)
            shutil.copy2(binaries / "dbcad_worker.bin", binaries / "mesh_tests.bin")
            for name in ("dbcad_worker", "mesh_tests"):
                (build / name).write_text(worker.wrapper(name))
                (build / name).chmod(0o755)
            (build / "build.json").write_text("{}")
            with self.assertRaisesRegex(ValueError, "External non-system"):
                worker.dependencies(build, worker.host())
            (binaries / "libfixture.so").unlink()
            with self.assertRaisesRegex(ValueError, "Unresolved"):
                worker.dependencies(build, worker.host())
            shutil.copy2(source / "libfixture.so", binaries / "libfixture.so")
            worker.dependencies(build, worker.host())
            archive = root / "runtime.tar.gz"
            args = argparse.Namespace(directory=build, output=archive)
            with patch.object(worker, "check", side_effect=ValueError("gate failed")):
                with self.assertRaisesRegex(ValueError, "gate failed"):
                    worker.package(args)
            self.assertFalse(archive.exists())
            def relocated_check(path):
                worker.dependencies(path, worker.host())
                subprocess.run([str(path / "dbcad_worker")], check=True)
            with patch.object(worker, "check", side_effect=relocated_check):
                worker.package(args)
            unpack = root / "relocated"
            with tarfile.open(archive) as package:
                self.assertFalse(any(m.issym() for m in package.getmembers()))
                # Python 3.11 before the tar filter backport still supports this
                # locally generated, symlink-free fixture archive.
                options = {"filter": "data"} if hasattr(tarfile, "data_filter") else {}
                package.extractall(unpack, **options)
            shutil.rmtree(source)
            shutil.rmtree(build)
            relocated_check(unpack / "dbcad-worker-linux")
            manifest = json.loads((unpack / "dbcad-worker-linux/SHA256.json").read_text())
            self.assertIn("runtime/code/bin/libfixture.so", manifest)


if __name__ == "__main__":
    unittest.main()
