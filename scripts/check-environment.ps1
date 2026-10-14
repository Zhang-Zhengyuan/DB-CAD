param([string]$DepsRoot = $env:DBCAD_DEPS_ROOT, [string]$QtRoot = $env:DBCAD_QT_ROOT, [string]$PostgresInclude = $env:DBCAD_POSTGRES_INCLUDE)
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot
if (-not $DepsRoot) { $DepsRoot = Join-Path $repoRoot '_deps' }
if (-not $PostgresInclude) { $PostgresInclude = Join-Path $DepsRoot 'postgresql\include' }
$missing = @()
foreach ($relative in @('acis\include\api.hxx', 'acis\lib\SPAAcisDs.lib', 'acis\lib\SPAAcisDsd.lib', 'acis\bin\SPAAcisDs.dll', 'acis\bin\SPAAcisDsd.dll', 'glaze-5.5.2\include\glaze\glaze.hpp', 'glfw\lib\glfw3_64.lib', 'mgclient-1.4.2\mgclient.h', 'openssl\lib\libssl64MD.lib', 'openssl\lib\libcrypto64MD.lib', 'libpqxx-7.10.1\lib\libpq.lib')) {
    if (-not (Test-Path -LiteralPath (Join-Path $DepsRoot $relative))) { $missing += "Dependency: $relative" }
}
if (-not (Test-Path -LiteralPath (Join-Path $PostgresInclude 'libpq-fe.h'))) { $missing += 'libpq-fe.h: set DBCAD_POSTGRES_INCLUDE or supply _deps/postgresql/include' }
foreach ($tool in @('uv', 'docker')) {
    if (-not (Get-Command $tool -ErrorAction SilentlyContinue)) { $missing += "PATH: $tool" }
}
if ($QtRoot -and -not (Test-Path -LiteralPath (Join-Path $QtRoot 'bin\windeployqt.exe'))) { $missing += 'Qt MSVC x64 kit: windeployqt.exe' }
if ($missing.Count) { $missing | ForEach-Object { Write-Host "[MISSING] $_" }; throw 'Environment incomplete; see docs/GETTING_STARTED.md.' }
Write-Host '[OK] Required dependency files and command-line tools found.'
Write-Host 'Next: scripts/build.ps1; deployment additionally requires a running Docker engine.'
