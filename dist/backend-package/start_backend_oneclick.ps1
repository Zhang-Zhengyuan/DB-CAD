param(
    [string]$HostAddress = "0.0.0.0",
    [int]$Port = 8000
)

$ErrorActionPreference = "Stop"

function Ensure-Uv {
    $uvCmd = Get-Command uv -ErrorAction SilentlyContinue
    if ($null -eq $uvCmd) {
        Write-Host "[INFO] uv 未安装，正在安装..."
        powershell -ExecutionPolicy Bypass -c "irm https://astral.sh/uv/install.ps1 | iex"
        $env:Path = "$env:USERPROFILE\.local\bin;$env:Path"
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

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

$candidateRoots = @(
    (Join-Path $scriptRoot "..\..\backend"),
    $scriptRoot
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

Write-Host "[INFO] 使用 uv 同步依赖..."
uv sync

Write-Host "[INFO] 后端启动: http://${HostAddress}:${Port}"
uv run uvicorn app.main:app --host $HostAddress --port $Port
