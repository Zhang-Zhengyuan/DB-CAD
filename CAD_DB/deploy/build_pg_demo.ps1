# Build pg_demo.exe
#
# Standalone compile of the PostgreSQL SAT-text demo. This does NOT
# participate in the main CAD_DB.vcxproj target, so it has no impact on
# the existing ACIS / Qt / Neo4j build path.
#
# Requirements before running this script:
#   * vcpkg is installed at $VCPKG_ROOT and libpqxx + libpq + openssl
#     have been built for x64-windows-static-md, e.g.:
#         vcpkg install libpqxx:x64-windows-static-md
#     (the script will use the include / lib directories under
#      $VCPKG_ROOT\installed\x64-windows-static-md).
#   * MSVC environment loaded (run from a "x64 Native Tools Command Prompt"
#     or this script will call vcvarsall.bat for you).

[CmdletBinding()]
param(
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Debug",
    [string]$Platform = "x64",
    [string]$Output = "",
    [string]$VcpkgRoot = $env:VCPKG_ROOT
)

$ErrorActionPreference = "Stop"

function Resolve-FullPath([string]$Path) {
    if ([string]::IsNullOrWhiteSpace($Path)) { return $Path }
    $item = Get-Item -LiteralPath $Path -ErrorAction SilentlyContinue
    if ($null -ne $item) { return $item.FullName }
    $parent = Split-Path -Parent $Path
    if ([string]::IsNullOrWhiteSpace($parent)) { $parent = "." }
    $resolvedParent = (Resolve-Path -LiteralPath $parent -ErrorAction SilentlyContinue).Path
    if ([string]::IsNullOrWhiteSpace($resolvedParent)) {
        $resolvedParent = (Get-Item -LiteralPath $parent -ErrorAction SilentlyContinue).FullName
    }
    if ([string]::IsNullOrWhiteSpace($resolvedParent)) {
        $resolvedParent = (Get-Location).Path
    }
    return (Join-Path $resolvedParent (Split-Path -Leaf $Path))
}

