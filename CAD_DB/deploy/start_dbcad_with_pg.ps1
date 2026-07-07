# start_dbcad_with_pg.ps1
#
# One-shot demo launcher:
#   1. starts Postgres + pgAdmin via deploy\start_pg_with_pgadmin.ps1
#   2. ensures pg_demo.exe is built (calls deploy\_build_pg_demo.cmd if missing)
#   3. writes pg_connect_info.conf so the GUI can find the same DB
#   4. launches the DBCAD GUI (CAD_DB.exe) with DBCAD_PG_* env inherited
#
# Stop everything later with:
#   powershell -File deploy\start_pg_with_pgadmin.ps1 -Stop

[CmdletBinding()]
param()

$ErrorActionPreference = "Continue"

$scriptRoot   = $PSScriptRoot
$projectRoot  = Resolve-Path (Join-Path $scriptRoot "..")
Set-Location -LiteralPath $projectRoot

Write-Host "=== Step 1: starting Postgres + pgAdmin ==="
& powershell -NoProfile -ExecutionPolicy Bypass -File (Join-Path $scriptRoot "start_pg_with_pgadmin.ps1")
if ($LASTEXITCODE -ne 0) {
    Write-Host "[FAIL] could not start Postgres + pgAdmin."
    exit 1
}

# Single source for the PG password: deploy\dbcad.local.env.
# The Docker launcher (start_pg_with_pgadmin.ps1) reads DBCAD_PASSWORD from
# here and injects it as POSTGRES_PASSWORD inside the container. We re-read
# it here so pg_connect_info.conf always has the real password, even if the
# container was started externally.
$envFile = Join-Path $scriptRoot 'dbcad.local.env'
if (Test-Path -LiteralPath $envFile) {
    Get-Content -LiteralPath $envFile | ForEach-Object {
        $line = $_.Trim()
        if ($line -and -not $line.StartsWith('#') -and $line.Contains('=')) {
            $k, $v = $line.Split('=', 2)
            $envVar = "Env:$k"
            if (-not (Test-Path -LiteralPath $envVar)) {
                Set-Item -LiteralPath $envVar -Value $v
            }
        }
    }
}

# If the PG container was already running before this script started, we don't
# know its password. Try to read it from environment; otherwise prompt once.
if (-not $env:DBCAD_PG_PASSWORD -or $env:DBCAD_PG_PASSWORD -eq "admin") {
    Write-Host ""
    Write-Host "[INFO] DBCAD_PG_PASSWORD not set (or 'admin'). Trying to auto-detect."
    $probe = & docker exec dbcad-postgres-demo env 2>&1 | Select-String "POSTGRES_PASSWORD"
    if ($probe) {
        $env:DBCAD_PG_PASSWORD = ($probe -replace "POSTGRES_PASSWORD=", "").Trim()
        Write-Host "  auto-detected existing container password."
    } else {
        Write-Host "  [WARN] Could not read password from container; you may need to set it manually."
    }
}
$env:DBCAD_PG_HOST = if ($env:DBCAD_PG_HOST) { $env:DBCAD_PG_HOST } else { "127.0.0.1" }
$env:DBCAD_PG_PORT = if ($env:DBCAD_PG_PORT) { $env:DBCAD_PG_PORT } else { "5432" }
$env:DBCAD_PG_USER = if ($env:DBCAD_PG_USER) { $env:DBCAD_PG_USER } else { "postgres" }
$env:DBCAD_PG_DBNAME = if ($env:DBCAD_PG_DBNAME) { $env:DBCAD_PG_DBNAME } else { "dbcad_demo" }

Write-Host "  PG password source: " -NoNewline
if ($env:DBCAD_PASSWORD) {
    Write-Host "dbcad.local.env (DBCAD_PASSWORD=$($env:DBCAD_PASSWORD.Substring(0, [Math]::Min(8, $env:DBCAD_PASSWORD.Length)))...)"
    if (-not $env:DBCAD_PG_PASSWORD -or $env:DBCAD_PG_PASSWORD -eq "admin") {
        $env:DBCAD_PG_PASSWORD = $env:DBCAD_PASSWORD
    }
} else {
    Write-Host "(none in dbcad.local.env)"
}

