param(
    [string]$PackageRoot = "",
    [switch]$Quiet = $false
)

$ErrorActionPreference = "Stop"

function Resolve-FullPath([string]$Path) {
    $item = Get-Item -LiteralPath $Path -ErrorAction SilentlyContinue
    if ($null -ne $item) {
        return $item.FullName
    }

    $parent = Split-Path -Parent $Path
    if ([string]::IsNullOrWhiteSpace($parent)) {
        $parent = "."
    }
    $leaf = Split-Path -Leaf $Path
    return (Join-Path (Resolve-Path $parent).Path $leaf)
}

function Get-DescendantProcessIds([int]$ParentProcessId) {
    $children = @(Get-CimInstance Win32_Process -Filter "ParentProcessId=$ParentProcessId" -ErrorAction SilentlyContinue)
    $ids = @()
    foreach ($child in $children) {
        $ids += [int]$child.ProcessId
        $ids += Get-DescendantProcessIds -ParentProcessId ([int]$child.ProcessId)
    }
    return $ids
}

function Stop-Pid([object]$PidValue, [string]$Name) {
    if ($null -eq $PidValue) {
        return
    }

    $pidText = [string]$PidValue
    if ([string]::IsNullOrWhiteSpace($pidText)) {
        return
    }

    $processId = 0
    if (-not [int]::TryParse($pidText, [ref]$processId)) {
        return
    }

    $process = Get-Process -Id $processId -ErrorAction SilentlyContinue
    if ($null -eq $process) {
        if (-not $Quiet) {
            Write-Host "[INFO] $Name is not running (PID $processId)."
        }
        return
    }

    $descendants = @(Get-DescendantProcessIds -ParentProcessId $processId | Select-Object -Unique)
    if ($descendants.Count -gt 0 -and -not $Quiet) {
        Write-Host "[INFO] Stopping $Name child processes: $($descendants -join ', ')"
    }
    foreach ($childId in ($descendants | Sort-Object -Descending)) {
        Stop-Process -Id $childId -Force -ErrorAction SilentlyContinue
    }

    if (-not $Quiet) {
        Write-Host "[INFO] Stopping $Name (PID $processId)..."
    }
    Stop-Process -Id $processId -Force -ErrorAction SilentlyContinue
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$deployRoot = Resolve-FullPath $scriptRoot
$projectRoot = Resolve-FullPath (Join-Path $deployRoot "..")
$repoRoot = Resolve-FullPath (Join-Path $projectRoot "..")

if ([string]::IsNullOrWhiteSpace($PackageRoot)) {
    $PackageRoot = Join-Path $repoRoot "dist\DBCAD-fullstack-package"
}
$PackageRoot = Resolve-FullPath $PackageRoot
$pidFile = Join-Path $PackageRoot "run\pids.json"

if (!(Test-Path -LiteralPath $pidFile)) {
    if (-not $Quiet) {
        Write-Host "[INFO] PID file not found: $pidFile"
    }
    return
}

$data = Get-Content -LiteralPath $pidFile -Raw | ConvertFrom-Json
Stop-Pid -PidValue $data.client_pid -Name "client"
Stop-Pid -PidValue $data.backend_pid -Name "FastAPI"
Stop-Pid -PidValue $data.bridge_pid -Name "storage bridge"

Remove-Item -LiteralPath $pidFile -Force -ErrorAction SilentlyContinue
if (-not $Quiet) {
    Write-Host "[OK] Stopped processes recorded by launcher."
}
