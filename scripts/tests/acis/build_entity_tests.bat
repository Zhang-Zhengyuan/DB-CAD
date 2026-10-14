@echo off
rem ACIS 2026 实体测试套件构建脚本。
rem 前置条件：
rem   1. _deps/acis 已按 docs/DEPENDENCIES.md 布局就位（官方 2026.1.0.1）
rem   2. 本目录下有 spatial_license.h（配套 2026 的许可密钥，向维护者索取，不入库）
rem 编译产物 entity_tests.exe 需与 SPAAcisDs.dll 同目录运行（脚本会自动拷贝）。
setlocal
set REPO=%~dp0..\..\..
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cl /nologo /std:c++20 /EHa /MD /utf-8 /DSPA_NO_AUTO_LINK /I"%REPO%\_deps\acis\include" /I. entity_tests.cpp /Fe:entity_tests.exe /link /STACK:4194304 /LIBPATH:"%REPO%\_deps\acis\lib" SPAAcisDs.lib
if %ERRORLEVEL%==0 (
  copy /y "%REPO%\_deps\acis\bin\SPAAcisDs.dll" . >nul
  echo Build OK: entity_tests.exe ^(run: entity_tests 9^)
) else (
  echo Build FAILED
  exit /b 1
)
