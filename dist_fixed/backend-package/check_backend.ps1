param(
    [string]$BaseUrl = "http://127.0.0.1:8000",
    [string]$BridgeUrl = "http://127.0.0.1:8100"
)

$ErrorActionPreference = "Stop"

$bridgeHealthUrl = "$BridgeUrl/health"
Write-Host "[INFO] Checking bridge health: $bridgeHealthUrl"
$bridgeResponse = Invoke-RestMethod -Uri $bridgeHealthUrl -Method Get -TimeoutSec 10
if ($bridgeResponse.status -ne "ok") {
    throw "Bridge health check failed"
}

Write-Host "[OK] Bridge health check passed"

$healthUrl = "$BaseUrl/health"
Write-Host "[INFO] Checking backend health: $healthUrl"
$response = Invoke-RestMethod -Uri $healthUrl -Method Get -TimeoutSec 10
if ($response.status -ne "ok") {
    throw "Backend health check failed"
}

Write-Host "[OK] Backend health check passed"
Write-Host "[INFO] Swagger docs: $BaseUrl/docs"
