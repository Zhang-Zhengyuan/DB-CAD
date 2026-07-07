# start_pg_with_pgadmin.ps1
# Boots two Docker containers:
#   1. dbcad-postgres-demo  — Postgres 16 with dbcad_demo database (pg_demo's
#      target). Persistent volume under DBCAD_DOCKER_DATA\postgres-data.
#   2. dbcad-pgadmin        — pgAdmin 4 web UI, served at http://localhost:5050.
#
# Idempotent: re-running on an already-running stack reports "already up" and
# exits 0. Stops both containers if -Stop is passed.

[CmdletBinding()]
param(
    [switch]$Stop,
    [switch]$ResetData,
    [int]$PgHostPort = 5432,
    [int]$PgAdminPort = 5050,
    [string]$ContainerName = "dbcad-postgres-demo",
    [string]$PgAdminName = "dbcad-pgadmin",
    [string]$PgUser = "postgres",
    [string]$PgDb = "dbcad_demo",
    [string]$PgImage = "postgres:16",
    [string]$PgAdminImage = "dpage/pgadmin4:latest",
    [string]$PgAdminEmail = "admin@dbcad.com",
    [string]$PgAdminPassword = "admin",
    [string]$DataRoot = ""
)

$ErrorActionPreference = "Stop"

function Resolve-FullPath([string]$Path) {
    if ([string]::IsNullOrWhiteSpace($Path)) { return $Path }
    $item = Get-Item -LiteralPath $Path -ErrorAction SilentlyContinue
    if ($null -ne $item) { return $item.FullName }
    $parent = Split-Path -Parent $Path
    if (-not (Test-Path -LiteralPath $parent)) {
        New-Item -ItemType Directory -Path $parent -Force | Out-Null
    }
    if (-not (Test-Path -LiteralPath $Path)) {
        New-Item -ItemType Directory -Path $Path -Force | Out-Null
    }
    return (Get-Item -LiteralPath $Path).FullName
}

if (-not (Get-Command docker -ErrorAction SilentlyContinue)) {
    throw "docker not found in PATH."
}

# default DataRoot under the repo's deploy folder so it survives rebuilds
if ([string]::IsNullOrWhiteSpace($DataRoot)) {
    $DataRoot = Join-Path $PSScriptRoot ".pg-data"
}
$dataRoot = Resolve-FullPath $DataRoot

if ($Stop) {
    Write-Host "[INFO] Stopping $ContainerName and $PgAdminName ..."
    & docker stop $PgAdminName  2>&1 | Out-Null
    & docker rm -f $PgAdminName 2>&1 | Out-Null
    & docker stop $ContainerName 2>&1 | Out-Null
    & docker rm -f $ContainerName 2>&1 | Out-Null
    Write-Host "[OK] stopped"
    exit 0
}

if ($ResetData -and (Test-Path -LiteralPath $dataRoot)) {
    Write-Host "[INFO] Resetting $dataRoot ..."
    Remove-Item -LiteralPath $dataRoot -Recurse -Force
}

$pgDataDir = Join-Path $dataRoot "postgres-data"
if (-not (Test-Path -LiteralPath $pgDataDir)) {
    New-Item -ItemType Directory -Path $pgDataDir -Force | Out-Null
}
$pgDataDir = (Resolve-FullPath $pgDataDir)

# ---- shared user-defined network (enables DNS by container name) --------
$DbCadNetwork = "dbcad-net"
$netExists = $false
$netList = & docker network ls --format "{{.Name}}" 2>$null
foreach ($n in $netList) { if ($n -eq $DbCadNetwork) { $netExists = $true; break } }
if (-not $netExists) {
    Write-Host "[INFO] Creating Docker network '$DbCadNetwork' (user-defined, enables DNS resolution) ..."
    $createOut = & docker network create --driver bridge $DbCadNetwork 2>&1
    if ($LASTEXITCODE -ne 0) {
        # Tolerate a race where another process created it concurrently.
        if ($createOut -match "already exists") {
            Write-Host "[INFO] Network '$DbCadNetwork' already exists (created concurrently); continuing."
        } else {
            throw "docker network create $DbCadNetwork failed: $createOut"
        }
    }
}

