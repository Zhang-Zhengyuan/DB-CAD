param(
    [string]$Configuration = "Debug",
    [string]$Platform = "x64",
    [string]$ServerBaseUrl = "http://127.0.0.1:8000",
    [string]$Author = "dbcad-exe",
    [string]$WinDeployQtPath = ""
)

$ErrorActionPreference = "Stop"

function Get-WinDeployQt {
    $cmd = Get-Command windeployqt -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }

    $qmake = Get-Command qmake -ErrorAction SilentlyContinue
    if ($qmake) {
        $qtBin = Split-Path -Parent $qmake.Source
        $candidate = Join-Path $qtBin "windeployqt.exe"
        if (Test-Path $candidate) { return $candidate }
    }

    throw "windeployqt not found. Add Qt bin to PATH first."
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptRoot "..\..\..")
$solutionPath = Join-Path $repoRoot "CAD_DB.sln"
$projectRoot = Join-Path $repoRoot "CAD_DB"

Write-Host "[INFO] Building client $Configuration|$Platform ..."
msbuild $solutionPath /t:Build /p:Configuration=$Configuration /p:Platform=$Platform /m

$candidateDirs = @(
    (Join-Path $projectRoot "$Platform\$Configuration"),
    (Join-Path $repoRoot "$Platform\$Configuration")
)
$existingDirs = @($candidateDirs | Where-Object { Test-Path $_ })
if ($existingDirs.Count -eq 0) {
    throw "Build output directory not found in expected locations."
}

$exe = $null
$buildDir = $null
foreach ($dir in $existingDirs) {
    $found = Get-ChildItem $dir -Filter *.exe | Where-Object { $_.Name -notlike "*.vshost.exe" } | Select-Object -First 1
    if ($found) {
        $exe = $found
        $buildDir = $dir
        break
    }
}
if ($null -eq $exe) {
    throw "No executable found in: $($existingDirs -join ', ')"
}

$packageDir = Join-Path $repoRoot "dist\client"
if (Test-Path $packageDir) {
    Remove-Item $packageDir -Recurse -Force
}
New-Item -ItemType Directory -Path $packageDir | Out-Null

Copy-Item $exe.FullName $packageDir -Force

$acisBin = Join-Path $repoRoot "_deps\acis\bin"
if (Test-Path $acisBin) {
    Write-Host "[INFO] Copying ACIS runtime DLLs ..."
    Get-ChildItem $acisBin -Filter *.dll | ForEach-Object {
        Copy-Item $_.FullName (Join-Path $packageDir $_.Name) -Force
    }
}

$windeployqt = if (![string]::IsNullOrWhiteSpace($WinDeployQtPath)) { $WinDeployQtPath } else { Get-WinDeployQt }
if (!(Test-Path $windeployqt)) {
    throw "windeployqt not found at: $windeployqt"
}
$deployMode = if ($Configuration -ieq "Debug") { "--debug" } else { "--release" }
Write-Host "[INFO] Running windeployqt ..."
& $windeployqt $deployMode --compiler-runtime (Join-Path $packageDir $exe.Name)

$fastapiConfig = Join-Path $packageDir "fastapi_connect_info.conf"
Set-Content -Path $fastapiConfig -Value "$ServerBaseUrl`n$Author" -Encoding UTF8

$neo4jConfSrc = Join-Path $projectRoot "neo4j_connect_info.conf"
if (Test-Path $neo4jConfSrc) {
    Copy-Item $neo4jConfSrc (Join-Path $packageDir "neo4j_connect_info.conf") -Force
}

$readme = Join-Path $packageDir "README_DEPLOY.txt"
@"
DBCAD Client Package

1) Run: $($exe.Name)
2) Configure FastAPI endpoint in fastapi_connect_info.conf
3) For local neo4j mode configure neo4j_connect_info.conf

Default backend URL:
$ServerBaseUrl
"@ | Set-Content -Path $readme -Encoding UTF8

$zipPath = Join-Path $repoRoot "dist\DBCAD-client-$Configuration.zip"
if (Test-Path $zipPath) {
    Remove-Item $zipPath -Force
}
Compress-Archive -Path (Join-Path $packageDir "*") -DestinationPath $zipPath

Write-Host "[OK] Client package created: $zipPath"
