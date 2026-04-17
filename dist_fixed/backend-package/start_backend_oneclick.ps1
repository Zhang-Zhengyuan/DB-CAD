param(
    [string]$HostAddress = "0.0.0.0",
    [int]$Port = 8000,
    [switch]$BackendOnly = $false
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
        throw "Missing CAD_DB_STORAGE_BRIDGE_URL in .env"
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

if (-not $BackendOnly -and (Test-Path (Join-Path $scriptRoot "start_fullstack_oneclick.ps1"))) {
    Write-Host "[INFO] Fullstack mode detected. Delegating to start_fullstack_oneclick.ps1 ..."
    & (Join-Path $scriptRoot "start_fullstack_oneclick.ps1") -FastApiHost $HostAddress -FastApiPort $Port
    exit $LASTEXITCODE
}

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
Validate-BackendConfig

Write-Host "[INFO] Syncing dependencies with uv..."
uv sync

Write-Host "[INFO] Starting backend: http://${HostAddress}:${Port}"
uv run uvicorn app.main:app --host $HostAddress --port $Port
