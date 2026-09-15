$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$indexPath = Join-Path $scriptDir "index.html"
$defaultConfigPath = Join-Path $scriptDir "editor-config.json"
$localConfigPath = Join-Path $scriptDir "editor-config.local.json"
$serverScript = Join-Path $scriptDir "server.ps1"
$serviceIdentity = "Silver_77_Quests.WebWorkshop"
$editorVersion = "2026-09-14-task175"
$editorUrl = "http://127.0.0.1:4173/index.html?v=$editorVersion"

function Get-WorkshopHealth {
  try {
    $response = Invoke-WebRequest -Uri "http://127.0.0.1:4173/api/health" -UseBasicParsing -TimeoutSec 1
    if ($response.StatusCode -ne 200) {
      return $null
    }
    $payload = $response.Content | ConvertFrom-Json
    return [pscustomobject]@{
      response = $response
      payload = $payload
    }
  } catch {
    return $null
  }
}

function Get-CanonicalWorkshopServerProcessIds {
  $knownServerScripts = @(
    [System.IO.Path]::GetFullPath($serverScript),
    "P:\Silver_77_Quests\JSON_Quvest\server.ps1",
    "D:\Dayz\Silver_77_Quests\JSON_Quvest\server.ps1"
  ) | Select-Object -Unique

  try {
    return @(Get-CimInstance Win32_Process -ErrorAction Stop |
      Where-Object {
        $process = $_
        $process.Name -in @("powershell.exe", "pwsh.exe") -and
        $process.ProcessId -ne $PID -and
        $process.CommandLine -and
        ($knownServerScripts | Where-Object {
          $process.CommandLine.IndexOf($_, [System.StringComparison]::OrdinalIgnoreCase) -ge 0
        }).Count -gt 0
      } |
      Select-Object -ExpandProperty ProcessId -Unique)
  } catch {
    return @()
  }
}

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

$existingHealth = Get-WorkshopHealth
$serverProcessIds = Get-CanonicalWorkshopServerProcessIds

if ($null -ne $existingHealth) {
  $existingService = [string]$existingHealth.payload.service
  $identifiedByService = [string]::Equals($existingService, $serviceIdentity, [System.StringComparison]::Ordinal)
  $identifiedLegacyProcess = $serverProcessIds.Count -gt 0

  if (-not $identifiedByService -and -not $identifiedLegacyProcess) {
    Write-Host "Port 4173 is occupied by an unknown HTTP responder. Browser was not opened." -ForegroundColor Red
    exit 1
  }

  try {
    Invoke-WebRequest -Uri "http://127.0.0.1:4173/api/shutdown" -Method Post -UseBasicParsing -TimeoutSec 2 | Out-Null
  } catch {
    Write-Host "Confirmed Workshop server did not accept graceful shutdown. Browser was not opened." -ForegroundColor Red
    exit 1
  }
  Start-Sleep -Milliseconds 500
}

foreach ($serverProcessId in $serverProcessIds) {
  if (Get-Process -Id $serverProcessId -ErrorAction SilentlyContinue) {
    try {
      Stop-Process -Id $serverProcessId -Force -ErrorAction Stop
    } catch {
      Write-Host "Confirmed Workshop process $serverProcessId could not be stopped. Browser was not opened." -ForegroundColor Red
      exit 1
    }
  }
}

if ($serverProcessIds.Count -gt 0) {
  Start-Sleep -Milliseconds 500
}

$serverReady = $false
$instanceToken = [guid]::NewGuid().ToString("D")
$serverProcess = Start-Process powershell -ArgumentList @(
  "-NoProfile",
  "-ExecutionPolicy", "Bypass",
  "-WindowStyle", "Hidden",
  "-File", $serverScript,
  "-InstanceToken", $instanceToken
) -PassThru

$lastHealthIdentity = "no response"

for ($attempt = 0; $attempt -lt 20; $attempt++) {
  Start-Sleep -Milliseconds 250
  $health = Get-WorkshopHealth
  if ($null -eq $health) {
    continue
  }
  $observedService = [string]$health.payload.service
  $observedToken = [string]$health.payload.instanceToken
  $lastHealthIdentity = "service='$observedService', token='$observedToken'"
  if (
    [string]::Equals($observedService, $serviceIdentity, [System.StringComparison]::Ordinal) -and
    [string]::Equals($observedToken, $instanceToken, [System.StringComparison]::Ordinal)
  ) {
    $serverReady = $true
    break
  }
}

if (-not $serverReady) {
  if ($serverProcess -and -not $serverProcess.HasExited) {
    Stop-Process -Id $serverProcess.Id -Force -ErrorAction SilentlyContinue
  }
  Write-Host "Canonical Workshop server did not become READY. Observed: $lastHealthIdentity. Browser was not opened." -ForegroundColor Red
  exit 1
}

Write-Host "Workshop server READY: service=$serviceIdentity instanceToken=$instanceToken"

Start-Process $editorUrl | Out-Null

Start-Sleep -Milliseconds 1400

try {
  $shell = New-Object -ComObject WScript.Shell
  [void]$shell.AppActivate("Quest JSON Workshop")
} catch {
  # If the browser ignores activation, the editor is still opened.
}
