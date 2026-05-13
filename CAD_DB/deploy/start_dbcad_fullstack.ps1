param(
    [string]$FastApiHost = "0.0.0.0",
    [int]$FastApiPort = 8000,
    [string]$BridgeHost = "127.0.0.1",
    [int]$BridgePort = 8100,
    [double]$StorageBridgeTimeoutSeconds = 15,
    [string]$Neo4jUri = "",
    [string]$Neo4jHost = "",
    [int]$Neo4jPort = 0,
    [string]$Neo4jUser = "",
    [string]$Neo4jPassword = "",
    [string]$ApiPassword = "",
    [string]$Author = "dbcad-exe",
    [string]$ClientExePath = "",
    [string]$PackageRoot = "",
    [string]$ConfigPath = "",
    [string]$WinDeployQtPath = "",
    [int]$Neo4jHttpPort = 7474,
    [string]$Neo4jDockerName = "",
    [string]$Neo4jDockerImage = "",
    [string]$Neo4jDockerDataRoot = "",
    [switch]$SkipClient = $false,
    [switch]$SkipPackageSync = $false,
    [switch]$PrepareOnly = $false,
    [switch]$SkipDependencySync = $false,
    [switch]$SkipBridgeHealthCheck = $false,
    [switch]$SkipNeo4jPortCheck = $false,
    [switch]$SkipNeo4jDocker = $false,
    [switch]$ResetNeo4jDockerData = $false,
    [switch]$SkipWinDeployQt = $false,
    [switch]$KeepExistingServices = $false,
    [int]$RetryCount = 30,
    [int]$RetryDelaySeconds = 1
)

$ErrorActionPreference = "Stop"

function Resolve-FullPath([string]$Path) {
    if ([string]::IsNullOrWhiteSpace($Path)) {
        return $Path
    }

    $item = Get-Item -LiteralPath $Path -ErrorAction SilentlyContinue
    if ($null -ne $item) {
        return $item.FullName
    }

    $parent = Split-Path -Parent $Path
    if ([string]::IsNullOrWhiteSpace($parent)) {
        $parent = "."
    }
    $leaf = Split-Path -Leaf $Path
    $resolvedParent = (Resolve-Path $parent).Path
    return (Join-Path $resolvedParent $leaf)
}

function Ensure-Directory([string]$Path) {
    if (!(Test-Path -LiteralPath $Path)) {
        New-Item -ItemType Directory -Path $Path -Force | Out-Null
    }
    return (Resolve-FullPath $Path)
}

