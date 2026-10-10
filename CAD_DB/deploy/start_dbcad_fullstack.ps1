param(
    [string]$FastApiHost = "0.0.0.0",
    [int]$FastApiPort = 8000,
    [string]$BridgeHost = "127.0.0.1",
    [int]$BridgePort = 8100,
    [double]$StorageBridgeTimeoutSeconds = 15,
    [string]$Neo4jUri = "",
    [string]$Neo4jHost = "",
    [int]$Neo4jPort = 0,
    [string]$Neo4jUser = "",
    [string]$Neo4jPassword = "",
    [string]$ApiPassword = "",
    [string]$Author = "dbcad-exe",
    [string]$ClientExePath = "",
    [ValidateSet("Auto", "Debug", "Release")][string]$Configuration = "Auto",
    [string]$PackageRoot = "",
    [string]$ConfigPath = "",
    [string]$WinDeployQtPath = "",
    [int]$Neo4jHttpPort = 7474,
    [string]$Neo4jDockerName = "",
    [string]$Neo4jDockerImage = "",
    [string]$Neo4jDockerDataRoot = "",
    [ValidateRange(0, 16)]
    [int]$ClientCount = 1,
    [switch]$SkipClient = $false,
    [switch]$SkipPackageSync = $false,
    [switch]$PrepareOnly = $false,
    [switch]$SkipDependencySync = $false,
    [switch]$LegacyBridgeMode = $false,
    [switch]$SkipBridgeHealthCheck = $false,
    [switch]$SkipNeo4jPortCheck = $false,
    [switch]$SkipNeo4jDocker = $false,
    [switch]$ResetNeo4jDockerData = $false,
    [switch]$SkipWinDeployQt = $false,
    [switch]$KeepExistingServices = $false,
    [int]$RetryCount = 30,
    [int]$RetryDelaySeconds = 1
)

$ErrorActionPreference = "Stop"

