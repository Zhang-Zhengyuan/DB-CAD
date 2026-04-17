param(
    [string]$BridgeHost = "127.0.0.1",
    [int]$BridgePort = 8100,
    [string]$Neo4jHost = "127.0.0.1",
    [int]$Neo4jPort = 7687,
    [string]$Neo4jUser = "neo4j",
    [string]$Neo4jPassword = "",
    [string]$FastApiHost = "0.0.0.0",
    [int]$FastApiPort = 8000,
    [double]$StorageBridgeTimeoutSeconds = 15,
    [string]$ApiPassword = "",
    [int]$RetryCount = 5,
    [int]$RetryDelaySeconds = 3,
    [string]$BridgeExePath = ""
)

$ErrorActionPreference = "Stop"

function Load-EnvFile([string]$EnvFilePath) {
    if (!(Test-Path $EnvFilePath)) { return }

    Get-Content $EnvFilePath | ForEach-Object {
        $line = $_.Trim()
        if ([string]::IsNullOrWhiteSpace($line)) { return }
        if ($line.StartsWith("#")) { return }
        $pair = $line.Split('=', 2)
        if ($pair.Count -ne 2) { return }
        $name = $pair[0].Trim().TrimStart([char]0xFEFF)
        $value = $pair[1].Trim()
        [Environment]::SetEnvironmentVariable($name, $value)
    }
}

function Resolve-BridgeExe([string]$scriptRoot, [string]$explicitPath) {
    if (-not [string]::IsNullOrWhiteSpace($explicitPath)) {
        if (Test-Path $explicitPath) { return (Resolve-Path $explicitPath).Path }
        throw "Bridge exe not found: $explicitPath"
    }

    $repoRoot = Resolve-Path (Join-Path $scriptRoot "..\..\..")
    $candidates = @(
        (Join-Path $scriptRoot "bridge-bin\CAD_DB.exe"),
        (Join-Path $scriptRoot "CAD_DB.exe"),
        (Join-Path $repoRoot "CAD_DB\x64\Release\CAD_DB.exe"),
        (Join-Path $repoRoot "x64\Release\CAD_DB.exe")
    )

    foreach ($candidate in $candidates) {
        if (Test-Path $candidate) {
            return (Resolve-Path $candidate).Path
        }
    }

    throw "Cannot locate CAD_DB.exe for storage bridge mode."
}

function Wait-Health([string]$url, [int]$maxAttempts, [int]$delaySeconds) {
    for ($i = 1; $i -le $maxAttempts; $i++) {
        try {
            $response = Invoke-RestMethod -Uri $url -Method Get -TimeoutSec 5
            if ($response.status -eq "ok") {
                return $true
            }
        } catch {
        }

        if ($i -lt $maxAttempts) {
            Start-Sleep -Seconds $delaySeconds
        }
    }
    return $false
}

function Start-BridgeProcess(
    [string]$bridgeExe,
    [string]$bridgeHost,
    [int]$bridgePort,
    [string]$neo4jHost,
    [int]$neo4jPort,
    [string]$neo4jUser,
    [string]$neo4jPassword
) {
    $bridgeArgs = @(
        "--storage-bridge",
        "--bridge-host", $bridgeHost,
        "--bridge-port", "$bridgePort",
        "--neo4j-host", $neo4jHost,
        "--neo4j-port", "$neo4jPort",
        "--neo4j-user", $neo4jUser
    )

    if (-not [string]::IsNullOrWhiteSpace($neo4jPassword)) {
        $bridgeArgs += @("--neo4j-password", $neo4jPassword)
    }

    return Start-Process -FilePath $bridgeExe -ArgumentList $bridgeArgs -WorkingDirectory (Split-Path $bridgeExe -Parent) -PassThru
}

