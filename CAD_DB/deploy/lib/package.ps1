# Package helpers for the fullstack launcher.
function Write-PackageReadme([string]$PackageRoot, [string]$LatestExePath) {
    $readme = @"
DBCAD fullstack package

Start all services and the client:
  .\start_dbcad_fullstack.cmd

Start all services and two clients:
  .\start_dbcad_two_clients.cmd

Open the Neo4j project history WebUI:
  http://127.0.0.1:8000/webui

Stop services started by the launcher:
  .\stop_dbcad_fullstack.cmd

Prepare package without starting services:
  .\start_dbcad_fullstack.cmd -PrepareOnly

Generated from:
  $LatestExePath
"@
    Write-TextFile -Path (Join-Path $PackageRoot "README_START_HERE.txt") -Text $readme
}

function Remove-ObsoletePackageScripts([string]$ServerDir) {
    $obsolete = @(
        "check_backend.ps1",
        "deploy_backend_uv.ps1",
        "run_server.ps1",
        "set_neo4j_password.ps1",
        "start_backend_oneclick.cmd",
        "start_backend_oneclick.ps1",
        "start_fullstack_oneclick.cmd",
        "start_fullstack_oneclick.ps1",
        "STARTUP_CONFIG.md"
    )

    foreach ($name in $obsolete) {
        $path = Join-Path $ServerDir $name
        if (Test-Path -LiteralPath $path) {
            Remove-Item -LiteralPath $path -Force
        }
    }
}

function Remove-LegacyCredentialFiles([string[]]$Directories) {
    $names = @(
        "neo4j_connect_info.conf",
        "neo4j_connect_info.config",
        "postgresql_connect_info.conf"
    )
    foreach ($dir in $Directories) {
        foreach ($name in $names) {
            $path = Join-Path $dir $name
            if (Test-Path -LiteralPath $path) {
                Remove-Item -LiteralPath $path -Force
            }
        }
    }
}

function Sync-FullstackPackage(
    [string]$ProjectRoot,
    [string]$RepoRoot,
    [string]$DeployRoot,
    [string]$ConfigPath,
    [string]$BackendRoot,
    [string]$PackageRoot,
    [System.IO.FileInfo]$LatestExe,
    [string]$FastApiUrl,
    [string]$Author,
    [string]$ApiPassword,
    [string]$BridgeHost,
    [int]$BridgePort,
    [string]$Neo4jHost,
    [int]$Neo4jPort,
    [string]$Neo4jUser,
    [string]$Neo4jPassword,
    [string]$Neo4jUri,
    [double]$StorageBridgeTimeoutSeconds,
    [bool]$DirectJsonMode
) {
    $clientDir = Ensure-Directory (Join-Path $PackageRoot "client")
    $serverDir = Ensure-Directory (Join-Path $PackageRoot "server")
    $bridgeDir = Ensure-Directory (Join-Path $serverDir "bridge-bin")

    if ($LatestExe.DirectoryName.TrimEnd('\') -ine $clientDir.TrimEnd('\')) {
        Clear-PackagedRuntime -Directory $clientDir -PackageRoot $PackageRoot
        Copy-ClientBuildRuntime -Exe $LatestExe -Destination $clientDir
    }

    Remove-LegacyCredentialFiles -Directories @($clientDir)
    Copy-FileToPathIfDifferent -Source $LatestExe.FullName -Destination (Join-Path $clientDir "CAD_DB.exe")
    # An already prepared standalone package needs no Qt development tools.
    if ($LatestExe.DirectoryName.TrimEnd('\') -ine $clientDir.TrimEnd('\')) {
        Invoke-WinDeployQtIfAvailable -ExePath (Join-Path $clientDir "CAD_DB.exe") -IsDebug (Test-DebugBuild $LatestExe)
    }
    Assert-ClientRuntime -ExePath (Join-Path $clientDir 'CAD_DB.exe')

    $fastApiConfig = "$FastApiUrl`r`n$Author`r`n$ApiPassword`r`n"
    Write-TextFile -Path (Join-Path $clientDir "fastapi_connect_info.conf") -Text $fastApiConfig

    Write-TextFile -Path (Join-Path $clientDir "storage_bridge_connect_info.conf") -Text "$BridgeHost`r`n$BridgePort`r`n"

    Clear-PackagedRuntime -Directory $bridgeDir -PackageRoot $PackageRoot
    Copy-DirectoryContents -Source $clientDir -Destination $bridgeDir
    Copy-FileToPathIfDifferent -Source (Join-Path $clientDir "CAD_DB.exe") -Destination (Join-Path $bridgeDir "CAD_DB.exe")
    Remove-LegacyCredentialFiles -Directories @($bridgeDir)

    $backendSource = Resolve-FullPath $BackendRoot
    $serverTarget = Resolve-FullPath $serverDir
    $serverApp = Join-Path $serverDir "app"
    if ($backendSource.TrimEnd('\') -ine $serverTarget.TrimEnd('\')) {
        if (Test-Path -LiteralPath $serverApp) {
            Remove-DirectorySafe -Path $serverApp -AllowedRoot $PackageRoot
        }
        Copy-Item -LiteralPath (Join-Path $BackendRoot "app") -Destination $serverDir -Recurse -Force
    }

    foreach ($file in @("pyproject.toml", "uv.lock", "requirements.txt", "README.md", ".env.example")) {
        Copy-FileIfExists -Source (Join-Path $BackendRoot $file) -DestinationDir $serverDir
    }

    foreach ($file in @(
        "start_dbcad_fullstack.ps1",
        "start_dbcad_fullstack.cmd",
        "start_dbcad_two_clients.ps1",
        "start_dbcad_two_clients.cmd",
        "stop_dbcad_fullstack.ps1",
        "stop_dbcad_fullstack.cmd"
    )) {
        Copy-FileIfExists -Source (Join-Path $DeployRoot $file) -DestinationDir $PackageRoot
    }
    Copy-DirectoryContents -Source (Join-Path $DeployRoot "lib") -Destination (Join-Path $PackageRoot "lib")
    Copy-FileToPathIfDifferent -Source $ConfigPath -Destination (Join-Path $PackageRoot "dbcad.local.env")

    $envFile = Join-Path $serverDir ".env"
    $envText = @"
CAD_DB_STORAGE_BACKEND=neo4j
CAD_DB_STORAGE_BRIDGE_URL=$(if ($DirectJsonMode) { "" } else { "http://127.0.0.1:$BridgePort" })
CAD_DB_STORAGE_BRIDGE_TIMEOUT_SECONDS=$StorageBridgeTimeoutSeconds
CAD_DB_NEO4J_URI=$Neo4jUri
CAD_DB_NEO4J_USER=$Neo4jUser
CAD_DB_NEO4J_PASSWORD=$Neo4jPassword
CAD_DB_NEO4J_DATABASE=neo4j
CAD_DB_API_PASSWORD=$ApiPassword
CAD_DB_DIRECT_JSON_MODE=$(if ($DirectJsonMode) { "true" } else { "false" })
"@
    Write-TextFile -Path $envFile -Text $envText

    Remove-ObsoletePackageScripts -ServerDir $serverDir
    Write-PackageReadme -PackageRoot $PackageRoot -LatestExePath $LatestExe.FullName

    return @{
        ClientDir = $clientDir
        ServerDir = $serverDir
        BridgeDir = $bridgeDir
    }
}
