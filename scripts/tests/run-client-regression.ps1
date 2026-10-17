param([ValidateSet('Release','Debug')][string]$Configuration='Release', [string]$QtRoot=$env:DBCAD_QT_ROOT)
$ErrorActionPreference='Stop'
$taskRoot=Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$taskLogs=Join-Path $taskRoot 'artifacts'
New-Item -ItemType Directory -Path $taskLogs -Force | Out-Null
if(-not $QtRoot -and (Test-Path (Join-Path $taskRoot 'build.local.props'))) {
    [xml]$taskProps=Get-Content (Join-Path $taskRoot 'build.local.props') -Raw
    $QtRoot=[string]($taskProps.Project.PropertyGroup | Where-Object QtInstall | Select-Object -First 1).QtInstall
}
if(-not $QtRoot) { throw 'Supply -QtRoot or configure QtInstall in build.local.props' }
& (Join-Path $taskRoot 'scripts/build.ps1') -Configuration $Configuration -QtRoot $QtRoot
$taskVswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$taskMsbuild=& $taskVswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
if(-not $taskMsbuild) { throw 'MSBuild not found' }
& $taskMsbuild (Join-Path $taskRoot 'CAD_DB/CAD_DB.vcxproj') /nologo /verbosity:minimal "/p:Configuration=$Configuration" /p:Platform=x64 /p:DBCADRegression=true "/p:QtInstall=$QtRoot"
if($LASTEXITCODE -ne 0) { throw 'Client regression build failed' }
$taskExe=Join-Path $taskRoot "x64/$Configuration/client_regression.exe"
& (Join-Path $QtRoot 'bin/windeployqt.exe') --release --no-translations $taskExe
if($LASTEXITCODE -ne 0) { throw 'Qt runtime staging failed' }
$taskPython=Join-Path $taskRoot 'CAD_DB/backend/.venv/Scripts/python.exe'
if(-not (Test-Path $taskPython)) { $taskPython=(Get-Command python -ErrorAction Stop).Source }
$taskListener=[System.Net.Sockets.TcpListener]::new([System.Net.IPAddress]::Loopback,0)
$taskListener.Start()
$taskPort=$taskListener.LocalEndpoint.Port
$taskListener.Stop()
$taskUrl="http://127.0.0.1:$taskPort"
$taskServer=Join-Path $PSScriptRoot 'client_regression_server.py'
$taskService=$null
# Windows PowerShell 5.1 turns redirected native stderr into a terminating
# error under $ErrorActionPreference='Stop', so never stream the exe through
# the pipeline; capture its streams via Start-Process and judge by exit code.
function Invoke-RegressionExe([string]$exe,[string[]]$arguments,[string]$log,[string]$errlog) {
    # PS 5.1 rejects an empty -ArgumentList collection, so only pass it when set.
    if($arguments -and $arguments.Count -gt 0) {
        $run=Start-Process -FilePath $exe -ArgumentList $arguments -WorkingDirectory (Split-Path -Parent $exe) -NoNewWindow -Wait -PassThru -RedirectStandardOutput $log -RedirectStandardError $errlog
    } else {
        $run=Start-Process -FilePath $exe -WorkingDirectory (Split-Path -Parent $exe) -NoNewWindow -Wait -PassThru -RedirectStandardOutput $log -RedirectStandardError $errlog
    }
    if($run.ExitCode -ne 0) { throw "Regression executable failed with exit code $($run.ExitCode); see $log and $errlog" }
}
Push-Location (Split-Path -Parent $taskExe)
try {
    Invoke-RegressionExe $taskExe @() (Join-Path $taskLogs "kernel-contract-$Configuration.log") (Join-Path $taskLogs "kernel-contract-$Configuration.err.log")
    $taskService=Start-Process -FilePath $taskPython -ArgumentList @(('"'+$taskServer+'"'),$taskPort) -WindowStyle Hidden -PassThru -RedirectStandardOutput (Join-Path $taskLogs 'regression-server-out.log') -RedirectStandardError (Join-Path $taskLogs 'regression-server-error.log')
    $taskReady=$false
    for($taskAttempt=0;$taskAttempt -lt 50;$taskAttempt++) {
        try { $null=Invoke-RestMethod "$taskUrl/health" -TimeoutSec 1; $taskReady=$true; break } catch { Start-Sleep -Milliseconds 100 }
    }
    if(-not $taskReady) { throw 'Isolated FastAPI regression server did not start' }
    Invoke-RegressionExe $taskExe @($taskUrl) (Join-Path $taskLogs "client-http-$Configuration.log") (Join-Path $taskLogs "client-http-$Configuration.err.log")
    Write-Host "[OK] $Configuration kernel and real client regressions passed"
} finally {
    Pop-Location
    if($taskService -and -not $taskService.HasExited) { Stop-Process -Id $taskService.Id }
}
