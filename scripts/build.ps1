param(
    [ValidateSet('Debug', 'Release')][string]$Configuration = 'Release',
    [string]$QtRoot = $env:DBCAD_QT_ROOT,
    [string]$QtMsBuildPath = $env:QtMsBuild,
    [string]$DepsRoot = $env:DBCAD_DEPS_ROOT,
    [string]$PostgresInclude = $env:DBCAD_POSTGRES_INCLUDE,
    [string]$RuntimeDir = $env:DBCAD_RUNTIME_DIR
)
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent $PSScriptRoot
$localProps = Join-Path $repoRoot 'build.local.props'
if (Test-Path -LiteralPath $localProps) {
    [xml]$settings = Get-Content -LiteralPath $localProps -Raw
    foreach ($group in $settings.Project.PropertyGroup) {
        if (-not $QtRoot -and $group.QtInstall) { $QtRoot = [string]$group.QtInstall }
        if (-not $QtMsBuildPath -and $group.QtMsBuild) { $QtMsBuildPath = [string]$group.QtMsBuild }
        if (-not $DepsRoot -and $group.DBCADDepsRoot) { $DepsRoot = ([string]$group.DBCADDepsRoot).Replace('$(MSBuildThisFileDirectory)', "$repoRoot\") }
        if (-not $PostgresInclude -and $group.DBCADPostgresInclude) { $PostgresInclude = ([string]$group.DBCADPostgresInclude).Replace('$(MSBuildThisFileDirectory)', "$repoRoot\") }
    }
}
if (-not $DepsRoot) { $DepsRoot = Join-Path $repoRoot '_deps' }
if (-not $PostgresInclude) { $PostgresInclude = Join-Path $DepsRoot 'postgresql\include' }
if (-not (Test-Path -LiteralPath (Join-Path $repoRoot 'CAD_DB\spatial_license.h'))) {
    throw 'Missing CAD_DB/spatial_license.h. Download the matching 2026 license header from your authorized Spatial account or obtain it from your license administrator. See docs/LOCAL_SETUP.md.'
}
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$msbuildCommand = Get-Command MSBuild.exe -ErrorAction SilentlyContinue
$msbuildPath = if ($msbuildCommand) { $msbuildCommand.Source } elseif (Test-Path -LiteralPath $vswhere) {
    & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
}
if (-not $msbuildPath) { throw 'Install Visual Studio 2022 C++ desktop tools, including the Windows SDK.' }
$buildArgs = @((Join-Path $repoRoot 'CAD_DB.sln'), '/nologo', '/verbosity:minimal', "/p:Configuration=$Configuration", '/p:Platform=x64')
if ($QtRoot) {
    $QtRoot = (Resolve-Path -LiteralPath $QtRoot).Path
    if (-not (Test-Path -LiteralPath (Join-Path $QtRoot 'bin\qmake.exe'))) { throw 'QtRoot must point to the Qt MSVC x64 kit directory.' }
    $buildArgs += "/p:QtInstall=$QtRoot"
}
if ($QtMsBuildPath) { $buildArgs += "/p:QtMsBuild=$QtMsBuildPath" }
if ($DepsRoot) { $buildArgs += "/p:DBCADDepsRoot=$((Resolve-Path -LiteralPath $DepsRoot).Path)" }
if ($PostgresInclude) { $buildArgs += "/p:DBCADPostgresInclude=$((Resolve-Path -LiteralPath $PostgresInclude).Path)" }
& $msbuildPath @buildArgs
if ($LASTEXITCODE -ne 0) { throw "Build failed with exit code $LASTEXITCODE" }
# Stage non-Qt DLLs from the same approved dependency kit used for linking.
# Qt DLLs/plugins are packaged by windeployqt in the deployment launcher.
if (-not $DepsRoot) { $DepsRoot = Join-Path $repoRoot '_deps' }
$outputDir = Join-Path $repoRoot "x64\$Configuration"
$runtimeCandidates = @((Join-Path $DepsRoot 'acis\bin'), (Join-Path $DepsRoot 'openssl\bin'), (Join-Path $DepsRoot 'postgresql\bin'))
if ($RuntimeDir) { $runtimeCandidates += $RuntimeDir }
foreach ($sourceDir in $runtimeCandidates) {
    if (Test-Path -LiteralPath $sourceDir) {
        Get-ChildItem -LiteralPath $sourceDir -Filter '*.dll' -File | Copy-Item -Destination $outputDir -Force
    }
}
# The official Debug import library still imports SPAAcisDs.dll internally.
# Renaming the .lib does not change that import name. Stage the matching DLL
# under its original name in each configuration's separate output directory.
if ($Configuration -eq 'Debug') {
    Copy-Item -LiteralPath (Join-Path $DepsRoot 'acis\bin\SPAAcisDsd.dll') -Destination (Join-Path $outputDir 'SPAAcisDs.dll') -Force
}
foreach ($required in @('libpq.dll', 'libssl-3-x64.dll', 'libcrypto-3-x64.dll', 'SPAAcisDs.dll')) {
    if (-not (Test-Path -LiteralPath (Join-Path $outputDir $required))) { throw "Build succeeded but runtime DLL is missing: $required. Supply -RuntimeDir or the dependency kit; see docs/DEPENDENCIES.md." }
}
Write-Host "[OK] Client build and non-Qt runtime staging complete: $outputDir"
