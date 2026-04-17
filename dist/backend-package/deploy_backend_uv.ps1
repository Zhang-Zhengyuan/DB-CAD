param(
    [string]$HostAddress = "0.0.0.0",
    [int]$Port = 8000,
    [string]$StorageBackend = "neo4j",
    [string]$Neo4jUri = "bolt://127.0.0.1:7687",
    [string]$Neo4jUser = "neo4j",
    [string]$Neo4jPassword = "change_me",
    [string]$Neo4jDatabase = "neo4j"
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

Write-Host "[INFO] Syncing environment with uv..."
uv sync

$env:CAD_DB_STORAGE_BACKEND = $StorageBackend
if ($StorageBackend -eq "neo4j") {
    $env:CAD_DB_NEO4J_URI = $Neo4jUri
    $env:CAD_DB_NEO4J_USER = $Neo4jUser
    $env:CAD_DB_NEO4J_PASSWORD = $Neo4jPassword
    $env:CAD_DB_NEO4J_DATABASE = $Neo4jDatabase
}

Write-Host "[INFO] Starting backend: http://${HostAddress}:${Port}"
uv run uvicorn app.main:app --host $HostAddress --port $Port
