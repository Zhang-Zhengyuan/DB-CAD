@echo off
rem L1.5 SAT 语料库回归机构建脚本（sat_corpus_regression.cpp）。
rem 前置条件同 build_graph_tests.bat。
setlocal
set REPO=%~dp0..\..\..
set QTROOT=%~1
if "%QTROOT%"=="" set QTROOT=D:\Qt\6.9.0\msvc2022_64
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cl /nologo /std:c++20 /Zc:__cplusplus /EHa /MD /utf-8 /DSPA_NO_AUTO_LINK /I"%REPO%\_deps" /I"%REPO%\_deps\acis\include" /I"%REPO%\CAD_DB" /I"%QTROOT%\include" /I"%QTROOT%\include\QtCore" /I. sat_corpus_regression.cpp "%REPO%\CAD_DB\entity_graph_serializer.cpp" "%REPO%\CAD_DB\entity_graph_deserializer.cpp" /Fe:sat_corpus_regression.exe /link /STACK:4194304 /LIBPATH:"%REPO%\_deps\acis\lib" SPAAcisDs.lib /LIBPATH:"%QTROOT%\lib" Qt6Core.lib
if %ERRORLEVEL%==0 (
  copy /y "%REPO%\_deps\acis\bin\SPAAcisDs.dll" . >nul
  copy /y "%QTROOT%\bin\Qt6Core.dll" . >nul
  echo Build OK: sat_corpus_regression.exe ^(run: sat_corpus_regression .^)
) else (
  echo Build FAILED
  exit /b 1
)