# Keep parameters and startup order stable; helpers have explicit owners.
foreach ($helperName in @("filesystem", "config", "pe", "runtime", "docker", "processes", "package")) {
    . (Join-Path $PSScriptRoot "lib\$helperName.ps1")
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$scriptRoot = Resolve-FullPath $scriptRoot
$sourceProjectRoot = Resolve-FullPath (Join-Path $scriptRoot "..")
$sourceBackendRoot = Join-Path $sourceProjectRoot "backend"
$packageServerRoot = Join-Path $scriptRoot "server"

if (Test-Path -LiteralPath (Join-Path $sourceBackendRoot "app\main.py")) {
    $isSourceCheckout = $true
    $deployRoot = $scriptRoot
    $projectRoot = $sourceProjectRoot
    $repoRoot = Resolve-FullPath (Join-Path $projectRoot "..")
    $backendRoot = $sourceBackendRoot
    if ([string]::IsNullOrWhiteSpace($PackageRoot)) {
        $PackageRoot = Join-Path $repoRoot "dist\DBCAD-fullstack-package"
    }
} elseif (Test-Path -LiteralPath (Join-Path $packageServerRoot "app\main.py")) {
    $isSourceCheckout = $false
    $deployRoot = $scriptRoot
    $PackageRoot = if ([string]::IsNullOrWhiteSpace($PackageRoot)) { $scriptRoot } else { $PackageRoot }
    $repoRoot = Resolve-FullPath (Join-Path $scriptRoot "..\..")
    $candidateProjectRoot = Join-Path $repoRoot "CAD_DB"
    $projectRoot = if (Test-Path -LiteralPath $candidateProjectRoot) { Resolve-FullPath $candidateProjectRoot } else { $scriptRoot }
    $backendRoot = $packageServerRoot
} else {
    throw "Cannot locate backend. Run this script from CAD_DB\deploy or DBCAD-fullstack-package."
}

if ([string]::IsNullOrWhiteSpace($ConfigPath)) {
    $ConfigPath = Join-Path $deployRoot "dbcad.local.env"
}
$ConfigPath = Resolve-FullPath $ConfigPath
$localConfig = Ensure-LocalConfig -ConfigFilePath $ConfigPath
$localConfig = Ensure-UnifiedPasswordConfig -ConfigFilePath $ConfigPath -Config $localConfig
$UnifiedPassword = Get-ConfigValue -Config $localConfig -Name "DBCAD_PASSWORD" -DefaultValue ""

if (-not $PSBoundParameters.ContainsKey("FastApiHost")) {
    $FastApiHost = Get-ConfigValue -Config $localConfig -Name "DBCAD_FASTAPI_HOST" -DefaultValue $FastApiHost
}
if (-not $PSBoundParameters.ContainsKey("FastApiPort")) {
    $FastApiPort = Get-ConfigInt -Config $localConfig -Name "DBCAD_FASTAPI_PORT" -DefaultValue $FastApiPort
}
if (-not $PSBoundParameters.ContainsKey("BridgeHost")) {
    $BridgeHost = Get-ConfigValue -Config $localConfig -Name "DBCAD_BRIDGE_HOST" -DefaultValue $BridgeHost
}
if (-not $PSBoundParameters.ContainsKey("BridgePort")) {
    $BridgePort = Get-ConfigInt -Config $localConfig -Name "DBCAD_BRIDGE_PORT" -DefaultValue $BridgePort
}
if (-not $PSBoundParameters.ContainsKey("Neo4jHost")) {
    $Neo4jHost = Get-ConfigValue -Config $localConfig -Name "DBCAD_NEO4J_HOST" -DefaultValue "127.0.0.1"
}
if (-not $PSBoundParameters.ContainsKey("Neo4jPort")) {
    $Neo4jPort = Get-ConfigInt -Config $localConfig -Name "DBCAD_NEO4J_BOLT_PORT" -DefaultValue 7687
}
if (-not $PSBoundParameters.ContainsKey("Neo4jHttpPort")) {
    $Neo4jHttpPort = Get-ConfigInt -Config $localConfig -Name "DBCAD_NEO4J_HTTP_PORT" -DefaultValue $Neo4jHttpPort
}
if (-not $PSBoundParameters.ContainsKey("Neo4jUser")) {
    $Neo4jUser = Get-ConfigValue -Config $localConfig -Name "DBCAD_NEO4J_USER" -DefaultValue "neo4j"
}
if (-not $PSBoundParameters.ContainsKey("Neo4jPassword")) {
    $Neo4jPassword = $UnifiedPassword
}
if (-not $PSBoundParameters.ContainsKey("ApiPassword")) {
    $ApiPassword = $UnifiedPassword
}
if (-not $PSBoundParameters.ContainsKey("Author")) {
    $Author = Get-ConfigValue -Config $localConfig -Name "DBCAD_AUTHOR" -DefaultValue $Author
}
if (-not $PSBoundParameters.ContainsKey("Neo4jDockerName")) {
    $Neo4jDockerName = Get-ConfigValue -Config $localConfig -Name "DBCAD_NEO4J_DOCKER_NAME" -DefaultValue "neo4j-apoc"
}
if (-not $PSBoundParameters.ContainsKey("Neo4jDockerImage")) {
    $Neo4jDockerImage = Get-ConfigValue -Config $localConfig -Name "DBCAD_NEO4J_DOCKER_IMAGE" -DefaultValue "neo4j:5.15"
}

$PackageRoot = Ensure-Directory $PackageRoot
$pidFileBeforeSync = Join-Path $PackageRoot "run\pids.json"
$stopScriptBeforeSync = Join-Path $deployRoot "stop_dbcad_fullstack.ps1"
if ((-not $KeepExistingServices) -and (Test-Path -LiteralPath $stopScriptBeforeSync) -and (Test-Path -LiteralPath $pidFileBeforeSync)) {
    Write-Host "[INFO] Stopping services from previous launcher run..."
    & $stopScriptBeforeSync -PackageRoot $PackageRoot -Quiet
}

$latestExe = Resolve-LatestCadDbExe -ProjectRoot $projectRoot -RepoRoot $repoRoot -PackageRoot $PackageRoot -ExplicitPath $ClientExePath -Configuration $Configuration -SourceCheckout:$isSourceCheckout
if ([string]::IsNullOrWhiteSpace($WinDeployQtPath)) {
    $WinDeployQtPath = Get-BuildWinDeployQtPath -Exe $latestExe -RepoRoot $repoRoot
}
$newestSource = if ($isSourceCheckout) { Get-ChildItem -LiteralPath $projectRoot -Recurse -File |
    Where-Object { $_.Extension -in @('.cpp', '.h', '.hxx', '.vcxproj') -and $_.FullName -notmatch '\\.venv\\|\\x64\\|\\_deps\\' } |
    Sort-Object LastWriteTime -Descending | Select-Object -First 1 }
if ($newestSource -and $latestExe.LastWriteTime -lt $newestSource.LastWriteTime) {
    throw "Selected executable $($latestExe.FullName) is older than $($newestSource.Name). Rebuild this configuration with scripts/build.ps1 before launching."
}
if ([string]::IsNullOrWhiteSpace($Neo4jUri)) {
    $Neo4jUri = "bolt://$Neo4jHost`:$Neo4jPort"
} else {
    $uriParts = Split-Neo4jUri $Neo4jUri
    if (-not $PSBoundParameters.ContainsKey("Neo4jHost")) { $Neo4jHost = $uriParts.Host }
    if (-not $PSBoundParameters.ContainsKey("Neo4jPort")) { $Neo4jPort = $uriParts.Port }
}

if ([string]::IsNullOrWhiteSpace($Neo4jUser)) { $Neo4jUser = "neo4j" }
if ([string]::IsNullOrWhiteSpace($Neo4jPassword)) { throw "Neo4j password is empty. Set DBCAD_PASSWORD in $ConfigPath or pass -Neo4jPassword." }
if ([string]::IsNullOrWhiteSpace($ApiPassword)) { throw "FastAPI password is empty. Set DBCAD_PASSWORD in $ConfigPath or pass -ApiPassword." }
if ([string]::IsNullOrWhiteSpace($Neo4jDockerName)) { $Neo4jDockerName = "neo4j-apoc" }
if ([string]::IsNullOrWhiteSpace($Neo4jDockerImage)) { $Neo4jDockerImage = "neo4j:5.15" }
if ([string]::IsNullOrWhiteSpace($Neo4jDockerDataRoot)) { $Neo4jDockerDataRoot = Join-Path $repoRoot "neo4j-runtime" }

$fastApiProbeHost = ConvertTo-LocalHealthHost $FastApiHost
$bridgeProbeHost = ConvertTo-LocalHealthHost $BridgeHost
$fastApiUrl = "http://$fastApiProbeHost`:$FastApiPort"
$fastApiListenUrl = "http://$FastApiHost`:$FastApiPort"
$bridgeUrl = "http://$bridgeProbeHost`:$BridgePort"
$bridgeListenUrl = "http://$BridgeHost`:$BridgePort"
$directJsonMode = -not [bool]$LegacyBridgeMode

# Stop any running CAD_DB.exe in the package directories before syncing
if (-not $SkipPackageSync) {
    Stop-PackageProcesses -PackageRoot $PackageRoot
}

if (-not $SkipPackageSync) {
    Write-Host "[INFO] Syncing fullstack package: $PackageRoot"
Write-Host "[INFO] Latest CAD_DB.exe: $($latestExe.FullName) ($($latestExe.LastWriteTime))"
if ($newestSource) { Write-Host "[INFO] Latest source: $($newestSource.FullName) ($($newestSource.LastWriteTime))" }
    $paths = Sync-FullstackPackage `
        -ProjectRoot $projectRoot `
        -RepoRoot $repoRoot `
        -DeployRoot $deployRoot `
        -ConfigPath $ConfigPath `
        -BackendRoot $backendRoot `
        -PackageRoot $PackageRoot `
        -LatestExe $latestExe `
        -FastApiUrl $fastApiUrl `
        -Author $Author `
        -ApiPassword $ApiPassword `
        -BridgeHost $BridgeHost `
        -BridgePort $BridgePort `
        -Neo4jHost $Neo4jHost `
        -Neo4jPort $Neo4jPort `
        -Neo4jUser $Neo4jUser `
        -Neo4jPassword $Neo4jPassword `
        -Neo4jUri $Neo4jUri `
        -StorageBridgeTimeoutSeconds $StorageBridgeTimeoutSeconds `
        -DirectJsonMode (-not [bool]$LegacyBridgeMode)
} else {
    $paths = @{
        ClientDir = Ensure-Directory (Join-Path $PackageRoot "client")
        ServerDir = Ensure-Directory (Join-Path $PackageRoot "server")
        BridgeDir = Ensure-Directory (Join-Path $PackageRoot "server\bridge-bin")
    }
}

$runDir = Ensure-Directory (Join-Path $PackageRoot "run")
$logDir = Ensure-Directory (Join-Path $runDir "logs")
$pidFile = Join-Path $runDir "pids.json"
$clientExe = Join-Path $paths.ClientDir "CAD_DB.exe"
$bridgeExe = Join-Path $paths.BridgeDir "CAD_DB.exe"
if (!(Test-Path -LiteralPath $clientExe)) {
    throw "Client executable not found in package: $clientExe"
}
if ($LegacyBridgeMode -and !(Test-Path -LiteralPath $bridgeExe)) {
    throw "Bridge executable not found in package: $bridgeExe"
}
Assert-ClientRuntime -ExePath $clientExe

if ($PrepareOnly) {
    Write-Host "[OK] DBCAD fullstack package is prepared."
    Write-Host "[INFO] Package: $PackageRoot"
    Write-Host "[INFO] Source executable: $($latestExe.FullName)"
    Write-Host "[INFO] Client executable: $clientExe"
    Write-Host "[INFO] Bridge executable: $bridgeExe"
    return
}

$bridgeProcess = $null
$backendProcess = $null
$clientProcesses = @()

if (-not $SkipNeo4jDocker) {
    Ensure-Neo4jDocker `
        -RepoRoot $repoRoot `
        -ContainerName $Neo4jDockerName `
        -Image $Neo4jDockerImage `
        -Neo4jHostName $Neo4jHost `
        -HttpPort $Neo4jHttpPort `
        -BoltPort $Neo4jPort `
        -User $Neo4jUser `
        -Password $Neo4jPassword `
        -DataRoot $Neo4jDockerDataRoot `
        -ResetData ([bool]$ResetNeo4jDockerData)
}

if (-not $SkipNeo4jPortCheck) {
    Write-Host "[INFO] Checking Neo4j bolt port: $Neo4jHost`:$Neo4jPort"
    if (-not (Wait-TcpPort -HostName $Neo4jHost -Port $Neo4jPort -Attempts $RetryCount -DelaySeconds $RetryDelaySeconds)) {
        throw "Neo4j is not reachable at $Neo4jHost`:$Neo4jPort. Start Neo4j first, pass the correct -Neo4jHost/-Neo4jPort, or use -SkipNeo4jPortCheck for remote/proxied deployments."
    }
}

if (-not $SkipNeo4jDocker) {
    Write-Host "[INFO] Checking Neo4j APOC plugin..."
    Wait-Neo4jApoc `
        -ContainerName $Neo4jDockerName `
        -User $Neo4jUser `
        -Password $Neo4jPassword `
        -Attempts $RetryCount `
        -DelaySeconds $RetryDelaySeconds
}

if ($LegacyBridgeMode -and (Test-ServiceHealth "$bridgeUrl/health")) {
    if ($KeepExistingServices) {
        Write-Warning "Bridge is already healthy at $bridgeUrl/health. Reusing it because -KeepExistingServices was set."
    } else {
        Write-Host "[INFO] Bridge is already running at $bridgeUrl/health. Stopping old process to restart..."
        Stop-ProcessByPort -Port $BridgePort
    }
}

if ($LegacyBridgeMode -and -not (Test-ServiceHealth "$bridgeUrl/health")) {
    Write-Host "[INFO] Starting storage bridge: $bridgeListenUrl"
    $bridgeProcess = Start-StorageBridge `
        -ExePath $bridgeExe `
        -WorkingDirectory $paths.BridgeDir `
        -BridgeHost $BridgeHost `
        -BridgePort $BridgePort `
        -Neo4jHost $Neo4jHost `
        -Neo4jPort $Neo4jPort `
        -Neo4jUser $Neo4jUser `
        -Neo4jPassword $Neo4jPassword `
        -LogDir $logDir

    if (-not (Wait-ServiceHealth -Url "$bridgeUrl/health" -Process $bridgeProcess -ServiceName "Bridge" -Attempts $RetryCount -DelaySeconds $RetryDelaySeconds)) {
        $exitInfo = if ($bridgeProcess.HasExited) { " ExitCode=$($bridgeProcess.ExitCode)." } else { "" }
        Stop-StartedProcess -Process $bridgeProcess -Name "storage bridge"
        throw "Bridge failed to become healthy.$exitInfo See logs in $logDir"
    }
}

if (Test-ServiceHealth "$fastApiUrl/health") {
    if ($KeepExistingServices) {
        Write-Warning "FastAPI is already healthy at $fastApiUrl/health. Reusing it because -KeepExistingServices was set."
    } else {
        Write-Host "[INFO] FastAPI is already running at $fastApiUrl/health. Stopping old process to restart..."
        Stop-ProcessByPort -Port $FastApiPort
    }
}

if (-not (Test-ServiceHealth "$fastApiUrl/health")) {
    Write-Host "[INFO] Starting FastAPI: $fastApiListenUrl"
    $backendProcess = Start-FastApi `
        -ServerDir $paths.ServerDir `
        -HostAddress $FastApiHost `
        -Port $FastApiPort `
        -BridgeUrl $bridgeUrl `
        -BridgeTimeoutSeconds $StorageBridgeTimeoutSeconds `
        -ApiPassword $ApiPassword `
        -DirectJsonMode $directJsonMode `
        -SkipDependencySync ([bool]$SkipDependencySync) `
        -SkipBridgeHealthCheck ([bool]$SkipBridgeHealthCheck -or $directJsonMode) `
        -LogDir $logDir

    if (-not (Wait-ServiceHealth -Url "$fastApiUrl/health" -Process $backendProcess -ServiceName "FastAPI" -Attempts $RetryCount -DelaySeconds $RetryDelaySeconds)) {
        $exitInfo = if ($backendProcess.HasExited) { " ExitCode=$($backendProcess.ExitCode)." } else { "" }
        Stop-StartedProcess -Process $backendProcess -Name "FastAPI"
        Stop-StartedProcess -Process $bridgeProcess -Name "storage bridge"
        throw "FastAPI failed to become healthy.$exitInfo See logs in $logDir"
    }
}

if ((-not $SkipClient) -and $ClientCount -gt 0) {
    Write-Host "[INFO] Starting $ClientCount client instance(s): $clientExe"
    # 调试期：让每个客户端按 PID 把 stdout/stderr 重定向到独立文件，
    # 避免两个 CAD_DB.exe 互相覆盖同一个 log/log_*.txt。
    # 关闭方法：把 $env:DBCAD_REDIRECT_LOG 设为 "0" 再启动，或把这段包到 if ($false) 里。
    $env:DBCAD_REDIRECT_LOG = "1"
    Ensure-Directory (Join-Path $paths.ClientDir 'log') | Out-Null
    for ($i = 1; $i -le $ClientCount; $i++) {
        $clientProcesses += Start-Process -FilePath $clientExe -WorkingDirectory $paths.ClientDir -PassThru
    }
    Start-Sleep -Seconds 2
    foreach ($process in $clientProcesses) {
        $process.Refresh()
        if ($process.HasExited) {
            foreach ($startedClient in $clientProcesses) { Stop-StartedProcess -Process $startedClient -Name 'client' }
            Stop-StartedProcess -Process $backendProcess -Name 'FastAPI'
            Stop-StartedProcess -Process $bridgeProcess -Name 'storage bridge'
            throw "Client PID $($process.Id) exited during startup (exit $($process.ExitCode)). See client/log and verify runtime dependencies."
        }
    }
    Write-Host "[INFO] Per-PID client logs: <client-dir>\log\CAD_DB_<pid>.log"
}

$pidData = [ordered]@{
    package_root = $PackageRoot
    generated_exe = $latestExe.FullName
    client_exe = $clientExe
    bridge_exe = $bridgeExe
    bridge_pid = if ($null -ne $bridgeProcess) { $bridgeProcess.Id } else { $null }
    backend_pid = if ($null -ne $backendProcess) { $backendProcess.Id } else { $null }
    client_pid = if ($clientProcesses.Count -gt 0) { $clientProcesses[0].Id } else { $null }
    client_pids = @($clientProcesses | ForEach-Object { $_.Id })
    client_count = $clientProcesses.Count
    bridge_url = $bridgeUrl
    collaboration_mode = if ($directJsonMode) { "direct_json" } else { "legacy_bridge" }
    fastapi_url = $fastApiUrl
    started_at = (Get-Date).ToString("o")
}
Write-PidFile -Path $pidFile -Data $pidData

Write-Host "[OK] DBCAD fullstack is ready."
Write-Host "[INFO] Package: $PackageRoot"
Write-Host "[INFO] Source executable: $($latestExe.FullName)"
if ($directJsonMode) {
    Write-Host "[INFO] Collaboration mode: direct_json (C++ bridge is not required)"
} else {
    Write-Host "[INFO] Collaboration mode: legacy_bridge"
    Write-Host "[INFO] Bridge health: $bridgeUrl/health"
}
Write-Host "[INFO] FastAPI health: $fastApiUrl/health"
Write-Host "[INFO] FastAPI docs: $fastApiUrl/docs"
Write-Host "[INFO] Neo4j history WebUI: $fastApiUrl/webui"
if (-not $SkipClient) {
    Write-Host "[INFO] Client instances: $($clientProcesses.Count)"
}
Write-Host "[INFO] PID file: $pidFile"
