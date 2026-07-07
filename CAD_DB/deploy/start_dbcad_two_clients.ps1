param(
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$RemainingArgs
)

$ErrorActionPreference = "Stop"

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$launcher = Join-Path $scriptRoot "start_dbcad_fullstack.ps1"
$powershellExe = Join-Path $PSHOME "powershell.exe"

if (!(Test-Path -LiteralPath $launcher)) {
    throw "Launcher not found: $launcher"
}

if (!(Test-Path -LiteralPath $powershellExe)) {
    throw "powershell.exe not found: $powershellExe"
}

& $powershellExe -NoProfile -ExecutionPolicy Bypass -File $launcher -ClientCount 2 @RemainingArgs
exit $LASTEXITCODE
