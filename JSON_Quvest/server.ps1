$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$workspaceRoot = Split-Path -Parent $scriptDir
$targetScript = Join-Path $workspaceRoot "Support\JSON_Quvest\server.ps1"

if (-not (Test-Path -LiteralPath $targetScript)) {
  Write-Host "Target server not found: $targetScript" -ForegroundColor Red
  exit 1
}

& $targetScript
