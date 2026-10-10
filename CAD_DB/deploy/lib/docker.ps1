# Docker helpers for the fullstack launcher.
function Invoke-Docker([string[]]$Arguments, [switch]$AllowFailure = $false) {
    $docker = Get-Command docker -ErrorAction SilentlyContinue
    if ($null -eq $docker) {
        if ($AllowFailure) {
            return $null
        }
        throw "Docker is not available. Install Docker Desktop or pass -SkipNeo4jDocker."
    }

    $oldErrorActionPreference = $ErrorActionPreference
    try {
        $ErrorActionPreference = "Continue"
        $output = & $docker.Source @Arguments 2>&1
        $exitCode = $LASTEXITCODE
    } finally {
        $ErrorActionPreference = $oldErrorActionPreference
    }

    if ($exitCode -ne 0) {
        if ($AllowFailure) {
            return $output
        }
        throw "docker $($Arguments -join ' ') failed: $output"
    }
    return $output
}

function Remove-DirectorySafe([string]$Path, [string]$AllowedRoot) {
    if (!(Test-Path -LiteralPath $Path)) {
        return
    }

    $target = (Resolve-Path -LiteralPath $Path).Path
    $root = (Resolve-Path -LiteralPath $AllowedRoot).Path.TrimEnd('\', '/') + [System.IO.Path]::DirectorySeparatorChar
    if (-not $target.StartsWith($root, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing to delete outside allowed root. Target=$target Root=$root"
    }

    Remove-Item -LiteralPath $target -Recurse -Force
}

function Ensure-Neo4jDocker(
    [string]$RepoRoot,
    [string]$ContainerName,
    [string]$Image,
    [string]$Neo4jHostName,
    [int]$HttpPort,
    [int]$BoltPort,
    [string]$User,
    [string]$Password,
    [string]$DataRoot,
    [bool]$ResetData
) {
    if ($Neo4jHostName -notin @("127.0.0.1", "localhost", "0.0.0.0")) {
        Write-Host "[INFO] Neo4j host is not local ($Neo4jHostName); Docker container management skipped."
        return
    }

    $docker = Get-Command docker -ErrorAction SilentlyContinue
    if ($null -eq $docker) {
        throw "Docker is not available. Install Docker Desktop or pass -SkipNeo4jDocker."
    }

    $dataRootFull = Ensure-Directory $DataRoot
    $confDir = Join-Path $dataRootFull "conf"
    $dataDir = Join-Path $dataRootFull "data"
    $pluginsDir = Join-Path $dataRootFull "plugins"

    if ($ResetData) {
        Write-Host "[INFO] Resetting local Neo4j Docker data under $dataRootFull"
        Invoke-Docker -Arguments @("rm", "-f", $ContainerName) -AllowFailure | Out-Null
        Remove-DirectorySafe -Path $dataRootFull -AllowedRoot $RepoRoot
    }

    Ensure-Directory $confDir | Out-Null
    Ensure-Directory $dataDir | Out-Null
    Ensure-Directory $pluginsDir | Out-Null

    $existingId = (Invoke-Docker -Arguments @("ps", "-a", "--filter", "name=^/$ContainerName$", "--format", "{{.ID}}") -AllowFailure)
    if (-not [string]::IsNullOrWhiteSpace([string]$existingId)) {
        $runningId = (Invoke-Docker -Arguments @("ps", "--filter", "name=^/$ContainerName$", "--format", "{{.ID}}") -AllowFailure)
        if ([string]::IsNullOrWhiteSpace([string]$runningId)) {
            Write-Host "[INFO] Starting existing Neo4j Docker container: $ContainerName"
            Invoke-Docker -Arguments @("start", $ContainerName) | Out-Null
        } else {
            Write-Host "[INFO] Neo4j Docker container already running: $ContainerName"
        }
        return
    }

    Write-Host "[INFO] Creating Neo4j Docker container: $ContainerName"
    $oldNeo4jAuth = [Environment]::GetEnvironmentVariable("NEO4J_AUTH", "Process")
    $oldNeo4jPlugins = [Environment]::GetEnvironmentVariable("NEO4J_PLUGINS", "Process")
    try {
        [Environment]::SetEnvironmentVariable("NEO4J_AUTH", "$User/$Password", "Process")
        [Environment]::SetEnvironmentVariable("NEO4J_PLUGINS", '["apoc"]', "Process")
        Invoke-Docker -Arguments @(
            "run", "-d",
            "--name", $ContainerName,
            "-p", "$HttpPort`:7474",
            "-p", "$BoltPort`:7687",
            "-v", "$confDir`:/conf",
            "-v", "$dataDir`:/data",
            "-v", "$pluginsDir`:/plugins",
            "-e", "NEO4J_AUTH",
            "-e", "NEO4J_PLUGINS",
            "-e", "NEO4J_apoc_export_file_enabled=true",
            "-e", "NEO4J_apoc_import_file_enabled=true",
            "-e", "NEO4J_apoc_import_file_use__neo4j__config=true",
            $Image
        ) | Out-Null
    } finally {
        [Environment]::SetEnvironmentVariable("NEO4J_AUTH", $oldNeo4jAuth, "Process")
        [Environment]::SetEnvironmentVariable("NEO4J_PLUGINS", $oldNeo4jPlugins, "Process")
    }
}

function Wait-Neo4jApoc(
    [string]$ContainerName,
    [string]$User,
    [string]$Password,
    [int]$Attempts,
    [int]$DelaySeconds
) {
    $lastOutput = ""
    for ($i = 1; $i -le $Attempts; $i++) {
        $output = Invoke-Docker -Arguments @(
            "exec",
            $ContainerName,
            "cypher-shell",
            "-u", $User,
            "-p", $Password,
            "RETURN apoc.version() AS apoc;"
        ) -AllowFailure
        $lastOutput = ($output | Out-String).Trim()
        if ($lastOutput -match "\d+\.\d+\.\d+") {
            return
        }
        if ($i -lt $Attempts) {
            Start-Sleep -Seconds $DelaySeconds
        }
    }

    throw "Neo4j APOC is not available in Docker container '$ContainerName'. Recreate local Neo4j with -ResetNeo4jDockerData. Last check output: $lastOutput"
}
