@echo off
setlocal
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0start_fullstack_oneclick.ps1" %*
endlocal
