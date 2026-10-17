param([ValidateSet('Release', 'Debug')][string]$Configuration = 'Release')
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot
$localProps = Join-Path $repoRoot 'build.local.props'
if (-not (Test-Path -LiteralPath $localProps)) { throw 'Configure build.local.props first; see docs/LOCAL_SETUP.md.' }
[xml]$settings = Get-Content -LiteralPath $localProps -Raw
$qtRoot = [string]($settings.Project.PropertyGroup | Where-Object QtInstall | Select-Object -First 1).QtInstall
& (Join-Path $repoRoot 'CAD_DB\deploy\start_dbcad_fullstack.ps1') -Configuration $Configuration -WinDeployQtPath (Join-Path $qtRoot 'bin\windeployqt.exe') -KeepExistingServices
