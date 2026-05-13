@echo off
setlocal
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0start_dbcad_fullstack.ps1" %*
endlocal
