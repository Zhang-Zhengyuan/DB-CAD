@echo off
setlocal
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0start_dbcad_with_pg.ps1" %*
endlocal
