$ErrorActionPreference = "Stop"

Set-Location (Join-Path $PSScriptRoot "..\backend")

Write-Host "[1/3] assignment 01"
uv run --extra dev --frozen pytest tests/test_assignment_01_crud_logic.py
if ($LASTEXITCODE) { exit $LASTEXITCODE }

Write-Host "[2/3] assignment 02"
uv run --extra dev --frozen pytest tests/test_assignment_02_sync_manager.py
if ($LASTEXITCODE) { exit $LASTEXITCODE }

Write-Host "[3/3] assignment 03"
uv run --extra dev --frozen pytest tests/test_assignment_03_ws_protocol.py
