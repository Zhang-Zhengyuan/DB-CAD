param([string]$DepsRoot = $env:DBCAD_DEPS_ROOT, [string]$QtRoot = $env:DBCAD_QT_ROOT)
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot
if (-not $DepsRoot) { $DepsRoot = Join-Path $repoRoot '_deps' }
$missing = @()
foreach ($relative in @('acis\include\api.hxx', 'acis\lib\SPAAcisDs.lib', 'acis\lib\SPAAcisDsd.lib', 'acis\bin\SPAAcisDs.dll', 'acis\bin\SPAAcisDsd.dll', 'glfw\lib\glfw3_64.lib')) {
    if (-not (Test-Path -LiteralPath (Join-Path $DepsRoot $relative))) { $missing += "Dependency: $relative" }
}
if (-not (Test-Path -LiteralPath (Join-Path $repoRoot 'CAD_DB\spatial_license.h'))) { $missing += 'CAD_DB/spatial_license.h: matching 2026 license header required; see docs/LOCAL_SETUP.md' }
foreach ($tool in @('uv', 'docker')) {
    if (-not (Get-Command $tool -ErrorAction SilentlyContinue)) { $missing += "PATH: $tool" }
}
if ($QtRoot -and -not (Test-Path -LiteralPath (Join-Path $QtRoot 'bin\windeployqt.exe'))) { $missing += 'Qt MSVC x64 kit: windeployqt.exe' }
if ($missing.Count) { $missing | ForEach-Object { Write-Host "[MISSING] $_" }; throw 'Environment incomplete; see docs/GETTING_STARTED.md.' }
Write-Host '[OK] Required dependency files and command-line tools found.'
Write-Host 'Next: scripts/build.ps1; deployment additionally requires a running Docker engine.'
