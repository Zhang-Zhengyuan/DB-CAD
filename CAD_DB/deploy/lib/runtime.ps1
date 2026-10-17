# Runtime helpers for the fullstack launcher.
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

function Get-BuildWinDeployQtPath([System.IO.FileInfo]$Exe, [string]$RepoRoot) {
    # Qt VS Tools records the kit used to compile this configuration. Prefer
    # that evidence to arbitrary Qt versions found elsewhere on the machine.
    $configuration = if (Test-DebugBuild $Exe) { 'Debug' } else { 'Release' }
    foreach ($path in @(
        (Join-Path $RepoRoot "x64\obj\CAD_DB\$configuration\qt\qtvars.xml"),
        (Join-Path $RepoRoot "CAD_DB\x64\$configuration\qt\qtvars.xml"),
        (Join-Path $Exe.DirectoryName 'qt\qtvars.xml')
    )) {
        if (Test-Path -LiteralPath $path) {
            [xml]$vars = Get-Content -LiteralPath $path -Raw
            $kit = $vars.SelectSingleNode("//*[local-name()='QMake_QT_INSTALL_PREFIX_']")
            if ($kit) {
                $tool = Join-Path $kit.InnerText.Trim() 'bin\windeployqt.exe'
                if (Test-Path -LiteralPath $tool) { return $tool }
            }
        }
    }
    return ''
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

    foreach ($qtInstall in @($env:DBCAD_QT_ROOT, $env:QTDIR)) {
        if ($qtInstall) {
            $candidate = Join-Path $qtInstall "bin\windeployqt.exe"
            if (Test-Path -LiteralPath $candidate) { return (Resolve-FullPath $candidate) }
        }
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

    # Do not guess an unrelated Qt version, architecture or compiler kit.
    return ""
}

function Test-DebugBuild([System.IO.FileInfo]$Exe) {
    # Select Qt deployment mode from actual imports, not the build folder.
    # The /MD Debug client uses release Qt while retaining PDBs/Debug ACIS.
    return @(Get-PeImports $Exe.FullName) -contains 'Qt6Cored.dll'
}

function Invoke-WinDeployQtIfAvailable([string]$ExePath, [bool]$IsDebug) {
    if ($SkipWinDeployQt) {
        return
    }

    $windeployqt = Get-WinDeployQtPath -ExplicitPath $WinDeployQtPath
    if ([string]::IsNullOrWhiteSpace($windeployqt)) {
        throw 'windeployqt was not found. Set DBCAD_QT_ROOT to the build kit or pass -WinDeployQtPath. Packaging stopped before launching an incomplete client.'
    }

    $mode = if ($IsDebug) { "--debug" } else { "--release" }
    Write-Host "[INFO] Running windeployqt for runtime dependencies: $windeployqt $mode"
    & $windeployqt $mode --compiler-runtime $ExePath
    if ($LASTEXITCODE -ne 0) { throw "windeployqt failed with exit code $LASTEXITCODE" }
}

function Resolve-LatestCadDbExe(
    [string]$ProjectRoot,
    [string]$RepoRoot,
    [string]$PackageRoot,
    [string]$ExplicitPath,
    [ValidateSet("Auto", "Debug", "Release")][string]$Configuration = "Auto",
    [switch]$SourceCheckout
) {
    if (-not [string]::IsNullOrWhiteSpace($ExplicitPath)) {
        if (Test-Path -LiteralPath $ExplicitPath) {
            return Get-Item -LiteralPath $ExplicitPath
        }
        throw "CAD_DB.exe not found: $ExplicitPath"
    }

    # A source launcher must never fall back to an old copied exe or package.
    # Package launchers use their own client even when a checkout exists nearby.
    if (-not $SourceCheckout) {
        $packagedExe = Join-Path $PackageRoot "client\CAD_DB.exe"
        if (Test-Path -LiteralPath $packagedExe) { return Get-Item -LiteralPath $packagedExe }
        throw "Packaged CAD_DB.exe not found: $packagedExe"
    }

    $configurations = if ($Configuration -eq "Auto") { @("Debug", "Release") } else { @($Configuration) }
    $candidates = @(foreach ($buildConfiguration in $configurations) {
        $path = Join-Path $RepoRoot "x64\$buildConfiguration\CAD_DB.exe"
        if (Test-Path -LiteralPath $path) { Get-Item -LiteralPath $path }
    })
    if ($candidates.Count -eq 0) {
        throw "No $Configuration build found in $RepoRoot\x64. Run scripts/build.ps1 -Configuration Debug or Release; custom builds require -ClientExePath."
    }
    # Debug wins ties, making the default predictable for local two-client tests.
    return ($candidates | Sort-Object @{ Expression = "LastWriteTime"; Descending = $true }, @{ Expression = "FullName"; Descending = $false } | Select-Object -First 1)
}

function Clear-PackagedRuntime([string]$Directory, [string]$PackageRoot) {
    # Only generated binaries/plugins are removed. Keep models, logs and local
    # user state. Never clean the source build output or a path outside the package.
    $resolved = (Resolve-Path -LiteralPath $Directory).Path
    $allowed = (Resolve-Path -LiteralPath $PackageRoot).Path.TrimEnd('\', '/') + '\'
    if (-not $resolved.StartsWith($allowed, [System.StringComparison]::OrdinalIgnoreCase)) { throw "Runtime cleanup outside package: $resolved" }
    Get-ChildItem -LiteralPath $resolved -File | Where-Object { $_.Extension -in @('.dll','.exe','.pdb','.ilk','.exp') -or $_.Name -eq 'qt.conf' } | Remove-Item -Force
    foreach ($name in @('platforms','imageformats','iconengines','styles','tls','networkinformation','generic','translations')) {
        $path = Join-Path $resolved $name
        if (Test-Path -LiteralPath $path) { Remove-DirectorySafe -Path $path -AllowedRoot $PackageRoot }
    }
}

function Copy-ClientBuildRuntime([System.IO.FileInfo]$Exe, [string]$Destination) {
    # Build outputs may contain object files, old packages and test data; copy
    # only runtime files. windeployqt regenerates the matching Qt plugins.
    Get-ChildItem -LiteralPath $Exe.DirectoryName -File | Where-Object { $_.Extension -in @('.dll','.exe') -and $_.Name -notmatch '^Qt[56]' } |
        Copy-Item -Destination $Destination -Force
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
