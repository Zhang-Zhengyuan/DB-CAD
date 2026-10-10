param([string]$QtRoot = $env:DBCAD_QT_ROOT, [string]$QtMsBuildPath = $env:QtMsBuild)
& (Join-Path $PSScriptRoot "..\scripts\build.ps1") -Configuration Debug -QtRoot $QtRoot -QtMsBuildPath $QtMsBuildPath