function Resolve-MsvcEnvironment {
    param([string]$TargetPlatform)

    if ($env:VSCMD_DEBUG -or $env:VCINSTALLDIR) {
        return  # already loaded
    }

    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path -LiteralPath $vswhere)) {
        Write-Warning "vswhere.exe not found. Open 'x64 Native Tools Command Prompt' first."
        return
    }

    $vs = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2>$null
    if (-not $vs) {
        Write-Warning "MSVC installation not found."
        return
    }

    $vcvars = Join-Path $vs "VC\Auxiliary\Build\vcvars64.bat"
    if (-not (Test-Path -LiteralPath $vcvars)) {
        Write-Warning "vcvars64.bat not found at $vcvars"
        return
    }

    Write-Host "[INFO] Loading MSVC env from $vcvars"
    $envLines = & cmd /c "`"$vcvars`" && set"
    foreach ($line in $envLines) {
        if ($line -match "^(?<k>[A-Z_][A-Z0-9_()]*)=(?<v>.*)$") {
            [System.Environment]::SetEnvironmentVariable($matches["k"], $matches["v"], "Process")
        }
    }
}

function Resolve-VcpkgRoot {
    param([string]$Explicit)
    if ($Explicit -and (Test-Path -LiteralPath $Explicit)) { return (Resolve-FullPath $Explicit) }
    $candidates = @("C:\vcpkg", "D:\vcpkg", "D:\tools\vcpkg", "C:\tools\vcpkg")
    foreach ($c in $candidates) {
        if (Test-Path -LiteralPath (Join-Path $c "vcpkg.exe")) { return (Resolve-FullPath $c) }
    }
    return $null
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectRoot = Resolve-FullPath (Join-Path $scriptRoot "..")
if ([string]::IsNullOrWhiteSpace($Output)) {
    $Output = Join-Path $projectRoot "x64\Demo\pg_demo.exe"
}
Push-Location $projectRoot
try {
    Resolve-MsvcEnvironment -TargetPlatform $Platform

    $vcpkgRoot = Resolve-VcpkgRoot -Explicit $VcpkgRoot
    if (-not $vcpkgRoot) {
        throw "vcpkg root not found. Set -VcpkgRoot 'C:\vcpkg' or place vcpkg.exe under C:\vcpkg."
    }
    Write-Host "[INFO] Using vcpkg at: $vcpkgRoot"

    $vcpkgInclude = Join-Path $vcpkgRoot "installed\x64-windows\include"
    $vcpkgLib     = Join-Path $vcpkgRoot "installed\x64-windows\lib"
    $vcpkgBin     = Join-Path $vcpkgRoot "installed\x64-windows\bin"

    if (-not (Test-Path -LiteralPath (Join-Path $vcpkgInclude "pqxx\pqxx"))) {
        throw "libpqxx headers not found at $vcpkgInclude\pqxx\pqxx. Run: vcpkg install libpqxx:x64-windows"
    }
    if (-not (Test-Path -LiteralPath (Join-Path $vcpkgLib "pqxx.lib"))) {
        throw "pqxx.lib not found at $vcpkgLib. Run: vcpkg install libpqxx:x64-windows"
    }

    $cl = (Get-Command cl.exe -ErrorAction SilentlyContinue).Source
    if (-not $cl) {
        throw "cl.exe not in PATH. Open 'x64 Native Tools Command Prompt for VS' first or let this script auto-load MSVC env (requires vswhere to find VS)."
    }
    Write-Host "[INFO] cl.exe: $cl"

    $rt = if ($Config -eq 'Debug') { '/MDd' } else { '/MD' }
    $outDir = Split-Path -Parent $Output
    if (-not (Test-Path -LiteralPath $outDir)) {
        New-Item -ItemType Directory -Path $outDir -Force | Out-Null
    }
    $outDir = (Get-Item -LiteralPath $outDir).FullName
    $resolvedOutput = Join-Path $outDir (Split-Path -Leaf $Output)

    $clArgs = @(
        "/nologo",
        "/EHsc",
        "/std:c++20",
        $rt,
        "/$Config",
        "/I`"$vcpkgInclude`"",
        "/Fe`"$resolvedOutput`"",
        "/Fo`"$outDir\\`"",
        "/link",
        "/LIBPATH:`"$vcpkgLib`"",
        "pqxx.lib",
        "pq.lib",
        "libssl.lib",
        "libcrypto.lib",
        "ws2_32.lib",
        "secur32.lib",
        "crypt32.lib",
        "wldap32.lib",
        "Normaliz.lib"
    )

    Write-Host "[INFO] cl.exe sources: pg_demo.cpp pg_store.cpp"
    Write-Host "[INFO] cl.exe flags:"
    foreach ($a in $clArgs) { Write-Host "         $a" }

    $argLine = ($clArgs -join ' ') + ' pg_demo.cpp pg_store.cpp'
    Write-Host "[INFO] full command line:"
    Write-Host "         cl.exe $argLine"
    & cmd.exe /c "cd /d `"$projectRoot`" && cl.exe $argLine"
    if ($LASTEXITCODE -ne 0) {
        throw "Compile or link failed (exit $LASTEXITCODE). See output above."
    }

    Write-Host "[OK] Built $resolvedOutput"

    # Copy runtime DLLs next to the exe so the user does not need to fiddle
    # with PATH or vcpkg bin. Idempotent.
    $binDir = Split-Path -Parent $resolvedOutput
    foreach ($dll in @("pqxx.dll", "libpq.dll", "libssl-3-x64.dll",
                       "libcrypto-3-x64.dll", "z.dll", "lz4.dll")) {
        $src = Join-Path $vcpkgBin $dll
        if (Test-Path -LiteralPath $src) {
            Copy-Item -LiteralPath $src -Destination $binDir -Force | Out-Null
        }
    }
    Write-Host "[OK] Copied runtime DLLs next to $resolvedOutput"

    Write-Host ""
    Write-Host "Next steps:"
    Write-Host "   .\deploy\start_pg_demo.ps1"
    Write-Host "   $resolvedOutput init"
}
finally {
    Pop-Location
}