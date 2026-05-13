@echo off
setlocal
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0reset_dbcad_deployment.ps1" %*
endlocal