function Copy-DirectoryContents([string]$Source, [string]$Destination) {
    if (!(Test-Path -LiteralPath $Source)) {
        return
    }

    $sourceFull = Resolve-FullPath $Source
    $destinationFull = Ensure-Directory $Destination
    if ($sourceFull.TrimEnd('\') -ieq $destinationFull.TrimEnd('\')) {
        return
    }

    Get-ChildItem -LiteralPath $sourceFull -Force | ForEach-Object {
        Copy-Item -LiteralPath $_.FullName -Destination $destinationFull -Recurse -Force
    }
}

function Copy-FileIfExists([string]$Source, [string]$DestinationDir) {
    if (Test-Path -LiteralPath $Source) {
        Ensure-Directory $DestinationDir | Out-Null
        $destination = Join-Path $DestinationDir (Split-Path -Leaf $Source)
        if ((Resolve-FullPath $Source).TrimEnd('\') -ieq (Resolve-FullPath $destination).TrimEnd('\')) {
            return
        }
        Copy-Item -LiteralPath $Source -Destination $destination -Force
    }
}

function Copy-FileToPathIfDifferent([string]$Source, [string]$Destination) {
    if (!(Test-Path -LiteralPath $Source)) {
        return
    }

    $parent = Split-Path -Parent $Destination
    Ensure-Directory $parent | Out-Null
    if ((Resolve-FullPath $Source).TrimEnd('\') -ieq (Resolve-FullPath $Destination).TrimEnd('\')) {
        return
    }

    Copy-Item -LiteralPath $Source -Destination $Destination -Force
}

function Read-EnvFile([string]$EnvFilePath) {
    $values = @{}
    if (!(Test-Path -LiteralPath $EnvFilePath)) {
        return $values
    }

    Get-Content -LiteralPath $EnvFilePath | ForEach-Object {
        $line = $_.Trim()
        if ([string]::IsNullOrWhiteSpace($line) -or $line.StartsWith("#")) {
            return
        }

        $pair = $line.Split("=", 2)
        if ($pair.Count -ne 2) {
            return
        }

        $name = $pair[0].Trim().TrimStart([char]0xFEFF)
        $value = $pair[1].Trim()
        if (-not [string]::IsNullOrWhiteSpace($name)) {
            $values[$name] = $value
        }
    }

    return $values
}

function Merge-EnvFiles([string[]]$Paths) {
    $merged = @{}
    foreach ($path in $Paths) {
        $values = Read-EnvFile $path
        foreach ($key in $values.Keys) {
            $merged[$key] = $values[$key]
        }
    }
    return $merged
}

function Write-EnvFile([string]$EnvFilePath, [hashtable]$Values) {
    $orderedKeys = @(
        "DBCAD_PASSWORD",
        "DBCAD_NEO4J_DOCKER_NAME",
        "DBCAD_NEO4J_DOCKER_IMAGE",
        "DBCAD_NEO4J_HOST",
        "DBCAD_NEO4J_BOLT_PORT",
        "DBCAD_NEO4J_HTTP_PORT",
        "DBCAD_NEO4J_USER",
        "DBCAD_FASTAPI_HOST",
        "DBCAD_FASTAPI_PORT",
        "DBCAD_BRIDGE_HOST",
        "DBCAD_BRIDGE_PORT",
        "DBCAD_AUTHOR"
    )

    $lines = @(
        "# Local DBCAD deployment configuration.",
        "# This is the single source for Docker, Bridge, FastAPI and client credentials.",
        "# Do not commit this file."
    )
    foreach ($key in $orderedKeys) {
        if ($Values.ContainsKey($key)) {
            $lines += "$key=$($Values[$key])"
        }
    }

    $parent = Split-Path -Parent $EnvFilePath
    Ensure-Directory $parent | Out-Null
    Set-Content -LiteralPath $EnvFilePath -Value $lines -Encoding UTF8
}

function New-StrongPassword([int]$Length = 32) {
    $alphabet = "ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz23456789_-"
    $rng = [System.Security.Cryptography.RandomNumberGenerator]::Create()
    try {
        $bytes = New-Object byte[] ($Length)
        $rng.GetBytes($bytes)
        $chars = @()
        for ($i = 0; $i -lt $Length; $i++) {
            $chars += $alphabet[$bytes[$i] % $alphabet.Length]
        }
        return (-join $chars)
    } finally {
        $rng.Dispose()
    }
}

function Test-WeakDeploymentPassword([string]$Password) {
    if ([string]::IsNullOrWhiteSpace($Password)) {
        return $true
    }

    $value = $Password.Trim()
    if ($value.Length -lt 16) {
        return $true
    }

    if ($value -match "^\d+$") {
        return $true
    }

    $weakValues = @("12345678", "password", "neo4j", "changeme", "your_password", "dbcad")
    return ($weakValues -contains $value.ToLowerInvariant())
}

function New-DefaultLocalConfig([string]$Password) {
    return @{
        DBCAD_PASSWORD = $Password
        DBCAD_NEO4J_DOCKER_NAME = "neo4j-apoc"
        DBCAD_NEO4J_DOCKER_IMAGE = "neo4j:5.15"
        DBCAD_NEO4J_HOST = "127.0.0.1"
        DBCAD_NEO4J_BOLT_PORT = "7687"
        DBCAD_NEO4J_HTTP_PORT = "7474"
        DBCAD_NEO4J_USER = "neo4j"
        DBCAD_FASTAPI_HOST = "0.0.0.0"
        DBCAD_FASTAPI_PORT = "8000"
        DBCAD_BRIDGE_HOST = "127.0.0.1"
        DBCAD_BRIDGE_PORT = "8100"
        DBCAD_AUTHOR = "dbcad-exe"
    }
}

function Ensure-LocalConfig([string]$ConfigFilePath) {
    if (Test-Path -LiteralPath $ConfigFilePath) {
        return Read-EnvFile $ConfigFilePath
    }

    $defaults = New-DefaultLocalConfig -Password (New-StrongPassword)
    Write-EnvFile -EnvFilePath $ConfigFilePath -Values $defaults
    Write-Host "[INFO] Created local config: $ConfigFilePath"
    Write-Host "[INFO] Generated one strong DBCAD_PASSWORD for Neo4j, FastAPI and client access."
    return $defaults
}

function Ensure-UnifiedPasswordConfig([string]$ConfigFilePath, [hashtable]$Config) {
    $password = Get-ConfigValue -Config $Config -Name "DBCAD_PASSWORD" -DefaultValue ""
    $legacyNeo4jPassword = Get-ConfigValue -Config $Config -Name "DBCAD_NEO4J_PASSWORD" -DefaultValue ""
    $legacyApiPassword = Get-ConfigValue -Config $Config -Name "DBCAD_API_PASSWORD" -DefaultValue ""
    $rewrite = $false

    if ([string]::IsNullOrWhiteSpace($password)) {
        if (-not (Test-WeakDeploymentPassword $legacyNeo4jPassword)) {
            $password = $legacyNeo4jPassword
        } elseif (-not (Test-WeakDeploymentPassword $legacyApiPassword)) {
            $password = $legacyApiPassword
        }
        $rewrite = $true
    }

    if (Test-WeakDeploymentPassword $password) {
        $password = New-StrongPassword
        $rewrite = $true
        Write-Warning "The local deployment password was missing or weak. A new strong DBCAD_PASSWORD was generated in $ConfigFilePath. Recreate the local Neo4j Docker data once with -ResetNeo4jDockerData."
    }

    if ($Config.ContainsKey("DBCAD_NEO4J_PASSWORD") -or $Config.ContainsKey("DBCAD_API_PASSWORD")) {
        $rewrite = $true
    }

    $normalized = New-DefaultLocalConfig -Password $password
    foreach ($key in $Config.Keys) {
        if ($key -in @("DBCAD_NEO4J_PASSWORD", "DBCAD_API_PASSWORD")) {
            continue
        }
        if ($normalized.ContainsKey($key)) {
            $normalized[$key] = $Config[$key]
        }
    }
    $normalized["DBCAD_PASSWORD"] = $password

    if ($rewrite) {
        Write-EnvFile -EnvFilePath $ConfigFilePath -Values $normalized
        Write-Host "[INFO] Normalized local config to one password source: $ConfigFilePath"
    }

    return $normalized
}

function Get-ConfigValue([hashtable]$Config, [string]$Name, [string]$DefaultValue = "") {
    if ($Config.ContainsKey($Name) -and -not [string]::IsNullOrWhiteSpace([string]$Config[$Name])) {
        return [string]$Config[$Name]
    }
    return $DefaultValue
}

function Get-ConfigInt([hashtable]$Config, [string]$Name, [int]$DefaultValue) {
    $value = Get-ConfigValue -Config $Config -Name $Name -DefaultValue ""
    $parsed = 0
    if ([int]::TryParse($value, [ref]$parsed) -and $parsed -gt 0) {
        return $parsed
    }
    return $DefaultValue
}

function ConvertTo-LocalHealthHost([string]$HostAddress) {
    if ([string]::IsNullOrWhiteSpace($HostAddress)) {
        return "127.0.0.1"
    }

    $normalized = $HostAddress.Trim().ToLowerInvariant()
    if ($normalized -in @("0.0.0.0", "::", "[::]", "localhost")) {
        return "127.0.0.1"
    }

    return $HostAddress.Trim()
}

function Get-WinDeployQtPath([string]$ExplicitPath) {
    if (-not [string]::IsNullOrWhiteSpace($ExplicitPath)) {
        if (Test-Path -LiteralPath $ExplicitPath) {
            return (Resolve-FullPath $ExplicitPath)
        }
        throw "windeployqt not found: $ExplicitPath"
    }

    $envPath = [Environment]::GetEnvironmentVariable("WINDEPLOYQT_PATH")
    if (-not [string]::IsNullOrWhiteSpace($envPath) -and (Test-Path -LiteralPath $envPath)) {
        return (Resolve-FullPath $envPath)
    }

    $cmd = Get-Command windeployqt -ErrorAction SilentlyContinue
    if ($cmd) {
        return $cmd.Source
    }

    $qmake = Get-Command qmake -ErrorAction SilentlyContinue
    if ($qmake) {
        $qtBin = Split-Path -Parent $qmake.Source
        $candidate = Join-Path $qtBin "windeployqt.exe"
        if (Test-Path -LiteralPath $candidate) {
            return (Resolve-FullPath $candidate)
        }
    }

    $candidatePaths = @(
        "D:\Qt\6.9.0\msvc2022_64\bin\windeployqt.exe",
        "C:\Qt\6.9.0\msvc2022_64\bin\windeployqt.exe"
    )

    foreach ($qtRoot in @("D:\Qt", "C:\Qt")) {
        if (!(Test-Path -LiteralPath $qtRoot)) {
            continue
        }

        $versions = @(Get-ChildItem -LiteralPath $qtRoot -Directory -ErrorAction SilentlyContinue | Sort-Object Name -Descending)
        foreach ($version in $versions) {
            foreach ($kit in @("msvc2022_64", "msvc2019_64", "msvc2022_arm64", "mingw_64")) {
                $candidatePaths += (Join-Path $version.FullName "$kit\bin\windeployqt.exe")
            }
        }
    }

    foreach ($candidate in $candidatePaths) {
        if (Test-Path -LiteralPath $candidate) {
            return (Resolve-FullPath $candidate)
        }
    }

    return ""
}

function Test-DebugBuild([System.IO.FileInfo]$Exe) {
    if ($Exe.DirectoryName -match "\\Debug($|\\)") {
        return $true
    }

    if (Test-Path -LiteralPath (Join-Path $Exe.DirectoryName "Qt6Cored.dll")) {
        return $true
    }

    return $false
}

function Invoke-WinDeployQtIfAvailable([string]$ExePath, [bool]$IsDebug) {
    if ($SkipWinDeployQt) {
        return
    }

    $windeployqt = Get-WinDeployQtPath -ExplicitPath $WinDeployQtPath
    if ([string]::IsNullOrWhiteSpace($windeployqt)) {
        Write-Warning "windeployqt was not found. Runtime DLLs will be copied from existing output folders only. Pass -WinDeployQtPath if Qt is installed in a custom location."
        return
    }

    $mode = if ($IsDebug) { "--debug" } else { "--release" }
    Write-Host "[INFO] Running windeployqt for runtime dependencies: $windeployqt $mode"
    & $windeployqt $mode --compiler-runtime $ExePath
}

function Resolve-LatestCadDbExe(
    [string]$ProjectRoot,
    [string]$RepoRoot,
    [string]$PackageRoot,
    [string]$ExplicitPath
) {
    if (-not [string]::IsNullOrWhiteSpace($ExplicitPath)) {
        if (Test-Path -LiteralPath $ExplicitPath) {
            return Get-Item -LiteralPath $ExplicitPath
        }
        throw "CAD_DB.exe not found: $ExplicitPath"
    }

    $candidateSpecs = @(
        @{ Path = (Join-Path $ProjectRoot "x64\Release\CAD_DB.exe"); Rank = 0 },
        @{ Path = (Join-Path $ProjectRoot "x64\Debug\CAD_DB.exe"); Rank = 0 },
        @{ Path = (Join-Path $RepoRoot "x64\Release\CAD_DB.exe"); Rank = 0 },
        @{ Path = (Join-Path $RepoRoot "x64\Debug\CAD_DB.exe"); Rank = 0 },
        @{ Path = (Join-Path $RepoRoot "dist\client-Release\CAD_DB.exe"); Rank = 1 },
        @{ Path = (Join-Path $RepoRoot "dist\client\CAD_DB.exe"); Rank = 1 },
        @{ Path = (Join-Path $RepoRoot "dist\DBCAD-fullstack-package\client\CAD_DB.exe"); Rank = 2 },
        @{ Path = (Join-Path $RepoRoot "dist\DBCAD-fullstack-package\server\bridge-bin\CAD_DB.exe"); Rank = 2 },
        @{ Path = (Join-Path $PackageRoot "client\CAD_DB.exe"); Rank = 2 },
        @{ Path = (Join-Path $PackageRoot "server\bridge-bin\CAD_DB.exe"); Rank = 2 }
    )

    $candidates = @()
    foreach ($spec in $candidateSpecs) {
        if (Test-Path -LiteralPath $spec.Path) {
            $item = Get-Item -LiteralPath $spec.Path
            $candidates += [pscustomobject]@{
                Item = $item
                Rank = $spec.Rank
                LastWriteTime = $item.LastWriteTime
            }
        }
    }

    if ($candidates.Count -eq 0) {
        throw "Cannot locate CAD_DB.exe. Build the project first or pass -ClientExePath."
    }

    return (($candidates | Sort-Object @{ Expression = "Rank"; Descending = $false }, @{ Expression = "LastWriteTime"; Descending = $true } | Select-Object -First 1).Item)
}

function Split-Neo4jUri([string]$UriText) {
    $result = @{
        Host = "127.0.0.1"
        Port = 7687
    }

    if ([string]::IsNullOrWhiteSpace($UriText)) {
        return $result
    }

    try {
        $uri = [System.Uri]$UriText
        if (-not [string]::IsNullOrWhiteSpace($uri.Host)) {
            $result.Host = $uri.Host
        }
        if ($uri.Port -gt 0) {
            $result.Port = $uri.Port
        }
    } catch {
    }

    return $result
}

function Test-ServiceHealth([string]$Url) {
    try {
        $response = Invoke-RestMethod -Uri $Url -Method Get -TimeoutSec 3
        return $response.status -eq "ok"
    } catch {
        return $false
    }
}

function Test-TcpPort([string]$HostName, [int]$Port) {
    try {
        $client = New-Object System.Net.Sockets.TcpClient
        $async = $client.BeginConnect($HostName, $Port, $null, $null)
        $connected = $async.AsyncWaitHandle.WaitOne(3000, $false)
        if ($connected) {
            $client.EndConnect($async)
        }
        $client.Close()
        return $connected
    } catch {
        return $false
    }
}

function Wait-TcpPort([string]$HostName, [int]$Port, [int]$Attempts, [int]$DelaySeconds) {
    for ($i = 1; $i -le $Attempts; $i++) {
        if (Test-TcpPort -HostName $HostName -Port $Port) {
            return $true
        }
        if ($i -lt $Attempts) {
            Start-Sleep -Seconds $DelaySeconds
        }
    }
    return $false
}

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
    $root = (Resolve-Path -LiteralPath $AllowedRoot).Path
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

function Wait-ServiceHealth(
    [string]$Url,
    [object]$Process,
    [string]$ServiceName,
    [int]$Attempts,
    [int]$DelaySeconds
) {
    for ($i = 1; $i -le $Attempts; $i++) {
        if ($null -ne $Process -and $Process.HasExited) {
            Write-Warning "$ServiceName exited before health check passed. ExitCode=$($Process.ExitCode)"
            return $false
        }

        if (Test-ServiceHealth $Url) {
            return $true
        }

        if ($i -lt $Attempts) {
            Start-Sleep -Seconds $DelaySeconds
        }
    }

    return $false
}

function Stop-StartedProcess([object]$Process, [string]$Name) {
    if ($null -eq $Process) {
        return
    }

    try {
        if (-not $Process.HasExited) {
            Write-Warning "Stopping $Name after startup failure (PID $($Process.Id))."
            Stop-Process -Id $Process.Id -Force -ErrorAction SilentlyContinue
        }
    } catch {
    }
}

function Write-TextFile([string]$Path, [string]$Text) {
    $parent = Split-Path -Parent $Path
    Ensure-Directory $parent | Out-Null
    Set-Content -LiteralPath $Path -Value $Text -Encoding UTF8
}

function Write-PidFile([string]$Path, [object]$Data) {
    $json = $Data | ConvertTo-Json -Depth 5
    Write-TextFile -Path $Path -Text $json
}

function Ensure-Uv {
    $uvCmd = Get-Command uv -ErrorAction SilentlyContinue
    if ($null -eq $uvCmd) {
        Write-Host "[INFO] uv is not installed. Installing uv..."
        powershell -NoProfile -ExecutionPolicy Bypass -Command "irm https://astral.sh/uv/install.ps1 | iex"
        $env:Path = "$env:USERPROFILE\.local\bin;$env:USERPROFILE\.cargo\bin;$env:Path"
    }

    $uvCmd = Get-Command uv -ErrorAction SilentlyContinue
    if ($null -eq $uvCmd) {
        throw "uv is unavailable. Install uv or add it to PATH."
    }

    return $uvCmd.Source
}

function Start-StorageBridge(
    [string]$ExePath,
    [string]$WorkingDirectory,
    [string]$BridgeHost,
    [int]$BridgePort,
    [string]$Neo4jHost,
    [int]$Neo4jPort,
    [string]$Neo4jUser,
    [string]$Neo4jPassword,
    [string]$LogDir
) {
    $args = @(
        "--storage-bridge",
        "--bridge-host", $BridgeHost,
        "--bridge-port", "$BridgePort",
        "--neo4j-host", $Neo4jHost,
        "--neo4j-port", "$Neo4jPort",
        "--neo4j-user", $Neo4jUser
    )

    if (-not [string]::IsNullOrWhiteSpace($Neo4jPassword)) {
        $args += @("--neo4j-password", $Neo4jPassword)
    }

    $stdout = Join-Path $LogDir "bridge.out.log"
    $stderr = Join-Path $LogDir "bridge.err.log"
    return Start-Process -FilePath $ExePath -ArgumentList $args -WorkingDirectory $WorkingDirectory -WindowStyle Hidden -PassThru -RedirectStandardOutput $stdout -RedirectStandardError $stderr
}

function Start-FastApi(
    [string]$ServerDir,
    [string]$HostAddress,
    [int]$Port,
    [string]$BridgeUrl,
    [double]$BridgeTimeoutSeconds,
    [string]$ApiPassword,
    [bool]$SkipDependencySync,
    [bool]$SkipBridgeHealthCheck,
    [string]$LogDir
) {
    if (!(Test-Path -LiteralPath (Join-Path $ServerDir "app\main.py")) -or !(Test-Path -LiteralPath (Join-Path $ServerDir "pyproject.toml"))) {
        throw "Backend package is incomplete. Expected app/main.py and pyproject.toml in $ServerDir"
    }

    if (-not $SkipBridgeHealthCheck -and -not (Test-ServiceHealth "$BridgeUrl/health")) {
        throw "Storage bridge health check failed before starting FastAPI: $BridgeUrl/health"
    }

    $uv = Ensure-Uv

    if (-not $SkipDependencySync) {
        Write-Host "[INFO] Syncing FastAPI dependencies with uv..."
        Push-Location $ServerDir
        try {
            & $uv sync
        } finally {
            Pop-Location
        }
    } else {
        Write-Host "[INFO] Skip dependency sync (-SkipDependencySync)."
    }

    [Environment]::SetEnvironmentVariable("CAD_DB_STORAGE_BRIDGE_URL", $BridgeUrl, "Process")
    [Environment]::SetEnvironmentVariable("CAD_DB_STORAGE_BRIDGE_TIMEOUT_SECONDS", [string]$BridgeTimeoutSeconds, "Process")
    [Environment]::SetEnvironmentVariable("CAD_DB_API_PASSWORD", $ApiPassword, "Process")

    $args = @(
        "run",
        "uvicorn",
        "app.main:app",
        "--host", $HostAddress,
        "--port", "$Port"
    )

    $stdout = Join-Path $LogDir "fastapi.out.log"
    $stderr = Join-Path $LogDir "fastapi.err.log"
    return Start-Process -FilePath $uv -ArgumentList $args -WorkingDirectory $ServerDir -WindowStyle Hidden -PassThru -RedirectStandardOutput $stdout -RedirectStandardError $stderr
}

function Write-PackageReadme([string]$PackageRoot, [string]$LatestExePath) {
    $readme = @"
DBCAD fullstack package

Start all services and the client:
  .\start_dbcad_fullstack.cmd

Stop services started by the launcher:
  .\stop_dbcad_fullstack.cmd

Prepare package without starting services:
  .\start_dbcad_fullstack.cmd -PrepareOnly

Generated from:
  $LatestExePath
"@
    Write-TextFile -Path (Join-Path $PackageRoot "README_START_HERE.txt") -Text $readme
}

function Remove-ObsoletePackageScripts([string]$ServerDir) {
    $obsolete = @(
        "check_backend.ps1",
        "deploy_backend_uv.ps1",
        "run_server.ps1",
        "set_neo4j_password.ps1",
        "start_backend_oneclick.cmd",
        "start_backend_oneclick.ps1",
        "start_fullstack_oneclick.cmd",
        "start_fullstack_oneclick.ps1",
        "STARTUP_CONFIG.md"
    )

    foreach ($name in $obsolete) {
        $path = Join-Path $ServerDir $name
        if (Test-Path -LiteralPath $path) {
            Remove-Item -LiteralPath $path -Force
        }
    }
}

function Remove-LegacyCredentialFiles([string[]]$Directories) {
    $names = @(
        "neo4j_connect_info.conf",
        "neo4j_connect_info.config",
        "postgresql_connect_info.conf"
    )
    foreach ($dir in $Directories) {
        foreach ($name in $names) {
            $path = Join-Path $dir $name
            if (Test-Path -LiteralPath $path) {
                Remove-Item -LiteralPath $path -Force
            }
        }
    }
}

function Sync-FullstackPackage(
    [string]$ProjectRoot,
    [string]$RepoRoot,
    [string]$DeployRoot,
    [string]$ConfigPath,
    [string]$BackendRoot,
    [string]$PackageRoot,
    [System.IO.FileInfo]$LatestExe,
    [string]$FastApiUrl,
    [string]$Author,
    [string]$ApiPassword,
    [string]$BridgeHost,
    [int]$BridgePort,
    [string]$Neo4jHost,
    [int]$Neo4jPort,
    [string]$Neo4jUser,
    [string]$Neo4jPassword,
    [string]$Neo4jUri,
    [double]$StorageBridgeTimeoutSeconds
) {
    $clientDir = Ensure-Directory (Join-Path $PackageRoot "client")
    $serverDir = Ensure-Directory (Join-Path $PackageRoot "server")
    $bridgeDir = Ensure-Directory (Join-Path $serverDir "bridge-bin")

    $runtimeDirs = @(
        (Join-Path $RepoRoot "dist\client-Release"),
        (Join-Path $RepoRoot "dist\client"),
        $LatestExe.DirectoryName
    )
    foreach ($runtimeDir in $runtimeDirs) {
        Copy-DirectoryContents -Source $runtimeDir -Destination $clientDir
    }

    Remove-LegacyCredentialFiles -Directories @($clientDir)
    Copy-FileToPathIfDifferent -Source $LatestExe.FullName -Destination (Join-Path $clientDir "CAD_DB.exe")
    Invoke-WinDeployQtIfAvailable -ExePath (Join-Path $clientDir "CAD_DB.exe") -IsDebug (Test-DebugBuild $LatestExe)

    $fastApiConfig = "$FastApiUrl`r`n$Author`r`n$ApiPassword`r`n"
    Write-TextFile -Path (Join-Path $clientDir "fastapi_connect_info.conf") -Text $fastApiConfig

    Write-TextFile -Path (Join-Path $clientDir "storage_bridge_connect_info.conf") -Text "$BridgeHost`r`n$BridgePort`r`n"

    Copy-DirectoryContents -Source $clientDir -Destination $bridgeDir
    Copy-FileToPathIfDifferent -Source (Join-Path $clientDir "CAD_DB.exe") -Destination (Join-Path $bridgeDir "CAD_DB.exe")
    Remove-LegacyCredentialFiles -Directories @($bridgeDir)

    $backendSource = Resolve-FullPath $BackendRoot
    $serverTarget = Resolve-FullPath $serverDir
    $serverApp = Join-Path $serverDir "app"
    if ($backendSource.TrimEnd('\') -ine $serverTarget.TrimEnd('\')) {
        if (Test-Path -LiteralPath $serverApp) {
            Remove-Item -LiteralPath $serverApp -Recurse -Force
        }
        Copy-Item -LiteralPath (Join-Path $BackendRoot "app") -Destination $serverDir -Recurse -Force
    }

    foreach ($file in @("pyproject.toml", "uv.lock", "requirements.txt", "README.md", ".env.example", "pytest.ini")) {
        Copy-FileIfExists -Source (Join-Path $BackendRoot $file) -DestinationDir $serverDir
    }

    foreach ($file in @(
        "start_dbcad_fullstack.ps1",
        "start_dbcad_fullstack.cmd",
        "stop_dbcad_fullstack.ps1",
        "stop_dbcad_fullstack.cmd"
    )) {
        Copy-FileIfExists -Source (Join-Path $DeployRoot $file) -DestinationDir $PackageRoot
    }
    Copy-FileToPathIfDifferent -Source $ConfigPath -Destination (Join-Path $PackageRoot "dbcad.local.env")

    $envFile = Join-Path $serverDir ".env"
    $envText = @"
CAD_DB_STORAGE_BACKEND=neo4j
CAD_DB_STORAGE_BRIDGE_URL=http://127.0.0.1:$BridgePort
CAD_DB_STORAGE_BRIDGE_TIMEOUT_SECONDS=$StorageBridgeTimeoutSeconds
CAD_DB_NEO4J_URI=$Neo4jUri
CAD_DB_NEO4J_USER=$Neo4jUser
CAD_DB_NEO4J_DATABASE=neo4j
CAD_DB_API_PASSWORD=$ApiPassword
"@
    Write-TextFile -Path $envFile -Text $envText

    Remove-ObsoletePackageScripts -ServerDir $serverDir
    Write-PackageReadme -PackageRoot $PackageRoot -LatestExePath $LatestExe.FullName

    return @{
        ClientDir = $clientDir
        ServerDir = $serverDir
        BridgeDir = $bridgeDir
    }
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$scriptRoot = Resolve-FullPath $scriptRoot
$sourceProjectRoot = Resolve-FullPath (Join-Path $scriptRoot "..")
$sourceBackendRoot = Join-Path $sourceProjectRoot "backend"
$packageServerRoot = Join-Path $scriptRoot "server"

if (Test-Path -LiteralPath (Join-Path $sourceBackendRoot "app\main.py")) {
    $deployRoot = $scriptRoot
    $projectRoot = $sourceProjectRoot
    $repoRoot = Resolve-FullPath (Join-Path $projectRoot "..")
    $backendRoot = $sourceBackendRoot
    if ([string]::IsNullOrWhiteSpace($PackageRoot)) {
        $PackageRoot = Join-Path $repoRoot "dist\DBCAD-fullstack-package"
    }
} elseif (Test-Path -LiteralPath (Join-Path $packageServerRoot "app\main.py")) {
    $deployRoot = $scriptRoot
    $PackageRoot = if ([string]::IsNullOrWhiteSpace($PackageRoot)) { $scriptRoot } else { $PackageRoot }
    $repoRoot = Resolve-FullPath (Join-Path $scriptRoot "..\..")
    $candidateProjectRoot = Join-Path $repoRoot "CAD_DB"
    $projectRoot = if (Test-Path -LiteralPath $candidateProjectRoot) { Resolve-FullPath $candidateProjectRoot } else { $scriptRoot }
    $backendRoot = $packageServerRoot
} else {
    throw "Cannot locate backend. Run this script from CAD_DB\deploy or DBCAD-fullstack-package."
}

if ([string]::IsNullOrWhiteSpace($ConfigPath)) {
    $ConfigPath = Join-Path $deployRoot "dbcad.local.env"
}
$ConfigPath = Resolve-FullPath $ConfigPath
$localConfig = Ensure-LocalConfig -ConfigFilePath $ConfigPath
$localConfig = Ensure-UnifiedPasswordConfig -ConfigFilePath $ConfigPath -Config $localConfig
$UnifiedPassword = Get-ConfigValue -Config $localConfig -Name "DBCAD_PASSWORD" -DefaultValue ""

if (-not $PSBoundParameters.ContainsKey("FastApiHost")) {
    $FastApiHost = Get-ConfigValue -Config $localConfig -Name "DBCAD_FASTAPI_HOST" -DefaultValue $FastApiHost
}
if (-not $PSBoundParameters.ContainsKey("FastApiPort")) {
    $FastApiPort = Get-ConfigInt -Config $localConfig -Name "DBCAD_FASTAPI_PORT" -DefaultValue $FastApiPort
}
if (-not $PSBoundParameters.ContainsKey("BridgeHost")) {
    $BridgeHost = Get-ConfigValue -Config $localConfig -Name "DBCAD_BRIDGE_HOST" -DefaultValue $BridgeHost
}
if (-not $PSBoundParameters.ContainsKey("BridgePort")) {
    $BridgePort = Get-ConfigInt -Config $localConfig -Name "DBCAD_BRIDGE_PORT" -DefaultValue $BridgePort
}
if (-not $PSBoundParameters.ContainsKey("Neo4jHost")) {
    $Neo4jHost = Get-ConfigValue -Config $localConfig -Name "DBCAD_NEO4J_HOST" -DefaultValue "127.0.0.1"
}
if (-not $PSBoundParameters.ContainsKey("Neo4jPort")) {
    $Neo4jPort = Get-ConfigInt -Config $localConfig -Name "DBCAD_NEO4J_BOLT_PORT" -DefaultValue 7687
}
if (-not $PSBoundParameters.ContainsKey("Neo4jHttpPort")) {
    $Neo4jHttpPort = Get-ConfigInt -Config $localConfig -Name "DBCAD_NEO4J_HTTP_PORT" -DefaultValue $Neo4jHttpPort
}
if (-not $PSBoundParameters.ContainsKey("Neo4jUser")) {
    $Neo4jUser = Get-ConfigValue -Config $localConfig -Name "DBCAD_NEO4J_USER" -DefaultValue "neo4j"
}
if (-not $PSBoundParameters.ContainsKey("Neo4jPassword")) {
    $Neo4jPassword = $UnifiedPassword
}
if (-not $PSBoundParameters.ContainsKey("ApiPassword")) {
    $ApiPassword = $UnifiedPassword
}
if (-not $PSBoundParameters.ContainsKey("Author")) {
    $Author = Get-ConfigValue -Config $localConfig -Name "DBCAD_AUTHOR" -DefaultValue $Author
}
if (-not $PSBoundParameters.ContainsKey("Neo4jDockerName")) {
    $Neo4jDockerName = Get-ConfigValue -Config $localConfig -Name "DBCAD_NEO4J_DOCKER_NAME" -DefaultValue "neo4j-apoc"
}
if (-not $PSBoundParameters.ContainsKey("Neo4jDockerImage")) {
    $Neo4jDockerImage = Get-ConfigValue -Config $localConfig -Name "DBCAD_NEO4J_DOCKER_IMAGE" -DefaultValue "neo4j:5.15"
}

$PackageRoot = Ensure-Directory $PackageRoot
$pidFileBeforeSync = Join-Path $PackageRoot "run\pids.json"
$stopScriptBeforeSync = Join-Path $deployRoot "stop_dbcad_fullstack.ps1"
if ((-not $KeepExistingServices) -and (Test-Path -LiteralPath $stopScriptBeforeSync) -and (Test-Path -LiteralPath $pidFileBeforeSync)) {
    Write-Host "[INFO] Stopping services from previous launcher run..."
    & $stopScriptBeforeSync -PackageRoot $PackageRoot -Quiet
}

$latestExe = Resolve-LatestCadDbExe -ProjectRoot $projectRoot -RepoRoot $repoRoot -PackageRoot $PackageRoot -ExplicitPath $ClientExePath
if ([string]::IsNullOrWhiteSpace($Neo4jUri)) {
    $Neo4jUri = "bolt://$Neo4jHost`:$Neo4jPort"
} else {
    $uriParts = Split-Neo4jUri $Neo4jUri
    if (-not $PSBoundParameters.ContainsKey("Neo4jHost")) { $Neo4jHost = $uriParts.Host }
    if (-not $PSBoundParameters.ContainsKey("Neo4jPort")) { $Neo4jPort = $uriParts.Port }
}

if ([string]::IsNullOrWhiteSpace($Neo4jUser)) { $Neo4jUser = "neo4j" }
if ([string]::IsNullOrWhiteSpace($Neo4jPassword)) { throw "Neo4j password is empty. Set DBCAD_PASSWORD in $ConfigPath or pass -Neo4jPassword." }
if ([string]::IsNullOrWhiteSpace($ApiPassword)) { throw "FastAPI password is empty. Set DBCAD_PASSWORD in $ConfigPath or pass -ApiPassword." }
if ([string]::IsNullOrWhiteSpace($Neo4jDockerName)) { $Neo4jDockerName = "neo4j-apoc" }
if ([string]::IsNullOrWhiteSpace($Neo4jDockerImage)) { $Neo4jDockerImage = "neo4j:5.15" }
if ([string]::IsNullOrWhiteSpace($Neo4jDockerDataRoot)) { $Neo4jDockerDataRoot = Join-Path $repoRoot "neo4j-runtime" }

$fastApiProbeHost = ConvertTo-LocalHealthHost $FastApiHost
$bridgeProbeHost = ConvertTo-LocalHealthHost $BridgeHost
$fastApiUrl = "http://$fastApiProbeHost`:$FastApiPort"
$fastApiListenUrl = "http://$FastApiHost`:$FastApiPort"
$bridgeUrl = "http://$bridgeProbeHost`:$BridgePort"
$bridgeListenUrl = "http://$BridgeHost`:$BridgePort"

if (-not $SkipPackageSync) {
    Write-Host "[INFO] Syncing fullstack package: $PackageRoot"
    Write-Host "[INFO] Latest CAD_DB.exe: $($latestExe.FullName) ($($latestExe.LastWriteTime))"
    $paths = Sync-FullstackPackage `
        -ProjectRoot $projectRoot `
        -RepoRoot $repoRoot `
        -DeployRoot $deployRoot `
        -ConfigPath $ConfigPath `
        -BackendRoot $backendRoot `
        -PackageRoot $PackageRoot `
        -LatestExe $latestExe `
        -FastApiUrl $fastApiUrl `
        -Author $Author `
        -ApiPassword $ApiPassword `
        -BridgeHost $BridgeHost `
        -BridgePort $BridgePort `
        -Neo4jHost $Neo4jHost `
        -Neo4jPort $Neo4jPort `
        -Neo4jUser $Neo4jUser `
        -Neo4jPassword $Neo4jPassword `
        -Neo4jUri $Neo4jUri `
        -StorageBridgeTimeoutSeconds $StorageBridgeTimeoutSeconds
} else {
    $paths = @{
        ClientDir = Ensure-Directory (Join-Path $PackageRoot "client")
        ServerDir = Ensure-Directory (Join-Path $PackageRoot "server")
        BridgeDir = Ensure-Directory (Join-Path $PackageRoot "server\bridge-bin")
    }
}

$runDir = Ensure-Directory (Join-Path $PackageRoot "run")
$logDir = Ensure-Directory (Join-Path $runDir "logs")
$pidFile = Join-Path $runDir "pids.json"
$clientExe = Join-Path $paths.ClientDir "CAD_DB.exe"
$bridgeExe = Join-Path $paths.BridgeDir "CAD_DB.exe"
if (!(Test-Path -LiteralPath $clientExe)) {
    throw "Client executable not found in package: $clientExe"
}
if (!(Test-Path -LiteralPath $bridgeExe)) {
    throw "Bridge executable not found in package: $bridgeExe"
}

if ($PrepareOnly) {
    Write-Host "[OK] DBCAD fullstack package is prepared."
    Write-Host "[INFO] Package: $PackageRoot"
    Write-Host "[INFO] Source executable: $($latestExe.FullName)"
    Write-Host "[INFO] Client executable: $clientExe"
    Write-Host "[INFO] Bridge executable: $bridgeExe"
    return
}

$bridgeProcess = $null
$backendProcess = $null
$clientProcess = $null

if (-not $SkipNeo4jDocker) {
    Ensure-Neo4jDocker `
        -RepoRoot $repoRoot `
        -ContainerName $Neo4jDockerName `
        -Image $Neo4jDockerImage `
        -Neo4jHostName $Neo4jHost `
        -HttpPort $Neo4jHttpPort `
        -BoltPort $Neo4jPort `
        -User $Neo4jUser `
        -Password $Neo4jPassword `
        -DataRoot $Neo4jDockerDataRoot `
        -ResetData ([bool]$ResetNeo4jDockerData)
}

if (-not $SkipNeo4jPortCheck) {
    Write-Host "[INFO] Checking Neo4j bolt port: $Neo4jHost`:$Neo4jPort"
    if (-not (Wait-TcpPort -HostName $Neo4jHost -Port $Neo4jPort -Attempts $RetryCount -DelaySeconds $RetryDelaySeconds)) {
        throw "Neo4j is not reachable at $Neo4jHost`:$Neo4jPort. Start Neo4j first, pass the correct -Neo4jHost/-Neo4jPort, or use -SkipNeo4jPortCheck for remote/proxied deployments."
    }
}

if (-not $SkipNeo4jDocker) {
    Write-Host "[INFO] Checking Neo4j APOC plugin..."
    Wait-Neo4jApoc `
        -ContainerName $Neo4jDockerName `
        -User $Neo4jUser `
        -Password $Neo4jPassword `
        -Attempts $RetryCount `
        -DelaySeconds $RetryDelaySeconds
}

if (Test-ServiceHealth "$bridgeUrl/health") {
    if ($KeepExistingServices) {
        Write-Warning "Bridge is already healthy at $bridgeUrl/health. Reusing it because -KeepExistingServices was set."
    } else {
        throw "Bridge is already running at $bridgeUrl/health. Stop the old process first or pass -KeepExistingServices to reuse it."
    }
} else {
    Write-Host "[INFO] Starting storage bridge: $bridgeListenUrl"
    $bridgeProcess = Start-StorageBridge `
        -ExePath $bridgeExe `
        -WorkingDirectory $paths.BridgeDir `
        -BridgeHost $BridgeHost `
        -BridgePort $BridgePort `
        -Neo4jHost $Neo4jHost `
        -Neo4jPort $Neo4jPort `
        -Neo4jUser $Neo4jUser `
        -Neo4jPassword $Neo4jPassword `
        -LogDir $logDir

    if (-not (Wait-ServiceHealth -Url "$bridgeUrl/health" -Process $bridgeProcess -ServiceName "Bridge" -Attempts $RetryCount -DelaySeconds $RetryDelaySeconds)) {
        $exitInfo = if ($bridgeProcess.HasExited) { " ExitCode=$($bridgeProcess.ExitCode)." } else { "" }
        Stop-StartedProcess -Process $bridgeProcess -Name "storage bridge"
        throw "Bridge failed to become healthy.$exitInfo See logs in $logDir"
    }
}

if (Test-ServiceHealth "$fastApiUrl/health") {
    if ($KeepExistingServices) {
        Write-Warning "FastAPI is already healthy at $fastApiUrl/health. Reusing it because -KeepExistingServices was set."
    } else {
        throw "FastAPI is already running at $fastApiUrl/health. Stop the old process first or pass -KeepExistingServices to reuse it."
    }
} else {
    Write-Host "[INFO] Starting FastAPI: $fastApiListenUrl"
    $backendProcess = Start-FastApi `
        -ServerDir $paths.ServerDir `
        -HostAddress $FastApiHost `
        -Port $FastApiPort `
        -BridgeUrl $bridgeUrl `
        -BridgeTimeoutSeconds $StorageBridgeTimeoutSeconds `
        -ApiPassword $ApiPassword `
        -SkipDependencySync ([bool]$SkipDependencySync) `
        -SkipBridgeHealthCheck ([bool]$SkipBridgeHealthCheck) `
        -LogDir $logDir

    if (-not (Wait-ServiceHealth -Url "$fastApiUrl/health" -Process $backendProcess -ServiceName "FastAPI" -Attempts $RetryCount -DelaySeconds $RetryDelaySeconds)) {
        $exitInfo = if ($backendProcess.HasExited) { " ExitCode=$($backendProcess.ExitCode)." } else { "" }
        Stop-StartedProcess -Process $backendProcess -Name "FastAPI"
        Stop-StartedProcess -Process $bridgeProcess -Name "storage bridge"
        throw "FastAPI failed to become healthy.$exitInfo See logs in $logDir"
    }
}

if (-not $SkipClient) {
    Write-Host "[INFO] Starting client: $clientExe"
    $clientProcess = Start-Process -FilePath $clientExe -WorkingDirectory $paths.ClientDir -PassThru
}

$pidData = [ordered]@{
    package_root = $PackageRoot
    generated_exe = $latestExe.FullName
    client_exe = $clientExe
    bridge_exe = $bridgeExe
    bridge_pid = if ($null -ne $bridgeProcess) { $bridgeProcess.Id } else { $null }
    backend_pid = if ($null -ne $backendProcess) { $backendProcess.Id } else { $null }
    client_pid = if ($null -ne $clientProcess) { $clientProcess.Id } else { $null }
    bridge_url = $bridgeUrl
    fastapi_url = $fastApiUrl
    started_at = (Get-Date).ToString("o")
}
Write-PidFile -Path $pidFile -Data $pidData

Write-Host "[OK] DBCAD fullstack is ready."
Write-Host "[INFO] Package: $PackageRoot"
Write-Host "[INFO] Source executable: $($latestExe.FullName)"
Write-Host "[INFO] Bridge health: $bridgeUrl/health"
Write-Host "[INFO] FastAPI health: $fastApiUrl/health"
Write-Host "[INFO] FastAPI docs: $fastApiUrl/docs"
Write-Host "[INFO] PID file: $pidFile"
