from pathlib import Path

from app.config import Settings, default_worker_exe


def test_linux_worker_default():
    assert default_worker_exe("linux").parts[-4:] == ("build", "linux", "Release", "dbcad_worker")


def test_windows_worker_default_is_unchanged():
    assert default_worker_exe("win32").parts[-3:] == ("x64", "Debug", "dbcad_worker.exe")


def test_worker_env_override(monkeypatch):
    monkeypatch.setenv("CAD_DB_WORKER_EXE", "/opt/teacher/worker/dbcad_worker")
    assert Path(Settings(_env_file=None).worker_exe) == Path("/opt/teacher/worker/dbcad_worker")