# ---- start postgres -------------------------------------------------------
$pgRunning = & docker ps --filter "name=^/$ContainerName$" --format "{{.ID}}" 2>$null
if (-not $pgRunning) {
    $pgExists = & docker ps -a --filter "name=^/$ContainerName$" --format "{{.ID}}" 2>$null
    if ($pgExists) {
        Write-Host "[INFO] Starting existing container $ContainerName ..."
        & docker start $ContainerName *> $null
    } else {
        Write-Host "[INFO] Pulling $PgImage ..."
        & docker pull $PgImage *> $null
        Write-Host "[INFO] Creating container $ContainerName on 127.0.0.1:$PgHostPort ..."
        & docker run -d `
            --name $ContainerName `
            --network $DbCadNetwork `
            -e "POSTGRES_USER=$PgUser" `
            -e "POSTGRES_PASSWORD=$PgAdminPassword" `
            -e "POSTGRES_DB=$PgDb" `
            -p "${PgHostPort}:5432" `
            -v "${pgDataDir}:/var/lib/postgresql/data" `
            $PgImage *> $null
        if ($LASTEXITCODE -ne 0) {
            throw "docker run postgres failed (exit $LASTEXITCODE)."
        }
    }
} else {
    Write-Host "[INFO] $ContainerName is already running."
}

# Wait for pg to accept connections
Write-Host "[INFO] Waiting for Postgres to accept connections on 127.0.0.1:$PgHostPort ..."
$ok = $false
for ($i = 0; $i -lt 30; $i++) {
    $probe = & docker exec $ContainerName pg_isready -U $PgUser 2>&1
    if ($LASTEXITCODE -eq 0) { $ok = $true; break }
    Start-Sleep -Seconds 1
}
if (-not $ok) {
    throw "Postgres did not become ready in time. Check 'docker logs $ContainerName'."
}
Write-Host "[OK] Postgres is up at 127.0.0.1:$PgHostPort  db=$PgDb  user=$PgUser  pw=$PgAdminPassword"

# ---- start pgadmin --------------------------------------------------------
$pgAdminRunning = & docker ps --filter "name=^/$PgAdminName$" --format "{{.ID}}" 2>$null
if (-not $pgAdminRunning) {
    $pgAdminExists = & docker ps -a --filter "name=^/$PgAdminName$" --format "{{.ID}}" 2>$null
    if ($pgAdminExists) {
        Write-Host "[INFO] Starting existing container $PgAdminName ..."
        & docker start $PgAdminName *> $null
    } else {
        Write-Host "[INFO] Pulling $PgAdminImage ..."
        & docker pull $PgAdminImage *> $null
        Write-Host "[INFO] Creating container $PgAdminName on 127.0.0.1:$PgAdminPort ..."
        & docker run -d `
            --name $PgAdminName `
            --network $DbCadNetwork `
            -e "PGADMIN_DEFAULT_EMAIL=$PgAdminEmail" `
            -e "PGADMIN_DEFAULT_PASSWORD=$PgAdminPassword" `
            -e "PGADMIN_LISTEN_PORT=80" `
            -p "${PgAdminPort}:80" `
            $PgAdminImage *> $null
        if ($LASTEXITCODE -ne 0) {
            Write-Warning "docker run pgadmin failed (exit $LASTEXITCODE). You can still use the CLI demo; the GUI is optional."
        }
    }
} else {
    Write-Host "[INFO] $PgAdminName is already running."
}

Write-Host ""
Write-Host "[OK] pgAdmin (if started) at http://localhost:$PgAdminPort"
Write-Host "     login: $PgAdminEmail / $PgAdminPassword"
Write-Host "     connect to host=dbcad-postgres-demo port=5432 user=$PgUser pw=$PgAdminPassword db=$PgDb"
Write-Host ""
Write-Host "Connection settings for pg_demo.exe and the C++ GUI:"
Write-Host "    DBCAD_PG_HOST=127.0.0.1"
Write-Host "    DBCAD_PG_PORT=$PgHostPort"
Write-Host "    DBCAD_PG_USER=$PgUser"
Write-Host "    DBCAD_PG_PASSWORD=$PgAdminPassword"
Write-Host "    DBCAD_PG_DBNAME=$PgDb"

# If the container was already up with an old (random) password, we can't know it.
# Warn the user so the pg_demo init / GUI call knows what to do.
if ($pgRunning -and $PgAdminPassword -ne "admin") {
    Write-Host ""
    Write-Host "[INFO] Container was already running before this script started."
    Write-Host "       If 'pg_demo init' fails with auth error, run with -ResetData to rebuild."
}
exit 0