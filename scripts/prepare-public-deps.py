"""Install the audited Windows x64 public dependency kit; no ACIS download."""
from pathlib import Path
import hashlib
import shutil
import urllib.request
import zipfile

ROOT = Path(__file__).resolve().parents[1]
WORK = ROOT / 'artifacts' / 'dependency-audit'
DEPS = ROOT / '_deps'
ARCHIVES = {
    'postgresql-18.6.zip': (
        'https://sbp.enterprisedb.com/getfile.jsp?fileid=1260609',
        'e2246ba91d22345bc3d017586c09ede52d9df180b1eeb480f050445f1cad84e2',
    ),
    'glfw.zip': (
        'https://github.com/glfw/glfw/releases/download/3.5.1/glfw-3.5.1.bin.WIN64.zip',
        '34c6766043e09ad9c6e88b5636a5f34d1f2857aae828768c5bf2016dc8821a93',
    ),
}


def install():
    WORK.mkdir(parents=True, exist_ok=True)
    # Verify every input before modifying existing dependencies.
    for filename, (url, digest) in ARCHIVES.items():
        archive = WORK / filename
        if not archive.exists():
            request = urllib.request.Request(url, headers={'User-Agent': 'DBCAD-dependency-installer'})
            with urllib.request.urlopen(request, timeout=120) as response, archive.open('wb') as output:
                shutil.copyfileobj(response, output)
        with archive.open('rb') as stream:
            actual_digest = hashlib.file_digest(stream, 'sha256').hexdigest()
        if actual_digest != digest:
            raise RuntimeError(f'Checksum mismatch: {archive}. Preserve it and obtain a verified archive.')
        with zipfile.ZipFile(archive) as package:
            if package.testzip():
                raise RuntimeError(f'Corrupt archive: {archive}')
    for name in ('openssl', 'glfw', 'postgresql'):
        original = DEPS / name
        backup = WORK / 'original-deps' / name
        if original.exists() and not backup.exists():
            shutil.copytree(original, backup)
    with zipfile.ZipFile(WORK / 'postgresql-18.6.zip') as package:
        for name in package.namelist():
            if name.endswith('/'):
                continue
            if name.startswith('pgsql/include/openssl/'):
                target = DEPS / 'openssl' / Path(name).relative_to('pgsql/include/openssl')
            elif name.startswith('pgsql/include/'):
                target = DEPS / 'postgresql/include' / Path(name).relative_to('pgsql/include')
            elif name in ('pgsql/lib/libpq.lib', 'pgsql/lib/libssl.lib', 'pgsql/lib/libcrypto.lib'):
                folder = 'postgresql/lib' if name.endswith('/libpq.lib') else 'openssl/lib'
                target = DEPS / folder / Path(name).name
            elif name in ('pgsql/server_license.txt', 'pgsql/commandlinetools_3rd_party_licenses.txt'):
                target = DEPS / 'postgresql/licenses' / Path(name).name
            elif name.startswith('pgsql/bin/'):
                leaf = Path(name).name
                if leaf in ('libssl-3-x64.dll', 'libcrypto-3-x64.dll'):
                    target = DEPS / 'openssl/bin' / leaf
                elif leaf in ('libpq.dll', 'libintl-9.dll', 'libiconv-2.dll', 'libwinpthread-1.dll'):
                    target = DEPS / 'postgresql/bin' / leaf
                else:
                    continue
            else:
                continue
            if not target.resolve().is_relative_to(DEPS.resolve()):
                raise RuntimeError(f'Invalid archive path: {name}')
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(package.read(name))
    with zipfile.ZipFile(WORK / 'glfw.zip') as package:
        prefix = 'glfw-3.5.1.bin.WIN64/'
        files = {
            'include/GLFW/glfw3.h': 'include/glfw3.h',
            'include/GLFW/glfw3native.h': 'include/glfw3native.h',
            'lib-vc2022/glfw3.lib': 'lib/glfw3_64.lib',
            'LICENSE.md': 'LICENSE.md',
        }
        for source, relative in files.items():
            target = DEPS / 'glfw' / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(package.read(prefix + source))
    print('Installed PostgreSQL 18.6 + OpenSSL 3.5.9 LTS + GLFW 3.5.1 (x64 /MD).')


if __name__ == '__main__':
    install()
