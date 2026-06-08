$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$indexPath = Join-Path $scriptDir "index.html"
$defaultConfigPath = Join-Path $scriptDir "editor-config.json"
$localConfigPath = Join-Path $scriptDir "editor-config.local.json"
$serverScript = Join-Path $scriptDir "server.ps1"
$editorVersion = "2026-04-25"
$editorUrl = "http://127.0.0.1:4173/index.html?v=$editorVersion"

function Get-EmbeddedDefaultConfig {
  return [pscustomobject]@{
    savePath = "Silver_77_Quests.json"
    backupPath = "Silver_77_Quests_BackUP.json"
    profileExportPath = ""
  }
}

function Read-JsonFile([string]$path) {
  if (-not (Test-Path -LiteralPath $path)) {
    return $null
  }

  try {
    return Get-Content -LiteralPath $path -Raw | ConvertFrom-Json
  } catch {
    return $null
  }
}

function Get-ConfigValue($config, [string]$name) {
  if ($null -eq $config) {
    return ""
  }

  $property = $config.PSObject.Properties[$name]
  if ($null -eq $property) {
    return ""
  }

  return [string]$property.Value
}

function Get-FirstConfiguredValue([object[]]$sources, [string]$name) {
  foreach ($source in $sources) {
    $value = Get-ConfigValue $source $name
    if (-not [string]::IsNullOrWhiteSpace($value)) {
      return $value.Trim()
    }
  }

  return ""
}

function Get-FirstConfiguredValueOrExplicitEmpty([object[]]$sources, [string]$name) {
  foreach ($source in $sources) {
    if ($null -eq $source) {
      continue
    }

    $property = $source.PSObject.Properties[$name]
    if ($null -ne $property) {
      return ([string]$property.Value).Trim()
    }
  }

  return ""
}

function New-EditorConfig([object[]]$sources) {
  $defaults = Get-EmbeddedDefaultConfig
  $allSources = @($sources) + @($defaults)

  return [pscustomobject]@{
    savePath = Get-FirstConfiguredValue $allSources "savePath"
    backupPath = Get-FirstConfiguredValue $allSources "backupPath"
    profileExportPath = Get-FirstConfiguredValueOrExplicitEmpty $allSources "profileExportPath"
  }
}

function Get-EditorConfig {
  return New-EditorConfig @(
    (Read-JsonFile $localConfigPath),
    (Read-JsonFile $defaultConfigPath)
  )
}

function Resolve-ConfigPath([string]$path, [string]$defaultFileName = "") {
  $value = [string]$path
  if ([string]::IsNullOrWhiteSpace($value)) {
    return ""
  }

  $resolvedPath = ""
  if ([System.IO.Path]::IsPathRooted($value)) {
    $resolvedPath = $value
  } else {
    $resolvedPath = Join-Path $scriptDir $value
  }

  $resolvedPath = [System.IO.Path]::GetFullPath($resolvedPath)

  $looksLikeDirectory = $resolvedPath.EndsWith([System.IO.Path]::DirectorySeparatorChar) -or
    $resolvedPath.EndsWith([System.IO.Path]::AltDirectorySeparatorChar)

  if ((Test-Path -LiteralPath $resolvedPath -PathType Container) -or $looksLikeDirectory) {
    if ([string]::IsNullOrWhiteSpace($defaultFileName)) {
      return ""
    }

    return Join-Path $resolvedPath $defaultFileName
  }

  return $resolvedPath
}

if (-not (Test-Path -LiteralPath $indexPath)) {
  Write-Host "index.html not found: $indexPath" -ForegroundColor Red
  exit 1
}

$config = Get-EditorConfig
$sourceFile = Resolve-ConfigPath ([string]$config.savePath) "Silver_77_Quests.json"
$backupFile = Resolve-ConfigPath ([string]$config.backupPath) "Silver_77_Quests_BackUP.json"

if ($sourceFile -and $backupFile -and (Test-Path -LiteralPath $sourceFile -PathType Leaf)) {
  $backupDirectory = Split-Path -Parent $backupFile
  if ($backupDirectory -and -not (Test-Path -LiteralPath $backupDirectory)) {
    New-Item -ItemType Directory -Force -Path $backupDirectory | Out-Null
  }

  $sameTarget = [string]::Equals(
    [System.IO.Path]::GetFullPath($sourceFile),
    [System.IO.Path]::GetFullPath($backupFile),
    [System.StringComparison]::OrdinalIgnoreCase
  )

  if ($sameTarget) {
    Write-Host "Backup skipped because savePath and backupPath point to the same file: $sourceFile" -ForegroundColor Yellow
  } else {
    Copy-Item -LiteralPath $sourceFile -Destination $backupFile -Force
  }
}

$serverProcessIds = @()

try {
  try {
    Invoke-WebRequest -Uri "http://127.0.0.1:4173/api/shutdown" -Method Post -UseBasicParsing -TimeoutSec 1 | Out-Null
    Start-Sleep -Milliseconds 300
  } catch {
  }

  $serverProcessIds = Get-CimInstance Win32_Process -ErrorAction Stop |
    Where-Object {
      $_.Name -eq "powershell.exe" -and
      $_.ProcessId -ne $PID -and
      $_.CommandLine -and
      $_.CommandLine -like "*server.ps1*" -and
      $_.CommandLine -like "*$serverScript*"
    } |
    Select-Object -ExpandProperty ProcessId -Unique
} catch {
  $serverProcessIds = @()
}

foreach ($serverProcessId in $serverProcessIds) {
  try {
    Stop-Process -Id $serverProcessId -Force -ErrorAction Stop
  } catch {
  }
}

if ($serverProcessIds.Count -gt 0) {
  Start-Sleep -Milliseconds 500
}

$serverReady = $false
Start-Process powershell -ArgumentList @(
  "-NoProfile",
  "-ExecutionPolicy", "Bypass",
  "-WindowStyle", "Hidden",
  "-File", $serverScript
) | Out-Null

for ($attempt = 0; $attempt -lt 20; $attempt++) {
  Start-Sleep -Milliseconds 250
  try {
    $health = Invoke-WebRequest -Uri "http://127.0.0.1:4173/api/health" -UseBasicParsing -TimeoutSec 1
    if ($health.StatusCode -eq 200) {
      $serverReady = $true
      break
    }
  } catch {
  }
}

if (-not $serverReady) {
  Write-Host "Local server did not start." -ForegroundColor Red
  exit 1
}

Start-Process $editorUrl | Out-Null

Start-Sleep -Milliseconds 1400

try {
  $shell = New-Object -ComObject WScript.Shell
  [void]$shell.AppActivate("Quest JSON Workshop")
} catch {
  # If the browser ignores activation, the editor is still opened.
}
