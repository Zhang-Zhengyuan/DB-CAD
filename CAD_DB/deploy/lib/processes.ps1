# Processes helpers for the fullstack launcher.
function Stop-StartedProcess([object]$Process, [string]$Name) {
    if ($null -eq $Process) {
        return
    }

    try {
        if (-not $Process.HasExited) {
            Write-Warning "Stopping $Name after startup failure (PID $($Process.Id))."
            Stop-Process -Id $Process.Id -Force -ErrorAction SilentlyContinue
        }
    } catch {
    }
}

function Stop-PackageProcesses([string]$PackageRoot) {
    $clientDir = Join-Path $PackageRoot "client"
    $exePaths = @($clientDir)

    $stoppedCount = 0

    foreach ($dir in $exePaths) {
        $exe = Join-Path $dir "CAD_DB.exe"
        if (Test-Path -LiteralPath $exe) {
            $running = Get-Process -Name "CAD_DB" -ErrorAction SilentlyContinue | Where-Object {
                $_.Path -ieq $exe
            }
            foreach ($proc in $running) {
                try {
                    Write-Host "[INFO] Stopping running CAD_DB.exe (PID $($proc.Id)) to allow sync..."
                    Stop-Process -Id $proc.Id -Force -ErrorAction Stop
                    $stoppedCount++
                    Start-Sleep -Milliseconds 500
                } catch {
                    Write-Warning "Failed to stop CAD_DB.exe PID $($proc.Id): $($_.Exception.Message)"
                }
            }
        }
    }

    if ($stoppedCount -gt 0) {
        Start-Sleep -Seconds 1
    }
}

function Stop-ProcessByPort([int]$Port, [string]$Protocol = "TCP") {
    $output = netstat -ano | Select-String -Pattern ":$Port\s+.+LISTENING"
    $pids = $output | ForEach-Object {
        if ($_ -match '\s+(\d+)\s*$') {
            [int]$matches[1]
        }
    } | Select-Object -Unique

    $stoppedCount = 0
    foreach ($processId in $pids) {
        if ($processId -eq 0) { continue }
        try {
            $proc = Get-Process -Id $processId -ErrorAction SilentlyContinue
            if ($null -ne $proc) {
                Write-Host "[INFO] Stopping process using port $Port (PID $processId, name: $($proc.ProcessName))..."
                Stop-Process -Id $processId -Force -ErrorAction Stop
                $stoppedCount++
                Start-Sleep -Milliseconds 500
            }
        } catch {
            $errMsg = $_.Exception.Message
            Write-Warning "Failed to stop process PID $processId on port $Port`: $errMsg"
        }
    }

    if ($stoppedCount -gt 0) {
        Start-Sleep -Seconds 1
    }
    return $stoppedCount
}

function Start-FastApi(
    [string]$ServerDir,
    [string]$HostAddress,
    [int]$Port,
    [string]$ApiPassword,
    [bool]$SkipDependencySync,
    [string]$LogDir
) {
    if (!(Test-Path -LiteralPath (Join-Path $ServerDir "app\main.py")) -or !(Test-Path -LiteralPath (Join-Path $ServerDir "pyproject.toml"))) {
        throw "Backend package is incomplete. Expected app/main.py and pyproject.toml in $ServerDir"
    }


    $uv = Ensure-Uv

    if (-not $SkipDependencySync) {
        Write-Host "[INFO] Syncing FastAPI dependencies with uv..."
        Push-Location $ServerDir
        try {
            & $uv sync --frozen
            if ($LASTEXITCODE -ne 0) { throw "uv sync failed; check Python and network access." }
        } finally {
            Pop-Location
        }
    } else {
        Write-Host "[INFO] Skip dependency sync (-SkipDependencySync)."
    }

    [Environment]::SetEnvironmentVariable("CAD_DB_API_PASSWORD", $ApiPassword, "Process")

    $args = @(
        "run",
        "uvicorn",
        "app.main:app",
        "--host", $HostAddress,
        "--port", "$Port"
    )

    $stdout = Join-Path $LogDir "fastapi.out.log"
    $stderr = Join-Path $LogDir "fastapi.err.log"
    return Start-Process -FilePath $uv -ArgumentList $args -WorkingDirectory $ServerDir -WindowStyle Hidden -PassThru -RedirectStandardOutput $stdout -RedirectStandardError $stderr
}
