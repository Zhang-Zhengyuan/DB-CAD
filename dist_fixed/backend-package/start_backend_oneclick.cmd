@echo off
setlocal
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0start_backend_oneclick.ps1" %*
endlocal