function Start-FastApiProcess(
    [string]$scriptRoot,
    [string]$fastApiHost,
    [int]$fastApiPort,
    [string]$bridgeUrl,
    [double]$bridgeTimeout,
    [string]$apiPassword
) {
    $scriptPath = Join-Path $scriptRoot "deploy_backend_uv.ps1"
    if (!(Test-Path $scriptPath)) {
        throw "deploy_backend_uv.ps1 not found: $scriptPath"
    }

    $fastApiArgs = @(
        "-NoProfile",
        "-ExecutionPolicy", "Bypass",
        "-File", $scriptPath,
        "-HostAddress", $fastApiHost,
        "-Port", "$fastApiPort",
        "-StorageBridgeUrl", $bridgeUrl,
        "-StorageBridgeTimeoutSeconds", "$bridgeTimeout"
    )

    if (-not [string]::IsNullOrWhiteSpace($apiPassword)) {
        $fastApiArgs += @("-ApiPassword", $apiPassword)
    }

    return Start-Process -FilePath "powershell" -ArgumentList $fastApiArgs -WorkingDirectory $scriptRoot -PassThru
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
Load-EnvFile (Join-Path $scriptRoot ".env")

if (-not $PSBoundParameters.ContainsKey("Neo4jHost")) {
    $envNeo4jUri = [Environment]::GetEnvironmentVariable("CAD_DB_NEO4J_URI")
    if (-not [string]::IsNullOrWhiteSpace($envNeo4jUri)) {
        try {
            $uri = [System.Uri]$envNeo4jUri
            if (-not [string]::IsNullOrWhiteSpace($uri.Host)) { $Neo4jHost = $uri.Host }
            if ($uri.Port -gt 0) { $Neo4jPort = $uri.Port }
        } catch {
        }
    }
}

if (-not $PSBoundParameters.ContainsKey("Neo4jUser")) {
    $envNeo4jUser = [Environment]::GetEnvironmentVariable("CAD_DB_NEO4J_USER")
    if (-not [string]::IsNullOrWhiteSpace($envNeo4jUser)) { $Neo4jUser = $envNeo4jUser }
}

if (-not $PSBoundParameters.ContainsKey("Neo4jPassword")) {
    $envNeo4jPassword = [Environment]::GetEnvironmentVariable("CAD_DB_NEO4J_PASSWORD")
    if (-not [string]::IsNullOrWhiteSpace($envNeo4jPassword)) { $Neo4jPassword = $envNeo4jPassword }
}

if (-not $PSBoundParameters.ContainsKey("ApiPassword")) {
    $envApiPassword = [Environment]::GetEnvironmentVariable("CAD_DB_API_PASSWORD")
    if (-not [string]::IsNullOrWhiteSpace($envApiPassword)) { $ApiPassword = $envApiPassword }
}

if ([string]::IsNullOrWhiteSpace($Neo4jPassword)) {
    throw "Neo4j password is empty. Set CAD_DB_NEO4J_PASSWORD in .env or pass -Neo4jPassword."
}

$bridgeExe = Resolve-BridgeExe -scriptRoot $scriptRoot -explicitPath $BridgeExePath
$bridgeUrl = "http://$BridgeHost`:$BridgePort"
$fastApiUrl = "http://127.0.0.1:$FastApiPort"

$bridgeProcess = $null
$fastApiProcess = $null

try {
    Write-Host "[INFO] Using bridge executable: $bridgeExe"

    $bridgeStarted = $false
    for ($attempt = 1; $attempt -le $RetryCount; $attempt++) {
        Write-Host "[INFO] Starting C++ bridge (attempt $attempt/$RetryCount)..."
        $bridgeProcess = Start-BridgeProcess -bridgeExe $bridgeExe -bridgeHost $BridgeHost -bridgePort $BridgePort -neo4jHost $Neo4jHost -neo4jPort $Neo4jPort -neo4jUser $Neo4jUser -neo4jPassword $Neo4jPassword

        if (Wait-Health -url "$bridgeUrl/health" -maxAttempts 15 -delaySeconds 1) {
            $bridgeStarted = $true
            break
        }

        Write-Warning "Bridge health check failed."
        if ($bridgeProcess -and !$bridgeProcess.HasExited) {
            Stop-Process -Id $bridgeProcess.Id -Force -ErrorAction SilentlyContinue
        }
        if ($attempt -lt $RetryCount) {
            Start-Sleep -Seconds $RetryDelaySeconds
        }
    }

    if (-not $bridgeStarted) {
        throw "Failed to start C++ bridge after retries."
    }

    Write-Host "[OK] Bridge is healthy: $bridgeUrl/health"

    $fastApiStarted = $false
    for ($attempt = 1; $attempt -le $RetryCount; $attempt++) {
        Write-Host "[INFO] Starting FastAPI (attempt $attempt/$RetryCount)..."
        $fastApiProcess = Start-FastApiProcess -scriptRoot $scriptRoot -fastApiHost $FastApiHost -fastApiPort $FastApiPort -bridgeUrl $bridgeUrl -bridgeTimeout $StorageBridgeTimeoutSeconds -apiPassword $ApiPassword

        if (Wait-Health -url "$fastApiUrl/health" -maxAttempts 20 -delaySeconds 1) {
            $fastApiStarted = $true
            break
        }

        Write-Warning "FastAPI health check failed."
        if ($fastApiProcess -and !$fastApiProcess.HasExited) {
            Stop-Process -Id $fastApiProcess.Id -Force -ErrorAction SilentlyContinue
        }
        if ($attempt -lt $RetryCount) {
            Start-Sleep -Seconds $RetryDelaySeconds
        }
    }

    if (-not $fastApiStarted) {
        throw "Failed to start FastAPI after retries."
    }

    Write-Host "[OK] Full stack started successfully"
    Write-Host "[INFO] Bridge health: $bridgeUrl/health"
    Write-Host "[INFO] FastAPI health: $fastApiUrl/health"
    Write-Host "[INFO] FastAPI docs: $fastApiUrl/docs"
    Write-Host "[INFO] Bridge PID: $($bridgeProcess.Id), FastAPI PID: $($fastApiProcess.Id)"

    Wait-Process -Id $fastApiProcess.Id
}
finally {
    if ($fastApiProcess -and !$fastApiProcess.HasExited) {
        Stop-Process -Id $fastApiProcess.Id -Force -ErrorAction SilentlyContinue
    }
    if ($bridgeProcess -and !$bridgeProcess.HasExited) {
        Stop-Process -Id $bridgeProcess.Id -Force -ErrorAction SilentlyContinue
    }
}
