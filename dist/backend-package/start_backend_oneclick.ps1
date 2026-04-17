param(
    [string]$HostAddress = "0.0.0.0",
    [int]$Port = 8000
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

    if ($storage.ToLower() -eq "neo4j") {
        $pwd = [Environment]::GetEnvironmentVariable("CAD_DB_NEO4J_PASSWORD")
        if ([string]::IsNullOrWhiteSpace($pwd)) {
            throw "Invalid Neo4j password in .env. Please set CAD_DB_NEO4J_PASSWORD to the real password before start."
        }
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
Validate-BackendConfig

Write-Host "[INFO] Syncing dependencies with uv..."
uv sync

Write-Host "[INFO] Starting backend: http://${HostAddress}:${Port}"
uv run uvicorn app.main:app --host $HostAddress --port $Port
