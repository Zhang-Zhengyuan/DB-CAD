param(
    [string]$OutDir = "dist",
    [string]$StorageBackend = "neo4j",
    [string]$StorageBridgeUrl = "http://127.0.0.1:8100",
    [double]$StorageBridgeTimeoutSeconds = 15,
    [string]$Neo4jUri = "bolt://127.0.0.1:7687",
    [string]$Neo4jUser = "neo4j",
    [string]$Neo4jPassword = "your_password",
    [string]$Neo4jDatabase = "neo4j",
    [string]$ApiPassword = "",
    [switch]$IncludeBridgeBinary = $true,
    [string]$BridgeConfiguration = "Release",
    [string]$BridgePlatform = "x64",
    [string]$QtInstall = "6.9.0_msvc2022_64",
    [string]$WinDeployQtPath = ""
)

$ErrorActionPreference = "Stop"

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptRoot "..\..\..")
$backendRoot = Join-Path $repoRoot "CAD_DB\backend"
$deployRoot = Join-Path $repoRoot "CAD_DB\deploy\server"
$outRoot = Join-Path $repoRoot $OutDir
$pkgRoot = Join-Path $outRoot "backend-package"

function Get-WinDeployQt {
    param([string]$explicitPath)

    if (-not [string]::IsNullOrWhiteSpace($explicitPath)) {
        if (Test-Path $explicitPath) { return (Resolve-Path $explicitPath).Path }
        throw "windeployqt not found at: $explicitPath"
    }

    $cmd = Get-Command windeployqt -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }

    $qmake = Get-Command qmake -ErrorAction SilentlyContinue
    if ($qmake) {
        $qtBin = Split-Path -Parent $qmake.Source
        $candidate = Join-Path $qtBin "windeployqt.exe"
        if (Test-Path $candidate) { return $candidate }
    }

    throw "windeployqt not found. Add Qt bin to PATH or pass -WinDeployQtPath."
}

if (Test-Path $pkgRoot) {
    Remove-Item $pkgRoot -Recurse -Force
}
New-Item -ItemType Directory -Path $pkgRoot | Out-Null

Copy-Item (Join-Path $backendRoot "app") (Join-Path $pkgRoot "app") -Recurse -Force
Copy-Item (Join-Path $backendRoot "pyproject.toml") $pkgRoot -Force
Copy-Item (Join-Path $backendRoot "README.md") $pkgRoot -Force
Copy-Item (Join-Path $backendRoot ".env.example") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "deploy_backend_uv.ps1") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "check_backend.ps1") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "start_backend_oneclick.ps1") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "start_backend_oneclick.cmd") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "start_fullstack_oneclick.ps1") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "start_fullstack_oneclick.cmd") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "set_neo4j_password.ps1") $pkgRoot -Force

if ($IncludeBridgeBinary) {
    $solutionPath = Join-Path $repoRoot "CAD_DB.sln"
    Write-Host "[INFO] Building bridge binary $BridgeConfiguration|$BridgePlatform ..."
    msbuild $solutionPath /t:Build /p:Configuration=$BridgeConfiguration /p:Platform=$BridgePlatform /p:QtInstall=$QtInstall /m

    $projectRoot = Join-Path $repoRoot "CAD_DB"
    $candidateDirs = @(
        (Join-Path $projectRoot "$BridgePlatform\$BridgeConfiguration"),
        (Join-Path $repoRoot "$BridgePlatform\$BridgeConfiguration")
    )
    $bridgeOutDir = $null
    foreach ($dir in $candidateDirs) {
        if (Test-Path (Join-Path $dir "CAD_DB.exe")) {
            $bridgeOutDir = $dir
            break
        }
    }
    if ($null -eq $bridgeOutDir) {
        throw "Cannot locate CAD_DB.exe bridge binary output."
    }

    $bridgePkgDir = Join-Path $pkgRoot "bridge-bin"
    New-Item -ItemType Directory -Path $bridgePkgDir -Force | Out-Null
    Get-ChildItem $bridgeOutDir -File | Where-Object { $_.Extension -in ".exe", ".dll" } | ForEach-Object {
        Copy-Item $_.FullName (Join-Path $bridgePkgDir $_.Name) -Force
    }

    $bridgeExe = Join-Path $bridgePkgDir "CAD_DB.exe"
    if (!(Test-Path $bridgeExe)) {
        throw "CAD_DB.exe not found in bridge package directory: $bridgePkgDir"
    }

    $windeployqt = Get-WinDeployQt -explicitPath $WinDeployQtPath
    Write-Host "[INFO] Running windeployqt for bridge runtime dependencies ..."
    & $windeployqt --release --compiler-runtime --no-translations --no-quick-import --no-system-d3d-compiler $bridgeExe

    $acisBin = Join-Path $repoRoot "_deps\acis\bin"
    if (Test-Path $acisBin) {
        $acisDll = Join-Path $acisBin "SpaACIS.dll"
        if (Test-Path $acisDll) {
            Copy-Item $acisDll (Join-Path $bridgePkgDir "SpaACIS.dll") -Force
        }
    }

    $clientDist = Join-Path $repoRoot "dist\client-Release"
    if (Test-Path $clientDist) {
        Write-Host "[INFO] Copying supplemental runtime DLLs from dist/client-Release ..."
        @("Qt6Core.dll", "Qt6Network.dll", "Qt6WebSockets.dll", "SpaACIS.dll", "dxcompiler.dll", "dxil.dll", "d3dcompiler_47.dll", "opengl32sw.dll") | ForEach-Object {
            $candidate = Join-Path $clientDist $_
            if (Test-Path $candidate) {
                Copy-Item $candidate (Join-Path $bridgePkgDir $_) -Force
            }
        }
    }
}

$envFile = Join-Path $pkgRoot ".env"
@"
CAD_DB_STORAGE_BACKEND=$StorageBackend
CAD_DB_STORAGE_BRIDGE_URL=$StorageBridgeUrl
CAD_DB_STORAGE_BRIDGE_TIMEOUT_SECONDS=$StorageBridgeTimeoutSeconds
CAD_DB_NEO4J_URI=$Neo4jUri
CAD_DB_NEO4J_USER=$Neo4jUser
CAD_DB_NEO4J_PASSWORD=$Neo4jPassword
CAD_DB_NEO4J_DATABASE=$Neo4jDatabase
CAD_DB_API_PASSWORD=$ApiPassword
"@ | Set-Content -Path $envFile -Encoding UTF8

$runFile = Join-Path $pkgRoot "run_server.ps1"
@"
param(
    [string]`$FastApiHost = "0.0.0.0",
    [int]`$FastApiPort = 8000,
    [string]`$BridgeHost = "127.0.0.1",
    [int]`$BridgePort = 8100
)

`$ErrorActionPreference = "Stop"

`$scriptRoot = Split-Path -Parent `$MyInvocation.MyCommand.Path

& (Join-Path `$scriptRoot "start_fullstack_oneclick.ps1") -BridgeHost `$BridgeHost -BridgePort `$BridgePort -FastApiHost `$FastApiHost -FastApiPort `$FastApiPort
"@ | Set-Content -Path $runFile -Encoding UTF8

if (!(Test-Path $outRoot)) {
    New-Item -ItemType Directory -Path $outRoot | Out-Null
}

$zipPath = Join-Path $outRoot "DBCAD-backend-package.zip"
if (Test-Path $zipPath) {
    Remove-Item $zipPath -Force
}
Compress-Archive -Path (Join-Path $pkgRoot "*") -DestinationPath $zipPath
Write-Host "[OK] Backend package created: $zipPath"
