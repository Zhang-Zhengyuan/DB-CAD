# Offline regression checks; never starts Docker or a server.
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$deployRoot = Join-Path $repoRoot 'CAD_DB\deploy'
foreach ($name in @('filesystem', 'config', 'runtime', 'processes', 'docker')) {
    . (Join-Path $deployRoot "lib\$name.ps1")
}
function Assert-True($Condition, [string]$Message) {
    if (-not $Condition) { throw $Message }
}

$scratch = Join-Path ([System.IO.Path]::GetTempPath()) ("dbcad-launcher-" + [guid]::NewGuid())
New-Item -ItemType Directory -Path $scratch | Out-Null
try {
    $configPath = Join-Path $scratch 'test.env'
    $config = New-DefaultLocalConfig -Password 'existing-password'
    Write-EnvFile $configPath $config
    $normalized = Ensure-UnifiedPasswordConfig $configPath $config
    Assert-True ($normalized.DBCAD_PASSWORD -ceq 'existing-password') 'Startup rotated an existing password.'
    $legacy = @{ DBCAD_NEO4J_PASSWORD = 'legacy'; DBCAD_FASTAPI_PORT = '18000' }
    $normalized = Ensure-UnifiedPasswordConfig $configPath $legacy
    Assert-True ($normalized.DBCAD_PASSWORD -ceq 'legacy') 'Legacy database password was replaced.'
    Assert-True ($normalized.DBCAD_FASTAPI_PORT -eq '18000') 'Custom port was lost.'

    function Ensure-Uv { return 'uv.exe' }
    function Start-Process {
        param($FilePath, $ArgumentList, $WorkingDirectory, $WindowStyle, [switch]$PassThru,
              $RedirectStandardOutput, $RedirectStandardError)
        $script:captured = @{
            Args = $ArgumentList; Style = $WindowStyle
            Uri = $env:CAD_DB_NEO4J_URI; Password = $env:CAD_DB_NEO4J_PASSWORD
            Worker = $env:CAD_DB_WORKER_EXE; Api = $env:CAD_DB_API_PASSWORD
        }
        return [pscustomobject]@{ Id = 123 }
    }
    $oldUri = [Environment]::GetEnvironmentVariable('CAD_DB_NEO4J_URI', 'Process')
    $oldPassword = [Environment]::GetEnvironmentVariable('CAD_DB_NEO4J_PASSWORD', 'Process')
    Start-FastApi -ServerDir (Join-Path $repoRoot 'CAD_DB\backend') -HostAddress '127.0.0.1' -Port 18000 `
        -ApiPassword 'test-api' -SkipDependencySync $true -LogDir $scratch -BackendEnvironment @{
            CAD_DB_NEO4J_URI = 'bolt://127.0.0.1:17687'
            CAD_DB_NEO4J_PASSWORD = 'test-db'
            CAD_DB_WORKER_EXE = 'C:\worker path\dbcad_worker.exe'
        } | Out-Null
    Assert-True ($captured.Uri -eq 'bolt://127.0.0.1:17687' -and $captured.Password -eq 'test-db') 'Database settings were not passed.'
    Assert-True ($captured.Worker -eq 'C:\worker path\dbcad_worker.exe' -and $captured.Api -eq 'test-api') 'Worker/API settings were not passed.'
    Assert-True ($captured.Style -eq 'Hidden' -and $captured.Args -contains '--frozen' -and $captured.Args -contains '--no-sync') 'Launch settings changed.'
    Assert-True ([Environment]::GetEnvironmentVariable('CAD_DB_NEO4J_URI', 'Process') -ceq $oldUri) 'Caller URI was not restored.'
    Assert-True ([Environment]::GetEnvironmentVariable('CAD_DB_NEO4J_PASSWORD', 'Process') -ceq $oldPassword) 'Caller password was not restored.'

    function Invoke-Docker { return 'The client is unauthorized due to authentication failure.' }
    try {
        Wait-Neo4jApoc -ContainerName 'test' -User 'neo4j' -Password 'test' -Attempts 1 -DelaySeconds 0
        throw 'Authentication failure was accepted.'
    } catch {
        Assert-True ($_.Exception.Message -match '^Neo4j authentication failed') 'Authentication failure was misdiagnosed.'
        Assert-True ($_.Exception.Message -notmatch 'ResetNeo4jDockerData') 'Login error recommended deleting data.'
    }
    function Invoke-Docker { return 'apoc', '"5.15.0"' }
    Wait-Neo4jApoc -ContainerName 'test' -User 'neo4j' -Password 'test' -Attempts 1 -DelaySeconds 0

    $fixture = Join-Path $scratch 'source with spaces'
    $fixtureDeploy = Join-Path $fixture 'CAD_DB\deploy'
    New-Item -ItemType Directory -Path $fixtureDeploy -Force | Out-Null
    Copy-Item -LiteralPath (Join-Path $deployRoot 'lib') -Destination $fixtureDeploy -Recurse
    Copy-Item -LiteralPath (Join-Path $deployRoot 'start_dbcad_fullstack.ps1') -Destination $fixtureDeploy
    foreach ($relative in @('CAD_DB\backend\app\main.py', 'x64\Release\dbcad_worker.exe', 'web\dist\index.html')) {
        $path = Join-Path $fixture $relative
        New-Item -ItemType Directory -Path (Split-Path -Parent $path) -Force | Out-Null
        Set-Content -LiteralPath $path -Value 'fixture'
    }
    Write-EnvFile (Join-Path $fixtureDeploy 'dbcad.local.env') (New-DefaultLocalConfig 'fixture-password-for-regression')
    $launcher = Join-Path $fixtureDeploy 'start_dbcad_fullstack.ps1'
    & $launcher -PrepareOnly
    Assert-True (-not (Test-Path (Join-Path $fixtureDeploy 'run'))) 'PrepareOnly created runtime state.'
    Remove-Item -LiteralPath (Join-Path $fixture 'web\dist\index.html')
    try { & $launcher -PrepareOnly; throw 'Missing web client was accepted.' } catch {
        Assert-True ($_.Exception.Message -match '^Web client is not built') 'Missing web client was misdiagnosed.'
    }
    try { & $launcher -PrepareOnly -Configuration Debug; throw 'Missing worker was accepted.' } catch {
        Assert-True ($_.Exception.Message -match '^Worker not found') 'Missing worker was misdiagnosed.'
    }
    $stopResult = @{ Count = 0 }
    function Stop-Process { param($Id, [switch]$Force, $ErrorAction) $stopResult.Count++ }
    $run = New-Item -ItemType Directory -Path (Join-Path $fixtureDeploy 'run')
    $identity = @{ backend_pid = $PID; backend_start_time_utc = '2000-01-01T00:00:00Z' }
    $identity | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $run.FullName 'pids.json')
    & (Join-Path $deployRoot 'stop_dbcad_fullstack.ps1') -PackageRoot $fixtureDeploy -Quiet
    Assert-True ($stopResult.Count -eq 0) 'Stop accepted a stale process identity.'
    $identity.backend_start_time_utc = (Get-Process -Id $PID).StartTime.ToUniversalTime().ToString('o')
    $identity | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $run.FullName 'pids.json')
    & (Join-Path $deployRoot 'stop_dbcad_fullstack.ps1') -PackageRoot $fixtureDeploy -Quiet
    Assert-True ($stopResult.Count -gt 0) 'Stop refused its recorded process.'
    Write-Host '[OK] Launcher regressions passed.'
} finally {
    # Only this test's unique temporary directory, never a deployment/data path.
    $resolved = [System.IO.Path]::GetFullPath($scratch)
    $tempRoot = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath()).TrimEnd('\') + '\'
    if (-not $resolved.StartsWith($tempRoot, [StringComparison]::OrdinalIgnoreCase) -or (Split-Path -Leaf $resolved) -notlike 'dbcad-launcher-*') {
        throw "Unsafe test cleanup path: $resolved"
    }
    Remove-Item -LiteralPath $resolved -Recurse -Force
}
