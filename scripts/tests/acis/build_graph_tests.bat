@echo off
rem 实体图 JSON 往返测试构建脚本（Mode1 存储链路）。
rem 前置条件：
rem   1. _deps/acis 已按 docs/DEPENDENCIES.md 布局就位（官方 2026.1.0.1）
rem   2. 本目录下有 spatial_license.h（不入库）
rem   3. Qt 6 已安装（默认 D:\Qt\6.9.0\msvc2022_64，可用第 1 参覆盖）
rem 产物 graph_roundtrip_tests.exe 需与 SPAAcisDs.dll、Qt6Core.dll 同目录运行。
setlocal
set REPO=%~dp0..\..\..
set QTROOT=%~1
if "%QTROOT%"=="" set QTROOT=D:\Qt\6.9.0\msvc2022_64
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cl /nologo /std:c++20 /Zc:__cplusplus /EHa /MD /utf-8 /DSPA_NO_AUTO_LINK /I"%REPO%\_deps" /I"%REPO%\_deps\acis\include" /I"%REPO%\CAD_DB" /I"%QTROOT%\include" /I"%QTROOT%\include\QtCore" /I. graph_roundtrip_tests.cpp "%REPO%\CAD_DB\entity_graph_serializer.cpp" "%REPO%\CAD_DB\entity_graph_deserializer.cpp" /Fe:graph_roundtrip_tests.exe /link /STACK:4194304 /LIBPATH:"%REPO%\_deps\acis\lib" SPAAcisDs.lib /LIBPATH:"%QTROOT%\lib" Qt6Core.lib
if %ERRORLEVEL%==0 (
  copy /y "%REPO%\_deps\acis\bin\SPAAcisDs.dll" . >nul
  copy /y "%QTROOT%\bin\Qt6Core.dll" . >nul
  echo Build OK: graph_roundtrip_tests.exe ^(run: graph_roundtrip_tests 12^)
) else (
  echo Build FAILED
  exit /b 1
)
