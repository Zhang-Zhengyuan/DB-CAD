param(
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$RemainingArgs
)

$ErrorActionPreference = "Stop"

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$launcher = Join-Path $scriptRoot "start_dbcad_fullstack.ps1"
$powershellExe = (Get-Process -Id $PID).Path

if (!(Test-Path -LiteralPath $launcher)) {
    throw "Launcher not found: $launcher"
}

if (!(Test-Path -LiteralPath $powershellExe)) {
    throw "PowerShell host not found: $powershellExe"
}

& $powershellExe -NoProfile -ExecutionPolicy Bypass -File $launcher -ClientCount 2 @RemainingArgs
exit $LASTEXITCODE