Write-Host "=== Step 2: ensuring pg_demo.exe is built ==="
$pgDemoExe = Join-Path $projectRoot "x64\Demo\pg_demo.exe"
if (-not (Test-Path -LiteralPath $pgDemoExe)) {
    Write-Host "  pg_demo.exe missing — running deploy\_build_pg_demo.cmd"
    & cmd.exe /c (Join-Path $scriptRoot "_build_pg_demo.cmd")
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[FAIL] build_pg_demo failed."
        exit 1
    }
} else {
    Write-Host "  pg_demo.exe present."
}

Write-Host "=== Step 3: writing pg_connect_info.conf (matches PG container env) ==="
$confPath = Join-Path $projectRoot "pg_connect_info.conf"

# Always pull the password from dbcad.local.env (single source of truth).
# Falls back to whatever DBCAD_PG_PASSWORD ended up being.
$effectivePassword = $env:DBCAD_PASSWORD
if (-not $effectivePassword) { $effectivePassword = $env:DBCAD_PG_PASSWORD }
$effectiveHost     = if ($env:DBCAD_PG_HOST) { $env:DBCAD_PG_HOST } else { "127.0.0.1" }
$effectivePort     = if ($env:DBCAD_PG_PORT) { $env:DBCAD_PG_PORT } else { "5432" }
$effectiveUser     = if ($env:DBCAD_PG_USER) { $env:DBCAD_PG_USER } else { "postgres" }
$effectiveDb       = if ($env:DBCAD_PG_DBNAME) { $env:DBCAD_PG_DBNAME } else { "dbcad_demo" }

$defaultConf = @(
    $effectiveHost,
    $effectivePort,
    $effectiveUser,
    $effectivePassword,
    $effectiveDb
)
[System.IO.File]::WriteAllLines($confPath, $defaultConf, [System.Text.UTF8Encoding]::new($false))
Write-Host "  wrote pg_connect_info.conf: $effectiveHost / $effectivePort / $effectiveUser / $effectivePassword / $effectiveDb"
Write-Host "  full path: $confPath"
if (-not $effectivePassword) {
    Write-Host "  [WARN] Password was empty in dbcad.local.env and not auto-detected; PostgreSQL menus will fail."
}

Write-Host "=== Step 4: running pg_demo.exe init (one-time schema ensure) ==="
$output = & $pgDemoExe init 2>&1 | ForEach-Object { Write-Host "  $_" }
if ($LASTEXITCODE -ne 0) {
    Write-Host "[WARN] pg_demo init exited with code $LASTEXITCODE; the GUI can still try."
}

Write-Host ""
Write-Host "=== Step 5: launching DBCAD GUI ==="
Write-Host "  - PostgreSQL menu:  > PostgreSQL(P)"
Write-Host "  - 4 menu items: save current, load from DB, list parts, set PG connection"
Write-Host "  - pgAdmin (if it started) at http://localhost:5050  login: admin@dbcad.local / admin"
Write-Host ""

$clientExe = $null
$searched = @()
foreach ($candidate in @("x64\Release\CAD_DB.exe", "x64\Debug\CAD_DB.exe",
                          "x64\Release\CAD_DB-MD.exe", "x64\Debug\CAD_DB-MD.exe",
                          "dist\client\CAD_DB.exe", "dist\client\CAD_DB-MD.exe",
                          "bin\Release\CAD_DB.exe", "bin\Debug\CAD_DB.exe")) {
    $full = Join-Path $projectRoot $candidate
    $searched += $full
    if (Test-Path -LiteralPath $full) {
        $clientExe = $full
        break
    }
}
if (-not $clientExe) {
    Write-Host "[WARN] CAD_DB.exe not found. Searched these paths under $projectRoot :"
    foreach ($p in $searched) {
        Write-Host "       - $p"
    }
    Write-Host ""
    Write-Host "       In Visual Studio, after Build Solution the exe lands at one of these."
    Write-Host "       If it isn't there, your build did not produce an executable (check the"
    Write-Host "       Error List for unresolved symbols)."
    Write-Host ""
    Write-Host "       Manual workaround: launch the exe yourself, e.g."
    Write-Host "       & 'x64\Debug\CAD_DB.exe'"
    exit 2
}

