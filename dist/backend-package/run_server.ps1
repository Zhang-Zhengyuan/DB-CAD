param(
    [string]$FastApiHost = "0.0.0.0",
    [int]$FastApiPort = 8000,
    [string]$BridgeHost = "127.0.0.1",
    [int]$BridgePort = 8100
)

$ErrorActionPreference = "Stop"

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

& (Join-Path $scriptRoot "start_fullstack_oneclick.ps1") -BridgeHost $BridgeHost -BridgePort $BridgePort -FastApiHost $FastApiHost -FastApiPort $FastApiPort
