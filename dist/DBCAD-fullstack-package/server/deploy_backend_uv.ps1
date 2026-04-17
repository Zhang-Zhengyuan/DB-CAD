param(
    [string]$HostAddress = "0.0.0.0",
    [int]$Port = 8000,
    [string]$StorageBridgeUrl = "http://127.0.0.1:8100",
    [double]$StorageBridgeTimeoutSeconds = 15,
    [string]$ApiPassword = "",
    [switch]$SkipDependencySync = $false,
    [switch]$SkipBridgeHealthCheck = $false
)

$ErrorActionPreference = "Stop"

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$entryScript = Join-Path $scriptRoot "start_backend_oneclick.ps1"
if (!(Test-Path $entryScript)) {
    throw "start_backend_oneclick.ps1 not found: $entryScript"
}

$invokeArgs = @{
    HostAddress = $HostAddress
    Port = $Port
    StorageBridgeUrl = $StorageBridgeUrl
    StorageBridgeTimeoutSeconds = $StorageBridgeTimeoutSeconds
}

if (-not [string]::IsNullOrWhiteSpace($ApiPassword)) {
    $invokeArgs.ApiPassword = $ApiPassword
}
if ($SkipDependencySync) {
    $invokeArgs.SkipDependencySync = $true
}
if ($SkipBridgeHealthCheck) {
    $invokeArgs.SkipBridgeHealthCheck = $true
}

& $entryScript @invokeArgs
