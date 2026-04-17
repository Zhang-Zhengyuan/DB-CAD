param(
    [string]$BaseUrl = "http://127.0.0.1:8000"
)

$ErrorActionPreference = "Stop"

$healthUrl = "$BaseUrl/health"
Write-Host "[INFO] 检查后端健康状态: $healthUrl"
$response = Invoke-RestMethod -Uri $healthUrl -Method Get -TimeoutSec 10
if ($response.status -ne "ok") {
    throw "后端健康检查失败"
}

Write-Host "[OK] 后端健康检查通过"
Write-Host "[INFO] Swagger 文档: $BaseUrl/docs"
