$env:Path += ";C:\Program Files\Microsoft Visual Studio\18\Insiders\MSBuild\Current\Bin\amd64"
$projectPath = Join-Path $PSScriptRoot "CAD_DB.vcxproj"

Write-Host "Starting Debug Build..." -ForegroundColor Cyan
msbuild $projectPath /p:Configuration=Debug /p:Platform=x64
