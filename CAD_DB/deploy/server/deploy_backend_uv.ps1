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
        Write-Host "[INFO] uv 未安装，正在安装..."
        powershell -ExecutionPolicy Bypass -c "irm https://astral.sh/uv/install.ps1 | iex"
        $env:Path = "$env:USERPROFILE\.local\bin;$env:Path"
    }
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$backendRoot = Resolve-Path (Join-Path $scriptRoot "..\..\backend")

Ensure-Uv

Set-Location $backendRoot

Write-Host "[INFO] 使用 uv 创建并同步环境..."
uv sync

$env:CAD_DB_STORAGE_BACKEND = $StorageBackend
if ($StorageBackend -eq "neo4j") {
    $env:CAD_DB_NEO4J_URI = $Neo4jUri
    $env:CAD_DB_NEO4J_USER = $Neo4jUser
    $env:CAD_DB_NEO4J_PASSWORD = $Neo4jPassword
    $env:CAD_DB_NEO4J_DATABASE = $Neo4jDatabase
}

Write-Host "[INFO] 启动后端服务: http://${HostAddress}:${Port}"
uv run uvicorn app.main:app --host $HostAddress --port $Port
