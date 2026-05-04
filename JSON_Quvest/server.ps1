$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$projectRoot = Split-Path -Parent $scriptDir
$rootEditorDir = $scriptDir
$defaultConfigPath = Join-Path $scriptDir "editor-config.json"
$localConfigPath = Join-Path $scriptDir "editor-config.local.json"
$draftPath = Join-Path $scriptDir "editor-draft.json"
$stackRulesPath = Join-Path $rootEditorDir "item-stack-rules.json"
$listener = [System.Net.HttpListener]::new()
$listener.Prefixes.Add("http://127.0.0.1:4173/")
$listener.Start()

function Get-EmbeddedDefaultConfig {
  return [pscustomobject]@{
    savePath = "Silver_77_Quests.json"
    backupPath = "Silver_77_Quests_BackUP.json"
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

function Get-ObjectPropertyValue($object, [string]$name, $defaultValue = $null) {
  if ($null -eq $object) {
    return $defaultValue
  }

  $property = $object.PSObject.Properties[$name]
  if ($null -eq $property) {
    return $defaultValue
  }

  return $property.Value
}

function Convert-ToPositiveInt($value, [int]$fallback = 1) {
  $parsed = 0
  if ([int]::TryParse([string]$value, [ref]$parsed) -and $parsed -ge 1) {
    return $parsed
  }

  return $fallback
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

function New-EditorConfig([object[]]$sources) {
  $defaults = Get-EmbeddedDefaultConfig
  $allSources = @($sources) + @($defaults)

  return [pscustomobject]@{
    savePath = Get-FirstConfiguredValue $allSources "savePath"
    backupPath = Get-FirstConfiguredValue $allSources "backupPath"
  }
}

function Get-DefaultConfig {
  return New-EditorConfig @((Read-JsonFile $defaultConfigPath))
}

function Get-EditorConfig {
  return New-EditorConfig @(
    (Read-JsonFile $localConfigPath),
    (Read-JsonFile $defaultConfigPath)
  )
}

function Save-LocalEditorConfig($config) {
  $json = $config | ConvertTo-Json -Depth 4
  [System.IO.File]::WriteAllText($localConfigPath, $json, [System.Text.UTF8Encoding]::new($false))
}

function Remove-LocalEditorConfig {
  if (Test-Path -LiteralPath $localConfigPath) {
    Remove-Item -LiteralPath $localConfigPath -Force
  }
}

function Test-SameConfig($left, $right) {
  return (Get-ConfigValue $left "savePath") -eq (Get-ConfigValue $right "savePath") -and
    (Get-ConfigValue $left "backupPath") -eq (Get-ConfigValue $right "backupPath")
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

function Get-DefaultStackRules {
  return [pscustomobject]@{
    version = 1
    rules = @()
  }
}

function New-StackRulesDocument($source) {
  $rules = @()
  $sourceRules = Get-ObjectPropertyValue $source "rules" @()

  foreach ($rule in @($sourceRules)) {
    $rules += [pscustomobject]@{
      className = [string](Get-ObjectPropertyValue $rule "className" "")
      stackSize = Convert-ToPositiveInt (Get-ObjectPropertyValue $rule "stackSize" 1) 1
    }
  }

  return [pscustomobject]@{
    version = 1
    rules = $rules
  }
}

function Get-StackRules {
  $raw = Read-JsonFile $stackRulesPath
  if ($null -eq $raw) {
    return Get-DefaultStackRules
  }

  return New-StackRulesDocument $raw
}

function Save-StackRules($payload) {
  $document = New-StackRulesDocument $payload
  $json = $document | ConvertTo-Json -Depth 6
  [System.IO.File]::WriteAllText($stackRulesPath, $json, [System.Text.UTF8Encoding]::new($false))
}

function Get-ContentType([string]$path) {
  switch ([System.IO.Path]::GetExtension($path).ToLowerInvariant()) {
    ".html" { return "text/html; charset=utf-8" }
    ".css" { return "text/css; charset=utf-8" }
    ".js" { return "application/javascript; charset=utf-8" }
    ".json" { return "application/json; charset=utf-8" }
    ".cmd" { return "text/plain; charset=utf-8" }
    ".ps1" { return "text/plain; charset=utf-8" }
    default { return "application/octet-stream" }
  }
}

function Write-TextResponse($response, [int]$statusCode, [string]$contentType, [string]$body) {
  $bytes = [System.Text.Encoding]::UTF8.GetBytes($body)
  $response.StatusCode = $statusCode
  $response.ContentType = $contentType
  $response.Headers["Cache-Control"] = "no-store, no-cache, must-revalidate, max-age=0"
  $response.Headers["Pragma"] = "no-cache"
  $response.Headers["Expires"] = "0"
  $response.ContentLength64 = $bytes.Length
  $response.OutputStream.Write($bytes, 0, $bytes.Length)
}

function Write-FileResponse($response, [string]$path) {
  $bytes = [System.IO.File]::ReadAllBytes($path)
  $response.StatusCode = 200
  $response.ContentType = Get-ContentType $path
  $response.Headers["Cache-Control"] = "no-store, no-cache, must-revalidate, max-age=0"
  $response.Headers["Pragma"] = "no-cache"
  $response.Headers["Expires"] = "0"
  $response.ContentLength64 = $bytes.Length
  $response.OutputStream.Write($bytes, 0, $bytes.Length)
}

function Resolve-SafePath([string]$relativePath) {
  $trimmed = $relativePath.TrimStart("/")
  if ([string]::IsNullOrWhiteSpace($trimmed)) {
    $trimmed = "index.html"
  }

  if ($trimmed -match '\.\.') {
    return $null
  }

  $normalized = $trimmed -replace '/', '\'
  return Join-Path $scriptDir $normalized
}

while ($listener.IsListening) {
  $context = $listener.GetContext()
  $request = $context.Request
  $response = $context.Response

  try {
    $path = $request.Url.AbsolutePath

    if ($request.HttpMethod -eq "GET" -and $path -eq "/api/health") {
      Write-TextResponse $response 200 "application/json; charset=utf-8" '{"ok":true}'
      continue
    }

    if ($request.HttpMethod -eq "POST" -and $path -eq "/api/shutdown") {
      Write-TextResponse $response 200 "application/json; charset=utf-8" '{"ok":true}'
      $listener.Stop()
      continue
    }

    if ($request.HttpMethod -eq "GET" -and $path -eq "/api/config") {
      $configJson = (Get-EditorConfig) | ConvertTo-Json -Depth 4
      Write-TextResponse $response 200 "application/json; charset=utf-8" $configJson
      continue
    }

    if ($request.HttpMethod -eq "GET" -and $path -eq "/api/current-json") {
      $config = Get-EditorConfig
      $targetPath = Resolve-ConfigPath ([string]$config.savePath) "Silver_77_Quests.json"
      if ([string]::IsNullOrWhiteSpace($targetPath) -or -not (Test-Path -LiteralPath $targetPath)) {
        Write-TextResponse $response 404 "application/json; charset=utf-8" '{"ok":false,"error":"Configured savePath file was not found"}'
        continue
      }

      $jsonBytes = [System.IO.File]::ReadAllBytes($targetPath)
      $response.StatusCode = 200
      $response.ContentType = "application/json; charset=utf-8"
      $response.Headers["Cache-Control"] = "no-store, no-cache, must-revalidate, max-age=0"
      $response.Headers["Pragma"] = "no-cache"
      $response.Headers["Expires"] = "0"
      $response.AddHeader("X-File-Name", [System.IO.Path]::GetFileName($targetPath))
      $response.ContentLength64 = $jsonBytes.Length
      $response.OutputStream.Write($jsonBytes, 0, $jsonBytes.Length)
      continue
    }

    if ($request.HttpMethod -eq "GET" -and $path -eq "/api/draft") {
      if (-not (Test-Path -LiteralPath $draftPath)) {
        Write-TextResponse $response 404 "application/json; charset=utf-8" '{"ok":false,"error":"Draft file was not found"}'
        continue
      }

      $draftBytes = [System.IO.File]::ReadAllBytes($draftPath)
      $response.StatusCode = 200
      $response.ContentType = "application/json; charset=utf-8"
      $response.Headers["Cache-Control"] = "no-store, no-cache, must-revalidate, max-age=0"
      $response.Headers["Pragma"] = "no-cache"
      $response.Headers["Expires"] = "0"
      $response.ContentLength64 = $draftBytes.Length
      $response.OutputStream.Write($draftBytes, 0, $draftBytes.Length)
      continue
    }

    if ($request.HttpMethod -eq "GET" -and $path -eq "/api/stack-rules") {
      $stackRulesJson = (Get-StackRules) | ConvertTo-Json -Depth 6
      Write-TextResponse $response 200 "application/json; charset=utf-8" $stackRulesJson
      continue
    }

    if ($request.HttpMethod -eq "POST" -and $path -eq "/api/config") {
      $reader = [System.IO.StreamReader]::new($request.InputStream, $request.ContentEncoding)
      $rawBody = $reader.ReadToEnd()
      $reader.Dispose()

      $payload = $rawBody | ConvertFrom-Json
      $defaultConfig = Get-DefaultConfig
      $nextConfig = New-EditorConfig @(
        $payload,
        (Read-JsonFile $localConfigPath),
        (Read-JsonFile $defaultConfigPath)
      )

      if (Test-SameConfig $nextConfig $defaultConfig) {
        Remove-LocalEditorConfig
      } else {
        Save-LocalEditorConfig $nextConfig
      }

      $configJson = $nextConfig | ConvertTo-Json -Depth 4
      Write-TextResponse $response 200 "application/json; charset=utf-8" $configJson
      continue
    }

    if ($request.HttpMethod -eq "POST" -and $path -eq "/api/stack-rules") {
      $reader = [System.IO.StreamReader]::new($request.InputStream, $request.ContentEncoding)
      $rawBody = $reader.ReadToEnd()
      $reader.Dispose()

      $payload = $rawBody | ConvertFrom-Json
      Save-StackRules $payload

      $stackRulesJson = (Get-StackRules) | ConvertTo-Json -Depth 6
      Write-TextResponse $response 200 "application/json; charset=utf-8" $stackRulesJson
      continue
    }

    if ($request.HttpMethod -eq "POST" -and $path -eq "/api/save") {
      $reader = [System.IO.StreamReader]::new($request.InputStream, $request.ContentEncoding)
      $rawBody = $reader.ReadToEnd()
      $reader.Dispose()

      $payload = $rawBody | ConvertFrom-Json
      $config = Get-EditorConfig
      $targetPath = Resolve-ConfigPath ([string]$config.savePath) "Silver_77_Quests.json"
      if ([string]::IsNullOrWhiteSpace($targetPath)) {
        $targetName = [string]$payload.fileName
        if ([string]::IsNullOrWhiteSpace($targetName)) {
          throw "savePath is not configured"
        }

        $targetFile = [System.IO.Path]::GetFileName($targetName)
        $targetPath = Join-Path $scriptDir $targetFile
      }

      $targetDirectory = Split-Path -Parent $targetPath
      if ($targetDirectory -and -not (Test-Path -LiteralPath $targetDirectory)) {
        New-Item -ItemType Directory -Force -Path $targetDirectory | Out-Null
      }

      $jsonText = [string]$payload.json
      if ([string]::IsNullOrWhiteSpace($jsonText)) {
        throw "json is required"
      }

      [void]($jsonText | ConvertFrom-Json)
      [System.IO.File]::WriteAllText($targetPath, $jsonText, [System.Text.UTF8Encoding]::new($false))

      $saveResult = @{
        ok = $true
        savedTo = $targetPath
      } | ConvertTo-Json -Depth 4
      Write-TextResponse $response 200 "application/json; charset=utf-8" $saveResult
      continue
    }

    if ($request.HttpMethod -eq "POST" -and $path -eq "/api/draft") {
      $reader = [System.IO.StreamReader]::new($request.InputStream, $request.ContentEncoding)
      $rawBody = $reader.ReadToEnd()
      $reader.Dispose()

      [void]($rawBody | ConvertFrom-Json)
      [System.IO.File]::WriteAllText($draftPath, $rawBody, [System.Text.UTF8Encoding]::new($false))

      Write-TextResponse $response 200 "application/json; charset=utf-8" '{"ok":true}'
      continue
    }

    if ($request.HttpMethod -eq "DELETE" -and $path -eq "/api/draft") {
      if (Test-Path -LiteralPath $draftPath) {
        Remove-Item -LiteralPath $draftPath -Force
      }

      Write-TextResponse $response 200 "application/json; charset=utf-8" '{"ok":true}'
      continue
    }

    if ($request.HttpMethod -ne "GET") {
      Write-TextResponse $response 405 "text/plain; charset=utf-8" "Method not allowed"
      continue
    }

    $safePath = Resolve-SafePath $path
    if (-not $safePath -or -not (Test-Path -LiteralPath $safePath)) {
      Write-TextResponse $response 404 "text/plain; charset=utf-8" "Not found"
      continue
    }

    Write-FileResponse $response $safePath
  } catch {
    $errorJson = @{
      ok = $false
      error = $_.Exception.Message
    } | ConvertTo-Json -Depth 4
    Write-TextResponse $response 500 "application/json; charset=utf-8" $errorJson
  } finally {
    $response.OutputStream.Close()
  }
}
