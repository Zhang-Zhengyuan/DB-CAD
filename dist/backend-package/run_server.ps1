param(
    [string]$HostAddress = "0.0.0.0",
    [int]$Port = 8000
)

$ErrorActionPreference = "Stop"

function Ensure-Uv {
    $uvCmd = Get-Command uv -ErrorAction SilentlyContinue
    if ($null -eq $uvCmd) {
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
        [Environment]::SetEnvironmentVariable($pair[0].Trim(), $pair[1].Trim())
    }
}

function Validate-BackendConfig {
    $storage = [Environment]::GetEnvironmentVariable("CAD_DB_STORAGE_BACKEND")
    if ([string]::IsNullOrWhiteSpace($storage)) { $storage = "neo4j" }

    if ($storage.ToLower() -eq "neo4j") {
        $pwd = [Environment]::GetEnvironmentVariable("CAD_DB_NEO4J_PASSWORD")
        if ([string]::IsNullOrWhiteSpace($pwd) -or $pwd -eq "change_me") {
            throw "Invalid Neo4j password in .env. Please edit CAD_DB_NEO4J_PASSWORD before start."
        }
    }
}

Ensure-Uv
Load-EnvFile (Join-Path (Get-Location) ".env")
Validate-BackendConfig
uv sync
uv run uvicorn app.main:app --host $HostAddress --port $Port
