param(
    [string]$Neo4jHost = '127.0.0.1',
    [int]$Neo4jPort = 7687,
    [string]$Neo4jUser = 'neo4j',
    [string]$Neo4jPassword = '',
    [int]$BridgePort = 8199
)
# ACIS 2026 迁移后的数据库全链路回归测试。
# 验证链路（全部走主程序生产代码路径）：
#   启动 CAD_DB.exe --storage-bridge（ACIS init + Neo4j 会话）
#   → POST /projects（建测试项目）
#   → POST /projects/{id}/models（SAT → 实体 → api_save_entity_list_neo4j_part 写图）
#   → GET  /projects/{id}/models/latest（api_restore_entity_list_neo4j_part 图 → 实体 → SAT）
#   → 与提交的原始 SAT 去文件头逐字节比对（与 CheckTestCase 同标准）
#   → 清理测试数据
# 前置条件：
#   1. x64\Release\CAD_DB.exe 已构建（scripts\build.ps1 -Configuration Release）
#   2. Neo4j 5.x + APOC 容器运行中；密码经 -Neo4jPassword 或 $env:CAD_DB_NEO4J_PASSWORD 提供
#   3. 测试模型 SAT 由脚本内嵌的 C++ 最小示例生成（见 $satBlock 下的说明）
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$exe = Join-Path $repoRoot 'x64\Release\CAD_DB.exe'
if (-not (Test-Path $exe)) { throw "Client not built: $exe (run scripts\build.ps1 -Configuration Release first)" }
if (-not $Neo4jPassword) { throw "Neo4j password required: pass -Neo4jPassword or set CAD_DB_NEO4J_PASSWORD (see CAD_DB/backend/.env)" }

# 两个测试模型（与 scripts/tests/acis/entity_tests.cpp 的方块/布尔差一致）。
# SAT 文本用 Python 驱动造太绕，这里直接向 bridge 提交两份预生成 SAT：
# 由 scripts/tests/acis/entity_tests.exe 运行产生的 entity_block.sat / entity_subtract.sat。
$satDir = Join-Path $PSScriptRoot 'acis'
$models = @(
    @{ Name = 'block';    Sat = Join-Path $satDir 'entity_block.sat' },
    @{ Name = 'subtract'; Sat = Join-Path $satDir 'entity_subtract.sat' }
)
foreach ($m in $models) {
    if (-not (Test-Path $m.Sat)) {
        throw "Test SAT missing: $($m.Sat). Run build_entity_tests.bat then entity_tests.exe to generate, or supply your own SAT files."
    }
}

$failed = 0
function Check([bool]$ok, [string]$what) {
    $verdict = if ($ok) { 'PASS' } else { 'FAIL' }
    Write-Host ("  {0,-58} {1}" -f $what, $verdict)
    if (-not $ok) { $script:failed++ }
}

# 与 access.cpp AccessTest::CheckTestCase 相同的比对标准：去前两行文件头后逐字节比较
function Sat-Matches([string]$a, [string]$b) {
    $la = $a -split "`n"; $lb = $b -split "`n"
    if ($la.Count -lt 3 -or $lb.Count -lt 3) { return $false }
    (($la | Select-Object -Skip 2) -join "`n") -eq (($lb | Select-Object -Skip 2) -join "`n")
}

$projectName = "bridge_roundtrip_test_" + (Get-Date -Format 'yyyyMMdd_HHmmss')
$bridgeErr = Join-Path $env:TEMP "bridge_test_$PID.err"

