$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$workspaceRoot = Split-Path -Parent $scriptDir
$targetScript = Join-Path $workspaceRoot "Support\JSON_Quvest\start-editor.ps1"

if (-not (Test-Path -LiteralPath $targetScript)) {
  Write-Host "Target launcher not found: $targetScript" -ForegroundColor Red
  exit 1
}

& $targetScript
