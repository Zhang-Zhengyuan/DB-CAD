@echo off
setlocal
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0stop_dbcad_fullstack.ps1" %*
endlocal
