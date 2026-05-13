$ErrorActionPreference = "Stop"

Set-Location (Join-Path $PSScriptRoot "..\backend")

Write-Host "[1/3] assignment 01"
uv run pytest tests/test_assignment_01_crud_logic.py

Write-Host "[2/3] assignment 02"
uv run pytest tests/test_assignment_02_sync_manager.py

Write-Host "[3/3] assignment 03"
uv run pytest tests/test_assignment_03_ws_protocol.py
