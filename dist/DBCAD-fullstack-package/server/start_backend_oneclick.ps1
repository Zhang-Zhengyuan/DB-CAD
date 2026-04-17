param(
    [string]$HostAddress = "0.0.0.0",
    [int]$Port = 8000,
    [string]$StorageBridgeUrl = "",
    [double]$StorageBridgeTimeoutSeconds = 15,
    [string]$ApiPassword = "",
    [switch]$SkipDependencySync = $false,
    [switch]$SkipBridgeHealthCheck = $false
)

$ErrorActionPreference = "Stop"

function Ensure-Uv {
    $uvCmd = Get-Command uv -ErrorAction SilentlyContinue
    if ($null -eq $uvCmd) {
        Write-Host "[INFO] uv is not installed. Installing..."
        powershell -ExecutionPolicy Bypass -c "irm https://astral.sh/uv/install.ps1 | iex"
        $env:Path = "$env:USERPROFILE\.local\bin;$env:Path"
    }
}

function Validate-BackendConfig {
    $storage = [Environment]::GetEnvironmentVariable("CAD_DB_STORAGE_BACKEND")
    if ([string]::IsNullOrWhiteSpace($storage)) { $storage = "neo4j" }

    if ($storage.ToLower() -ne "neo4j") {
        throw "Only neo4j backend is supported. Set CAD_DB_STORAGE_BACKEND=neo4j"
    }

    $bridgeUrl = [Environment]::GetEnvironmentVariable("CAD_DB_STORAGE_BRIDGE_URL")
    if ([string]::IsNullOrWhiteSpace($bridgeUrl)) {
        throw "Missing CAD_DB_STORAGE_BRIDGE_URL. Set it in .env or pass -StorageBridgeUrl."
    }
}

function Wait-BridgeHealth([string]$bridgeUrl) {
    if ([string]::IsNullOrWhiteSpace($bridgeUrl)) {
        return $false
    }

    try {
        $response = Invoke-RestMethod -Uri "$bridgeUrl/health" -Method Get -TimeoutSec 5
        return $response.status -eq "ok"
    } catch {
        return $false
    }
}

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

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

$candidateRoots = @(
    $scriptRoot,
    (Join-Path $scriptRoot "..\..\backend")
)

$backendRoot = $null
foreach ($candidate in $candidateRoots) {
    if ((Test-Path (Join-Path $candidate "app\main.py")) -and (Test-Path (Join-Path $candidate "pyproject.toml"))) {
        $backendRoot = (Resolve-Path $candidate).Path
        break
    }
}

if ($null -eq $backendRoot) {
    throw "Cannot locate backend root. Expected app/main.py and pyproject.toml near script."
}

Ensure-Uv
Set-Location $backendRoot

Load-EnvFile (Join-Path $scriptRoot ".env")
Load-EnvFile (Join-Path $backendRoot ".env")

if (-not [string]::IsNullOrWhiteSpace($StorageBridgeUrl)) {
    [Environment]::SetEnvironmentVariable("CAD_DB_STORAGE_BRIDGE_URL", $StorageBridgeUrl)
}
if ($StorageBridgeTimeoutSeconds -gt 0) {
    [Environment]::SetEnvironmentVariable("CAD_DB_STORAGE_BRIDGE_TIMEOUT_SECONDS", [string]$StorageBridgeTimeoutSeconds)
}
if (-not [string]::IsNullOrWhiteSpace($ApiPassword)) {
    [Environment]::SetEnvironmentVariable("CAD_DB_API_PASSWORD", $ApiPassword)
}

Validate-BackendConfig

if (-not $SkipDependencySync) {
    Write-Host "[INFO] Syncing dependencies with uv..."
    uv sync
} else {
    Write-Host "[INFO] Skip dependency sync (-SkipDependencySync)."
}

$resolvedBridgeUrl = [Environment]::GetEnvironmentVariable("CAD_DB_STORAGE_BRIDGE_URL")
Write-Host "[INFO] Storage bridge URL: $resolvedBridgeUrl"
if (-not $SkipBridgeHealthCheck) {
    if (Wait-BridgeHealth -bridgeUrl $resolvedBridgeUrl) {
        Write-Host "[OK] Bridge health check passed: $resolvedBridgeUrl/health"
    } else {
        Write-Warning "Bridge health check failed: $resolvedBridgeUrl/health"
        Write-Warning "FastAPI will still start, but storage APIs may return 502 until bridge is healthy."
    }
}

Write-Host "[INFO] Starting backend: http://${HostAddress}:${Port}"
Write-Host "[INFO] Swagger docs: http://127.0.0.1:${Port}/docs"
uv run uvicorn app.main:app --host $HostAddress --port $Port
