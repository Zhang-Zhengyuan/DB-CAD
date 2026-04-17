param(
    [string]$BaseUrl = "http://127.0.0.1:8000"
)

$ErrorActionPreference = "Stop"

$healthUrl = "$BaseUrl/health"
Write-Host "[INFO] Checking backend health: $healthUrl"
$response = Invoke-RestMethod -Uri $healthUrl -Method Get -TimeoutSec 10
if ($response.status -ne "ok") {
    throw "Backend health check failed"
}

Write-Host "[OK] Backend health check passed"
Write-Host "[INFO] Swagger docs: $BaseUrl/docs"
