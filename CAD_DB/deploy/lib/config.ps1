# Config helpers for the fullstack launcher.
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
        "DBCAD_AUTHOR",
        "DBCAD_POSTGRES_DOCKER_IMAGE",
        "DBCAD_POSTGRES_DOCKER_NAME",
        "DBCAD_POSTGRES_HOST",
        "DBCAD_POSTGRES_PORT",
        "DBCAD_POSTGRES_USER",
        "DBCAD_POSTGRES_DBNAME"
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
        DBCAD_POSTGRES_DOCKER_IMAGE = "postgres:16"
        DBCAD_POSTGRES_DOCKER_NAME = "dbcad-postgres-demo"
        DBCAD_POSTGRES_HOST = "127.0.0.1"
        DBCAD_POSTGRES_PORT = "5432"
        DBCAD_POSTGRES_USER = "postgres"
        DBCAD_POSTGRES_DBNAME = "dbcad_demo"
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
