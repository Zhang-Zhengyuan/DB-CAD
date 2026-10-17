# Regression: GUI clients must be visible; only the backend runs hidden.
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$launcherPath = Join-Path $repoRoot 'CAD_DB\deploy\start_dbcad_fullstack.ps1'
$backendPath = Join-Path $repoRoot 'CAD_DB\deploy\lib\processes.ps1'
function Get-StartProcessCommand([string]$Path, [string]$FilePathVariable) {
    $tokens = $null
    $errors = $null
    $ast = [System.Management.Automation.Language.Parser]::ParseFile($Path, [ref]$tokens, [ref]$errors)
    if ($errors.Count) { throw "Parse error in $Path" }
    $matches = @($ast.FindAll({ param($node)
        $node -is [System.Management.Automation.Language.CommandAst] -and
        $node.GetCommandName() -eq 'Start-Process' -and
        $node.Extent.Text -match ('-FilePath\s+\$' + [regex]::Escape($FilePathVariable) + '\b')
    }.GetNewClosure(), $true))
    if ($matches.Count -ne 1) { throw "Expected one Start-Process for $FilePathVariable" }
    return $matches[0].Extent.Text
}
$clientCommand = Get-StartProcessCommand $launcherPath 'clientExe'
if ($clientCommand -notmatch '-WindowStyle\s+Normal\b') { throw 'Interactive CAD clients must use WindowStyle Normal.' }
$backendCommand = Get-StartProcessCommand $backendPath 'uv'
if ($backendCommand -notmatch '-WindowStyle\s+Hidden\b') { throw 'Background FastAPI must use WindowStyle Hidden.' }
Write-Host '[OK] Desktop client visibility and background service window styles verified.'
