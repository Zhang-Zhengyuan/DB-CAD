# Regression: copied/packaged executables must not shadow current source builds.
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
. (Join-Path $repoRoot 'CAD_DB\deploy\lib\runtime.ps1')
$fixtureRoot = Join-Path ([System.IO.Path]::GetTempPath()) ("dbcad-deploy-resolution-" + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $fixtureRoot | Out-Null
function New-FixtureExe([string]$RelativePath, [datetime]$Timestamp) {
    $path = Join-Path $fixtureRoot $RelativePath
    New-Item -ItemType Directory -Path (Split-Path -Parent $path) -Force | Out-Null
    Set-Content -LiteralPath $path -Value 'fixture'
    (Get-Item -LiteralPath $path).LastWriteTime = $Timestamp
    return (Get-Item -LiteralPath $path).FullName
}
function Assert-Path([System.IO.FileInfo]$Actual, [string]$Expected) {
    if ($Actual.FullName -ine $Expected) { throw "Selected $($Actual.FullName), expected $Expected" }
}
try {
    $time = Get-Date
    $debug = New-FixtureExe 'repo\x64\Debug\CAD_DB.exe' $time.AddMinutes(-2)
    $release = New-FixtureExe 'repo\x64\Release\CAD_DB.exe' $time.AddMinutes(-1)
    $legacy = New-FixtureExe 'repo\CAD_DB\x64\Debug\CAD_DB.exe' $time
    $package = New-FixtureExe 'package\client\CAD_DB.exe' $time
    $options = @{
        ProjectRoot = Join-Path $fixtureRoot 'repo\CAD_DB'
        RepoRoot = Join-Path $fixtureRoot 'repo'
        PackageRoot = Join-Path $fixtureRoot 'package'
        ExplicitPath = ''
    }
    Assert-Path (Resolve-LatestCadDbExe @options -SourceCheckout) $release
    Assert-Path (Resolve-LatestCadDbExe @options -SourceCheckout -Configuration Debug) $debug
    (Get-Item -LiteralPath $debug).LastWriteTime = (Get-Item -LiteralPath $release).LastWriteTime
    Assert-Path (Resolve-LatestCadDbExe @options -SourceCheckout) $debug
    Assert-Path (Resolve-LatestCadDbExe @options -Configuration Release) $package
    $options.ExplicitPath = $legacy
    Assert-Path (Resolve-LatestCadDbExe @options -SourceCheckout -Configuration Release) $legacy
    $options.ExplicitPath = ''
    # A missing requested configuration must fail even with a newer legacy exe.
    Remove-Item -LiteralPath $debug
    $failed = $false
    try { Resolve-LatestCadDbExe @options -SourceCheckout -Configuration Debug | Out-Null } catch { $failed = $true }
    if (-not $failed) { throw 'Missing Debug unexpectedly fell back to another executable.' }
    Remove-Item -LiteralPath $release
    $failed = $false
    try { Resolve-LatestCadDbExe @options -SourceCheckout | Out-Null } catch { $failed = $true }
    if (-not $failed) { throw 'Source checkout unexpectedly used a legacy/package executable.' }
    Write-Host '[OK] 7 deployment executable selection regressions passed.'
} finally {
    $resolved = (Resolve-Path -LiteralPath $fixtureRoot).Path
    $tempRoot = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath()).TrimEnd('\') + '\'
    if ($resolved.StartsWith($tempRoot, [System.StringComparison]::OrdinalIgnoreCase) -and
        (Split-Path -Leaf $resolved) -like 'dbcad-deploy-resolution-*') {
        Remove-Item -LiteralPath $resolved -Recurse -Force
    } else { throw "Unexpected fixture directory: $resolved" }
}
