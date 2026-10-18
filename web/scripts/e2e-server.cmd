@echo off
rem P5 e2e backend launcher: runs from TEMP so backend/.env is NOT loaded
set CAD_DB_COMPONENT_ROOT=%~dp0..\..\CAD_DB\backend\data\components-e2e
pushd %TEMP%
"%~dp0..\..\CAD_DB\backend\.venv\Scripts\python.exe" -m uvicorn --app-dir "%~dp0..\..\CAD_DB\backend" app.main:app --host 127.0.0.1 --port 8765 --log-level warning
popd
