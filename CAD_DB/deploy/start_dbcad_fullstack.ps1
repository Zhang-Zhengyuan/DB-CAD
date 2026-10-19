param(
    [string]$FastApiHost = "0.0.0.0",
    [int]$FastApiPort = 8000,
    [string]$Neo4jUri = "",
    [string]$Neo4jHost = "",
    [int]$Neo4jPort = 0,
    [string]$Neo4jUser = "",
    [string]$Neo4jPassword = "",
    [string]$ApiPassword = "",
    [string]$Author = "dbcad-web",
    [string]$ConfigPath = "",
    [int]$Neo4jHttpPort = 7474,
    [string]$Neo4jDockerName = "",
    [string]$Neo4jDockerImage = "",
    [string]$Neo4jDockerDataRoot = "",
    [switch]$SkipBrowser = $false,
    [switch]$KeepExistingServices = $false,
    [switch]$SkipNeo4jPortCheck = $false,
    [switch]$SkipNeo4jDocker = $false,
    [switch]$ResetNeo4jDockerData = $false,
    [int]$RetryCount = 30,
    [int]$RetryDelaySeconds = 1
)

# v3 全栈启动（V3_PLAN P7：Qt 客户端退役，浏览器是唯一客户端）：
# Neo4j（docker）+ FastAPI（/api/v1 + /web 静态托管）+ 打开浏览器。
$ErrorActionPreference = "Stop"

foreach ($helperName in @("filesystem", "config", "pe", "runtime", "docker", "processes", "package")) {
    . (Join-Path $PSScriptRoot "lib\$helperName.ps1")
}

$scriptRoot = Resolve-FullPath (Split-Path -Parent $MyInvocation.MyCommand.Path)
$projectRoot = Resolve-FullPath (Join-Path $scriptRoot "..")
$repoRoot = Resolve-FullPath (Join-Path $projectRoot "..")
$backendRoot = Join-Path $projectRoot "backend"
if (-not (Test-Path -LiteralPath (Join-Path $backendRoot "app\main.py"))) {
    throw "Cannot locate backend at $backendRoot. Run this script from CAD_DB\deploy."
}

if ([string]::IsNullOrWhiteSpace($ConfigPath)) {
    $ConfigPath = Join-Path $scriptRoot "dbcad.local.env"
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
if (-not $PSBoundParameters.ContainsKey("Neo4jDockerName")) {
    $Neo4jDockerName = Get-ConfigValue -Config $localConfig -Name "DBCAD_NEO4J_DOCKER_NAME" -DefaultValue "neo4j-apoc"
}
if (-not $PSBoundParameters.ContainsKey("Neo4jDockerImage")) {
    $Neo4jDockerImage = Get-ConfigValue -Config $localConfig -Name "DBCAD_NEO4J_DOCKER_IMAGE" -DefaultValue "neo4j:5.15"
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

$runDir = Ensure-Directory (Join-Path $projectRoot "deploy\run")
$logDir = Ensure-Directory (Join-Path $runDir "logs")
$pidFile = Join-Path $runDir "pids.json"
$stopScript = Join-Path $scriptRoot "stop_dbcad_fullstack.ps1"
if ((-not $KeepExistingServices) -and (Test-Path -LiteralPath $stopScript) -and (Test-Path -LiteralPath $pidFile)) {
    Write-Host "[INFO] Stopping services from previous launcher run..."
    & $stopScript -Quiet -ErrorAction SilentlyContinue
}

$fastApiProbeHost = ConvertTo-LocalHealthHost $FastApiHost
$fastApiUrl = "http://$fastApiProbeHost`:$FastApiPort"
$fastApiListenUrl = "http://$FastApiHost`:$FastApiPort"

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
        throw "Neo4j is not reachable at $Neo4jHost`:$Neo4jPort. Start Neo4j first, pass the correct -Neo4jHost/-Neo4jPort, or use -SkipNeo4jPortCheck."
    }
}
if (-not $SkipNeo4jDocker) {
    Write-Host "[INFO] Checking Neo4j APOC plugin..."
    Wait-Neo4jApoc -ContainerName $Neo4jDockerName -User $Neo4jUser -Password $Neo4jPassword -Attempts $RetryCount -DelaySeconds $RetryDelaySeconds
}

$backendProcess = $null
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
        -ServerDir $backendRoot `
        -HostAddress $FastApiHost `
        -Port $FastApiPort `
        -ApiPassword $ApiPassword `
        -SkipDependencySync $false `
        -LogDir $logDir
    if (-not (Wait-ServiceHealth -Url "$fastApiUrl/health" -Process $backendProcess -ServiceName "FastAPI" -Attempts $RetryCount -DelaySeconds $RetryDelaySeconds)) {
        $exitInfo = if ($backendProcess.HasExited) { " ExitCode=$($backendProcess.ExitCode)." } else { "" }
        Stop-StartedProcess -Process $backendProcess -Name "FastAPI"
        throw "FastAPI failed to become healthy.$exitInfo See logs in $logDir"
    }
}

$webUrl = "$fastApiUrl/web/"
if (-not $SkipBrowser) {
    Write-Host "[INFO] Opening browser: $webUrl"
    Start-Process $webUrl
}

Write-PidFile -Path $pidFile -Data ([ordered]@{
    backend_pid = if ($null -ne $backendProcess) { $backendProcess.Id } else { $null }
    collaboration_mode = "v3_server_authoritative"
    fastapi_url = $fastApiUrl
    started_at = (Get-Date).ToString("o")
})

Write-Host "[OK] DBCAD v3 fullstack is ready."
Write-Host "[INFO] Web client: $webUrl"
Write-Host "[INFO] FastAPI health: $fastApiUrl/health"
Write-Host "[INFO] Neo4j console: $fastApiUrl/webui"
Write-Host "[INFO] PID file: $pidFile"
