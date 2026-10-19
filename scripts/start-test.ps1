param([ValidateSet('Release', 'Debug')][string]$Configuration = 'Release')
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot
if (-not (Test-Path -LiteralPath (Join-Path $repoRoot 'build.local.props'))) {
    throw 'Configure build.local.props first; see docs/LOCAL_SETUP.md.'
}
& (Join-Path $repoRoot 'scripts/build.ps1') -Configuration $Configuration
& (Join-Path $repoRoot 'CAD_DB/deploy/start_dbcad_fullstack.ps1') -KeepExistingServices
