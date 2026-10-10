param([string]$PackageRoot = '')
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
if (-not $PackageRoot) { $PackageRoot = Join-Path $repoRoot 'dist\DBCAD-fullstack-package' }
. (Join-Path $repoRoot 'CAD_DB\deploy\lib\pe.ps1')
. (Join-Path $repoRoot 'CAD_DB\deploy\lib\runtime.ps1')
$release = Get-Item (Join-Path $repoRoot 'x64\Release\CAD_DB.exe')
$debug = Get-Item (Join-Path $repoRoot 'x64\Debug\CAD_DB.exe')
if (Test-DebugBuild $release) { throw 'Release incorrectly identified as Debug' }
if (-not (Test-DebugBuild $debug)) { throw 'Debug incorrectly identified as Release' }
Assert-ClientRuntime (Join-Path $PackageRoot 'client\CAD_DB.exe')
$fixture = Join-Path ([System.IO.Path]::GetTempPath()) ('dbcad-runtime-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path (Join-Path $fixture 'platforms') -Force | Out-Null
try {
    Copy-Item -LiteralPath $release.FullName -Destination (Join-Path $fixture 'CAD_DB.exe')
    Set-Content -LiteralPath (Join-Path $fixture 'Qt6Cored.dll') -Value 'stale-debug-fixture'
    if (Test-DebugBuild (Get-Item (Join-Path $fixture 'CAD_DB.exe'))) { throw 'Stale DLL changed exe configuration' }
    Remove-Item -LiteralPath (Join-Path $fixture 'Qt6Cored.dll')
    $tool = Get-BuildWinDeployQtPath -Exe $release -RepoRoot $repoRoot
    if (-not $tool) { throw 'Build kit not resolved from Qt build metadata' }
    $kit = Split-Path -Parent (Split-Path -Parent $tool)
    Copy-Item -LiteralPath (Join-Path $kit 'plugins\platforms\qwindows.dll') -Destination (Join-Path $fixture 'platforms')
    $caught = $false
    try { Assert-ClientRuntime (Join-Path $fixture 'CAD_DB.exe') } catch {
        if ($_.Exception.Message -notmatch 'Qt6Core.dll') { throw }
        $caught = $true
    }
    if (-not $caught) { throw 'Incomplete package unexpectedly passed' }
    Write-Host '[OK] PE modes, stale DLL detection, build kit resolution and missing imports regression passed.'
} finally {
    $resolved = (Resolve-Path -LiteralPath $fixture).Path
    $tempRoot = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath()).TrimEnd('\') + '\'
    if ($resolved.StartsWith($tempRoot, [System.StringComparison]::OrdinalIgnoreCase) -and (Split-Path -Leaf $resolved) -like 'dbcad-runtime-*') {
        Remove-Item -LiteralPath $resolved -Recurse -Force
    } else { throw "Invalid test fixture: $resolved" }
}
