@echo off
setlocal
chcp 65001 >nul

set "ROOT_DIR=%~dp0"
set "PROJECT_DIR=%ROOT_DIR:~0,-1%"
set "ADDON_BUILDER=D:\SteamLibrary\steamapps\common\DayZ Tools\Bin\AddonBuilder\AddonBuilder.exe"
set "SIGN_KEY=P:\keys\Silver777.biprivatekey"
set "SOURCE_DIR=%ROOT_DIR%Silver_77_Migrate_server"
set "MOD_DIR=%ROOT_DIR%..\Mods_DONE\@Silver_77_Migrate_server"
set "ADDONS_DIR=%MOD_DIR%\addons"
set "SUPPORT_DIR=%MOD_DIR%\Support"
set "CANONICAL_CONFIG=%SOURCE_DIR%\config\MigrationConfig.json"
set "SUPPORT_CONFIG=%SUPPORT_DIR%\MigrationConfig.json"
set "BUILD_LOG=%TEMP%\silver77_migrate_server_build.log"
set "BUILD_TEMP=%TEMP%\silver77_migrate_server_addonbuilder"
set "PBO_NAME=Silver_77_Migrate_server"

if not exist "%ADDON_BUILDER%" (
    echo [ERROR] Addon Builder not found:
    echo %ADDON_BUILDER%
    exit /b 1
)

if not exist "%SIGN_KEY%" (
    echo [ERROR] Signing key not found:
    echo %SIGN_KEY%
    exit /b 1
)

if not exist "%SOURCE_DIR%" (
    echo [ERROR] Source directory not found:
    echo %SOURCE_DIR%
    exit /b 1
)

if not exist "%CANONICAL_CONFIG%" (
    echo [ERROR] Canonical MigrationConfig.json not found:
    echo %CANONICAL_CONFIG%
    exit /b 1
)

if not exist "%MOD_DIR%" mkdir "%MOD_DIR%"
if not exist "%MOD_DIR%" (
    echo [ERROR] Cannot create mod output directory:
    echo %MOD_DIR%
    exit /b 1
)

if not exist "%ADDONS_DIR%" mkdir "%ADDONS_DIR%"
if not exist "%ADDONS_DIR%" (
    echo [ERROR] Cannot create addons output directory:
    echo %ADDONS_DIR%
    exit /b 1
)

if not exist "%BUILD_TEMP%" mkdir "%BUILD_TEMP%"
if not exist "%BUILD_TEMP%" (
    echo [ERROR] Cannot create Addon Builder temporary directory:
    echo %BUILD_TEMP%
    exit /b 1
)

echo Building %PBO_NAME%.pbo...
"%ADDON_BUILDER%" "%SOURCE_DIR%" "%ADDONS_DIR%" -packonly -clear -temp="%BUILD_TEMP%" -project="%PROJECT_DIR%" -prefix=%PBO_NAME% -sign="%SIGN_KEY%" > "%BUILD_LOG%" 2>&1
set "BUILD_EXIT=%ERRORLEVEL%"
type "%BUILD_LOG%"

if not "%BUILD_EXIT%"=="0" (
    echo [ERROR] Addon Builder exited with code %BUILD_EXIT%.
    exit /b %BUILD_EXIT%
)

findstr /C:"[FATAL]" "%BUILD_LOG%" >nul
if not errorlevel 1 (
    echo [ERROR] Addon Builder reported a fatal error.
    exit /b 1
)

if not exist "%ADDONS_DIR%\%PBO_NAME%.pbo" (
    echo [ERROR] Expected PBO was not created:
    echo %ADDONS_DIR%\%PBO_NAME%.pbo
    exit /b 1
)

if not exist "%ADDONS_DIR%\%PBO_NAME%.pbo.Silver777.bisign" (
    echo [ERROR] Expected PBO signature was not created:
    echo %ADDONS_DIR%\%PBO_NAME%.pbo.Silver777.bisign
    exit /b 1
)

if not exist "%SUPPORT_DIR%" mkdir "%SUPPORT_DIR%"
if not exist "%SUPPORT_DIR%" (
    echo [ERROR] Cannot create Support directory:
    echo %SUPPORT_DIR%
    exit /b 1
)

copy /Y "%CANONICAL_CONFIG%" "%SUPPORT_CONFIG%" >nul
if errorlevel 1 (
    echo [ERROR] Cannot copy canonical MigrationConfig.json to Support.
    exit /b 1
)

fc /B "%CANONICAL_CONFIG%" "%SUPPORT_CONFIG%" >nul
if errorlevel 1 (
    echo [ERROR] Support MigrationConfig.json differs from canonical source.
    exit /b 1
)

echo [OK] PBO build completed.
echo [OK] Support config refreshed:
echo %SUPPORT_CONFIG%
exit /b 0
