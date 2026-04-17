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

$resolvedStorageBackend = if ($PSBoundParameters.ContainsKey("StorageBackend")) { $StorageBackend } else { [Environment]::GetEnvironmentVariable("CAD_DB_STORAGE_BACKEND") }
if ([string]::IsNullOrWhiteSpace($resolvedStorageBackend)) { $resolvedStorageBackend = "neo4j" }

$env:CAD_DB_STORAGE_BACKEND = $resolvedStorageBackend
if ($resolvedStorageBackend -eq "neo4j") {
    $resolvedNeo4jUri = if ($PSBoundParameters.ContainsKey("Neo4jUri")) { $Neo4jUri } else { [Environment]::GetEnvironmentVariable("CAD_DB_NEO4J_URI") }
    if ([string]::IsNullOrWhiteSpace($resolvedNeo4jUri)) { $resolvedNeo4jUri = "bolt://127.0.0.1:7687" }

    $resolvedNeo4jUser = if ($PSBoundParameters.ContainsKey("Neo4jUser")) { $Neo4jUser } else { [Environment]::GetEnvironmentVariable("CAD_DB_NEO4J_USER") }
    if ([string]::IsNullOrWhiteSpace($resolvedNeo4jUser)) { $resolvedNeo4jUser = "neo4j" }

    $resolvedNeo4jPassword = if ($PSBoundParameters.ContainsKey("Neo4jPassword")) { $Neo4jPassword } else { [Environment]::GetEnvironmentVariable("CAD_DB_NEO4J_PASSWORD") }
    if ([string]::IsNullOrWhiteSpace($resolvedNeo4jPassword)) { $resolvedNeo4jPassword = "change_me" }

    $resolvedNeo4jDatabase = if ($PSBoundParameters.ContainsKey("Neo4jDatabase")) { $Neo4jDatabase } else { [Environment]::GetEnvironmentVariable("CAD_DB_NEO4J_DATABASE") }
    if ([string]::IsNullOrWhiteSpace($resolvedNeo4jDatabase)) { $resolvedNeo4jDatabase = "neo4j" }

    $env:CAD_DB_NEO4J_URI = $resolvedNeo4jUri
    $env:CAD_DB_NEO4J_USER = $resolvedNeo4jUser
    $env:CAD_DB_NEO4J_PASSWORD = $resolvedNeo4jPassword
    $env:CAD_DB_NEO4J_DATABASE = $resolvedNeo4jDatabase
}

Write-Host "[INFO] Starting backend: http://${HostAddress}:${Port}"
uv run uvicorn app.main:app --host $HostAddress --port $Port