# -----------------------------------------------------------------------------
# Make Qt find its platform plugins (qwindows.dll) at runtime.
# Detect Qt install dir from the qtvs tools property file that VS wrote at
# build time. If that fails, fall back to a few common locations.
# -----------------------------------------------------------------------------
function Resolve-QtPluginPath {
    param([string]$root)
    $qtVars = Join-Path $root 'x64\Debug\qt\qtvars.xml'
    if (-not (Test-Path -LiteralPath $qtVars)) {
        $qtVars = Join-Path $root 'x64\Release\qt\qtvars.xml'
    }
    if (Test-Path -LiteralPath $qtVars) {
        try {
            [xml]$doc = Get-Content -LiteralPath $qtVars -Raw
            $prefix = $doc.Project.PropertyGroup.QMake_QT_INSTALL_PREFIX_
            if ($prefix) {
                $plug = Join-Path $prefix 'plugins'
                if (Test-Path -LiteralPath $plug) { return $plug }
            }
        } catch {}
    }
    foreach ($cand in @('E:\Devtools\6.8.3\msvc2022_64\plugins',
                        'C:\Qt\6.8.3\msvc2022_64\plugins',
                        'C:\Qt\6.8.0\msvc2019_64\plugins',
                        'C:\Qt\6.7.0\msvc2019_64\plugins')) {
        if (Test-Path -LiteralPath $cand) { return $cand }
    }
    return $null
}

$qtPlugins = Resolve-QtPluginPath -root $projectRoot
if ($qtPlugins) {
    Write-Host "Qt plugins: $qtPlugins"
    $env:QT_PLUGIN_PATH = $qtPlugins
    $env:QT_QPA_PLATFORM_PLUGIN_PATH = $qtPlugins
} else {
    Write-Host "[WARN] Could not locate Qt plugins dir. The GUI may fail to start with"
    Write-Host "       'no Qt platform plugin could be initialized'. Set QT_PLUGIN_PATH"
    Write-Host "       manually to your Qt install's plugins folder."
}

# Tell the GUI (CAD_DB.exe) where to find pg_demo.exe and pg_connect_info.conf
# without rebuilding mainwindow.cpp. These mirror the paths the build script
# produces and the location where the deploy script writes the conf.
$pgDemoReal = Join-Path $projectRoot 'x64\Demo\pg_demo.exe'
$pgConfReal = Join-Path $projectRoot 'pg_connect_info.conf'
if (Test-Path -LiteralPath $pgDemoReal) { $env:DBCAD_PG_DEMO = $pgDemoReal }
if (Test-Path -LiteralPath $pgConfReal) { $env:DBCAD_PG_CONNECT = $pgConfReal }
Write-Host "DBCAD_PG_DEMO     = $($env:DBCAD_PG_DEMO)"
Write-Host "DBCAD_PG_CONNECT  = $($env:DBCAD_PG_CONNECT)"

# Compatibility shim for older CAD_DB.exe builds that hardcode the wrong path
# `x64/Debug/x64/Demo/pg_demo.exe` (double x64, off by one). Copy pg_demo.exe
# and its vcpkg DLLs plus the conf into the legacy location so the old binary
# still works while mainwindow.cpp is being reworked.
$guiDir = Split-Path -Parent $clientExe
$legacyDemoDir = Join-Path $guiDir 'x64\Demo'
$legacyPgDemo  = Join-Path $legacyDemoDir 'pg_demo.exe'
$legacyConf    = Join-Path $guiDir 'pg_connect_info.conf'
if ((Test-Path -LiteralPath $pgDemoReal) -and
    -not (Test-Path -LiteralPath $legacyPgDemo)) {
    New-Item -ItemType Directory -Path $legacyDemoDir -Force | Out-Null
    Copy-Item -LiteralPath $pgDemoReal -Destination $legacyPgDemo -Force
    foreach ($dep in @('pqxx.dll', 'libpq.dll',
                       'libssl-3-x64.dll', 'libcrypto-3-x64.dll',
                       'z.dll', 'lz4.dll')) {
        $src = Join-Path 'C:\vcpkg\installed\x64-windows\bin' $dep
        if (Test-Path -LiteralPath $src) {
            Copy-Item -LiteralPath $src -Destination (Join-Path $legacyDemoDir $dep) -Force
        }
    }
    Write-Host "[shim] copied pg_demo.exe + deps to $legacyDemoDir"
}
if ((Test-Path -LiteralPath $pgConfReal) -and
    -not (Test-Path -LiteralPath $legacyConf)) {
    Copy-Item -LiteralPath $pgConfReal -Destination $legacyConf -Force
    Write-Host "[shim] copied pg_connect_info.conf to $legacyConf"
}

Write-Host "Launching $clientExe"
try {
    Start-Process -FilePath $clientExe -ErrorAction Stop
} catch {
    Write-Host "[FAIL] Could not launch $clientExe : $($_.Exception.Message)"
    exit 3
}

exit 0