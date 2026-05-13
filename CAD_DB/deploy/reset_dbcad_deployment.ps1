param(
    [string]$ConfigPath = "",
    [switch]$KeepDist = $false,
    [switch]$KeepNeo4jData = $false,
    [switch]$KeepDockerContainer = $false
)

$ErrorActionPreference = "Stop"

function Resolve-FullPath([string]$Path) {
    $item = Get-Item -LiteralPath $Path -ErrorAction SilentlyContinue
    if ($null -ne $item) { return $item.FullName }
    $parent = Split-Path -Parent $Path
    if ([string]::IsNullOrWhiteSpace($parent)) { $parent = "." }
    return (Join-Path (Resolve-Path $parent).Path (Split-Path -Leaf $Path))
}

function Read-EnvFile([string]$Path) {
    $values = @{}
    if (!(Test-Path -LiteralPath $Path)) { return $values }
    Get-Content -LiteralPath $Path | ForEach-Object {
        $line = $_.Trim()
        if ([string]::IsNullOrWhiteSpace($line) -or $line.StartsWith("#")) { return }
        $pair = $line.Split("=", 2)
        if ($pair.Count -ne 2) { return }
        $values[$pair[0].Trim()] = $pair[1].Trim()
    }
    return $values
}

function Remove-DirectorySafe([string]$Path, [string]$AllowedRoot) {
    if (!(Test-Path -LiteralPath $Path)) { return }
    $target = (Resolve-Path -LiteralPath $Path).Path
    $root = (Resolve-Path -LiteralPath $AllowedRoot).Path
    if (-not $target.StartsWith($root, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing to delete outside allowed root. Target=$target Root=$root"
    }
    Write-Host "[INFO] Removing directory: $target"
    Remove-Item -LiteralPath $target -Recurse -Force
}

function Invoke-Docker([string[]]$Arguments) {
    $docker = Get-Command docker -ErrorAction SilentlyContinue
    if ($null -eq $docker) { return }
    $oldErrorActionPreference = $ErrorActionPreference
    $global:LASTEXITCODE = 0
    try {
        $ErrorActionPreference = "Continue"
        & $docker.Source @Arguments *> $null
    } finally {
        $ErrorActionPreference = $oldErrorActionPreference
    }
}

$scriptRoot = Resolve-FullPath (Split-Path -Parent $MyInvocation.MyCommand.Path)
$projectRoot = Resolve-FullPath (Join-Path $scriptRoot "..")
$repoRoot = Resolve-FullPath (Join-Path $projectRoot "..")

if ([string]::IsNullOrWhiteSpace($ConfigPath)) {
    $ConfigPath = Join-Path $scriptRoot "dbcad.local.env"
}
$config = Read-EnvFile $ConfigPath
$containerName = if ($config.ContainsKey("DBCAD_NEO4J_DOCKER_NAME")) { $config["DBCAD_NEO4J_DOCKER_NAME"] } else { "neo4j-apoc" }

$stopScript = Join-Path $scriptRoot "stop_dbcad_fullstack.ps1"
if (Test-Path -LiteralPath $stopScript) {
    & $stopScript -Quiet
}

if (-not $KeepDockerContainer) {
    Write-Host "[INFO] Removing Docker container if it exists: $containerName"
    Invoke-Docker -Arguments @("rm", "-f", $containerName)
}

if (-not $KeepNeo4jData) {
    foreach ($path in @(
        (Join-Path $repoRoot "neo4j-runtime"),
        (Join-Path $repoRoot "neo4j-conf"),
        (Join-Path $repoRoot "neo4j-data"),
        (Join-Path $repoRoot "neo4j-plugins"),
        (Join-Path $projectRoot "neo4j-conf"),
        (Join-Path $projectRoot "neo4j-data"),
        (Join-Path $projectRoot "neo4j-plugins")
    )) {
        Remove-DirectorySafe -Path $path -AllowedRoot $repoRoot
    }
}

if (-not $KeepDist) {
    Remove-DirectorySafe -Path (Join-Path $repoRoot "dist") -AllowedRoot $repoRoot
}

$legacySecretFiles = @(
    (Join-Path $projectRoot "neo4j_connect_info.conf"),
    (Join-Path $projectRoot "fastapi_connect_info.conf"),
    (Join-Path $projectRoot "postgresql_connect_info.conf"),
    (Join-Path $repoRoot "x64\Debug\neo4j_connect_info.config"),
    (Join-Path $repoRoot "x64\Debug\neo4j_connect_info.conf"),
    (Join-Path $repoRoot "x64\Release\neo4j_connect_info.conf")
)
foreach ($path in $legacySecretFiles) {
    if (Test-Path -LiteralPath $path) {
        Write-Host "[INFO] Removing legacy credential/config file: $path"
        Remove-Item -LiteralPath $path -Force
    }
}

Write-Host "[OK] Deployment reset completed."