Write-Host "== Starting storage bridge (port $BridgePort) =="
$p = Start-Process -FilePath $exe -ArgumentList '--storage-bridge',
    "--neo4j-host=$Neo4jHost", "--neo4j-port=$Neo4jPort", "--neo4j-user=$Neo4jUser",
    "--neo4j-password=$Neo4jPassword", "--bridge-port=$BridgePort" `
    -WorkingDirectory (Split-Path -Parent $exe) -PassThru -RedirectStandardError $bridgeErr
try {
    Start-Sleep -Seconds 8
    if ($p.HasExited) {
        Write-Host (Get-Content $bridgeErr -Raw -ErrorAction SilentlyContinue)
        throw "bridge exited prematurely: $($p.ExitCode)"
    }
    Check $true "bridge started (ACIS init + Neo4j session)"

    $base = "http://127.0.0.1:$BridgePort"
    $h = Invoke-RestMethod -Uri "$base/health" -TimeoutSec 10
    Check ($h.status -eq 'ok') "GET /health"

    $pr = Invoke-RestMethod -Uri "$base/projects" -Method POST -ContentType 'application/json' `
        -Body (@{ name = $projectName } | ConvertTo-Json) -TimeoutSec 30
    $projectId = $pr.id
    Check ([bool]$projectId) "POST /projects ($projectId)"

    $baseVersion = 0
    foreach ($m in $models) {
        Write-Host "== Model: $($m.Name) =="
        $sat = Get-Content -Raw $m.Sat
        # PS5.1 的 ConvertTo-Json 会把长字符串装箱成 {value:...}，改为手动 JSON 转义。
        # 反斜杠与引号先转义，再统一把换行压成 \n（注意顺序：换行转义里也含反斜杠，
        # 因此先把已有反斜杠翻倍，之后插入的转义序列不会再被二次处理）。
        $bs = [string][char]92
        $esc = $sat.Replace($bs, $bs + $bs).Replace('"', $bs + '"')
        $esc = $esc -replace "`r`n", ($bs + 'n') -replace "`n", ($bs + 'n') -replace "`t", ($bs + 't')
        $payload = '{"author":"bridge_roundtrip_test","content":{"sat":"' + $esc + '"},"base_version":' + $baseVersion + '}'
        $sw = [Diagnostics.Stopwatch]::StartNew()
        $resp = Invoke-RestMethod -Uri "$base/projects/$projectId/models" -Method POST -ContentType 'application/json' -Body $payload -TimeoutSec 120
        $sw.Stop()
        Check ($resp.version -gt $baseVersion) "POST model (v$($resp.version), $($sw.ElapsedMilliseconds)ms)"
        $baseVersion = $resp.version

        $g = Invoke-RestMethod -Uri "$base/projects/$projectId/models/latest" -TimeoutSec 60
        Check ([bool]$g.content.sat) "GET models/latest"
        Check (Sat-Matches $sat $g.content.sat) "$($m.Name): roundtrip SAT matches after header"
    }
} finally {
    if ($p -and -not $p.HasExited) { Stop-Process -Id $p.Id -Force -ErrorAction SilentlyContinue }
}

# 无论成败都清理测试数据（防止残留）
try {
Write-Host "== Cleanup test data =="
$cleanupPy = Join-Path $env:TEMP "bridge_cleanup_$PID.py"
@'
import sys
from neo4j import GraphDatabase
name, host, port, user, pw = sys.argv[1:6]
drv = GraphDatabase.driver(f"bolt://{host}:{port}", auth=(user, pw))
with drv.session() as s:
    s.run("MATCH (p:BridgeProject {name:$n}) DETACH DELETE p", n=name)
    s.run("""MATCH (n) WHERE any(l IN labels(n) WHERE l IN
          ['body','lump','shell','face','loop','coedge','edge','vertex','point',
           'transform','plane-surface','cone-surface','ellipse-curve','straight-curve',
           'BridgeVersion','part']) DETACH DELETE n""")
drv.close()
print("cleanup done")
'@ | Out-File -Encoding utf8 $cleanupPy
$backendPy = Join-Path $repoRoot 'CAD_DB/backend/.venv/Scripts/python.exe'
$pyBin = if (Test-Path $backendPy) { $backendPy } else { 'python' }
& $pyBin $cleanupPy $projectName $Neo4jHost $Neo4jPort $Neo4jUser $Neo4jPassword
Remove-Item $cleanupPy -ErrorAction SilentlyContinue
Write-Host "  cleanup attempted"

Write-Host ""
if ($failed -eq 0) {
    Write-Host "ALL BRIDGE ROUNDTRIP TESTS PASSED" -ForegroundColor Green
    exit 0
} else {
    Write-Host "BRIDGE ROUNDTRIP TESTS HAD FAILURES ($failed)" -ForegroundColor Red
    exit 1
}
} catch {
    Write-Host "  cleanup skipped: $($_.Exception.Message)"
}
