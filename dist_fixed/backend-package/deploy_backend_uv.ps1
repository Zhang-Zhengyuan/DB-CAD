param(
    [string]$HostAddress = "0.0.0.0",
    [int]$Port = 8000,
    [string]$StorageBridgeUrl = "http://127.0.0.1:8100",
    [double]$StorageBridgeTimeoutSeconds = 15,
    [string]$ApiPassword = ""
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
Load-EnvFile (Join-Path $backendRoot ".env")

Write-Host "[INFO] Syncing environment with uv..."
uv sync

$env:CAD_DB_STORAGE_BACKEND = "neo4j"
$env:CAD_DB_STORAGE_BRIDGE_URL = $StorageBridgeUrl
$env:CAD_DB_STORAGE_BRIDGE_TIMEOUT_SECONDS = [string]$StorageBridgeTimeoutSeconds

$resolvedApiPassword = if ($PSBoundParameters.ContainsKey("ApiPassword")) { $ApiPassword } else { [Environment]::GetEnvironmentVariable("CAD_DB_API_PASSWORD") }
if (-not [string]::IsNullOrWhiteSpace($resolvedApiPassword)) {
    $env:CAD_DB_API_PASSWORD = $resolvedApiPassword
}

Write-Host "[INFO] Starting backend: http://${HostAddress}:${Port}"
uv run uvicorn app.main:app --host $HostAddress --port $Port
