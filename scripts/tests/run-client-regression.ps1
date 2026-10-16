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
$taskPython=(Get-Command python -ErrorAction Stop).Source
$taskListener=[System.Net.Sockets.TcpListener]::new([System.Net.IPAddress]::Loopback,0)
$taskListener.Start()
$taskPort=$taskListener.LocalEndpoint.Port
$taskListener.Stop()
$taskUrl="http://127.0.0.1:$taskPort"
$taskServer=Join-Path $PSScriptRoot 'client_regression_server.py'
$taskService=$null
Push-Location (Split-Path -Parent $taskExe)
try {
    & $taskExe *> (Join-Path $taskLogs "kernel-contract-$Configuration.log")
    if($LASTEXITCODE -ne 0) { throw 'Kernel contracts failed; see artifacts logs' }
    $taskService=Start-Process -FilePath $taskPython -ArgumentList @(('"'+$taskServer+'"'),$taskPort) -WindowStyle Hidden -PassThru -RedirectStandardOutput (Join-Path $taskLogs 'regression-server-out.log') -RedirectStandardError (Join-Path $taskLogs 'regression-server-error.log')
    $taskReady=$false
    for($taskAttempt=0;$taskAttempt -lt 50;$taskAttempt++) {
        try { $null=Invoke-RestMethod "$taskUrl/health" -TimeoutSec 1; $taskReady=$true; break } catch { Start-Sleep -Milliseconds 100 }
    }
    if(-not $taskReady) { throw 'Isolated FastAPI regression server did not start' }
    & $taskExe $taskUrl *> (Join-Path $taskLogs "client-http-$Configuration.log")
    if($LASTEXITCODE -ne 0) { throw 'Client HTTP/widget regression failed; see artifacts logs' }
    Write-Host "[OK] $Configuration kernel and real client regressions passed"
} finally {
    Pop-Location
    if($taskService -and -not $taskService.HasExited) { Stop-Process -Id $taskService.Id }
}
