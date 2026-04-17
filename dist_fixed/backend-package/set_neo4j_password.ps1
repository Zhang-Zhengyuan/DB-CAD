param(
    [Parameter(Mandatory = $true)]
    [string]$Neo4jPassword
)

$ErrorActionPreference = "Stop"

$envFile = Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) ".env"
if (!(Test-Path $envFile)) {
    throw ".env file not found: $envFile"
}

$content = Get-Content $envFile
$updated = $false
for ($i = 0; $i -lt $content.Count; $i++) {
    if ($content[$i] -like "CAD_DB_NEO4J_PASSWORD=*") {
        $content[$i] = "CAD_DB_NEO4J_PASSWORD=$Neo4jPassword"
        $updated = $true
    }
}
if (-not $updated) {
    $content += "CAD_DB_NEO4J_PASSWORD=$Neo4jPassword"
}

Set-Content -Path $envFile -Value $content -Encoding UTF8
Write-Host "[OK] Updated CAD_DB_NEO4J_PASSWORD in .env"
