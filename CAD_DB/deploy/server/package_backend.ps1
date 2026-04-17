param(
    [string]$OutDir = "dist",
    [string]$StorageBackend = "neo4j",
    [string]$Neo4jUri = "bolt://127.0.0.1:7687",
    [string]$Neo4jUser = "neo4j",
    [string]$Neo4jPassword = "change_me",
    [string]$Neo4jDatabase = "neo4j"
)

$ErrorActionPreference = "Stop"

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptRoot "..\..\..")
$backendRoot = Join-Path $repoRoot "CAD_DB\backend"
$deployRoot = Join-Path $repoRoot "CAD_DB\deploy\server"
$outRoot = Join-Path $repoRoot $OutDir
$pkgRoot = Join-Path $outRoot "backend-package"

if (Test-Path $pkgRoot) {
    Remove-Item $pkgRoot -Recurse -Force
}
New-Item -ItemType Directory -Path $pkgRoot | Out-Null

Copy-Item (Join-Path $backendRoot "app") (Join-Path $pkgRoot "app") -Recurse -Force
Copy-Item (Join-Path $backendRoot "pyproject.toml") $pkgRoot -Force
Copy-Item (Join-Path $backendRoot "README.md") $pkgRoot -Force
Copy-Item (Join-Path $backendRoot ".env.example") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "deploy_backend_uv.ps1") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "check_backend.ps1") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "start_backend_oneclick.ps1") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "start_backend_oneclick.cmd") $pkgRoot -Force
Copy-Item (Join-Path $deployRoot "set_neo4j_password.ps1") $pkgRoot -Force

$envFile = Join-Path $pkgRoot ".env"
@"
CAD_DB_STORAGE_BACKEND=$StorageBackend
CAD_DB_NEO4J_URI=$Neo4jUri
CAD_DB_NEO4J_USER=$Neo4jUser
CAD_DB_NEO4J_PASSWORD=$Neo4jPassword
CAD_DB_NEO4J_DATABASE=$Neo4jDatabase
"@ | Set-Content -Path $envFile -Encoding UTF8

$runFile = Join-Path $pkgRoot "run_server.ps1"
@"
param(
    [string]`$HostAddress = "0.0.0.0",
    [int]`$Port = 8000
)

`$ErrorActionPreference = "Stop"

`$scriptRoot = Split-Path -Parent `$MyInvocation.MyCommand.Path

function Ensure-Uv {
    `$uvCmd = Get-Command uv -ErrorAction SilentlyContinue
    if (`$null -eq `$uvCmd) {
        powershell -ExecutionPolicy Bypass -c "irm https://astral.sh/uv/install.ps1 | iex"
        `$env:Path = "`$env:USERPROFILE\.local\bin;`$env:Path"
    }
}

function Load-EnvFile([string]`$EnvFilePath) {
    if (!(Test-Path `$EnvFilePath)) { return }
    Get-Content `$EnvFilePath | ForEach-Object {
        `$line = `$_.Trim()
        if ([string]::IsNullOrWhiteSpace(`$line)) { return }
        if (`$line.StartsWith("#")) { return }
        `$pair = `$line.Split('=', 2)
        if (`$pair.Count -ne 2) { return }
        [Environment]::SetEnvironmentVariable(`$pair[0].Trim(), `$pair[1].Trim())
    }
}

function Validate-BackendConfig {
    `$storage = [Environment]::GetEnvironmentVariable("CAD_DB_STORAGE_BACKEND")
    if ([string]::IsNullOrWhiteSpace(`$storage)) { `$storage = "neo4j" }

    if (`$storage.ToLower() -eq "neo4j") {
        `$pwd = [Environment]::GetEnvironmentVariable("CAD_DB_NEO4J_PASSWORD")
        `$placeholders = @("change_me", "your_password", "password", "neo4j")
        if ([string]::IsNullOrWhiteSpace(`$pwd) -or (`$placeholders -contains `$pwd.ToLower())) {
            throw "Invalid Neo4j password in .env. Please set CAD_DB_NEO4J_PASSWORD to the real password before start."
        }
    }
}

Ensure-Uv
Set-Location `$scriptRoot
Load-EnvFile (Join-Path `$scriptRoot ".env")
Validate-BackendConfig
uv sync
uv run uvicorn app.main:app --host `$HostAddress --port `$Port
"@ | Set-Content -Path $runFile -Encoding UTF8

if (!(Test-Path $outRoot)) {
    New-Item -ItemType Directory -Path $outRoot | Out-Null
}

$zipPath = Join-Path $outRoot "DBCAD-backend-package.zip"
if (Test-Path $zipPath) {
    Remove-Item $zipPath -Force
}
Compress-Archive -Path (Join-Path $pkgRoot "*") -DestinationPath $zipPath
Write-Host "[OK] Backend package created: $zipPath"
