@echo off
chcp 65001 >nul
echo ========================================
echo Silver_77_Quests - split client/server build
echo ========================================
echo.

set "DAYZ_TOOLS=D:\SteamLibrary\steamapps\common\DayZ Tools\Bin\AddonBuilder"
set "ROOT_DIR=%~dp0"
set "PROJECT_DIR=%ROOT_DIR:~0,-1%"
set "CLIENT_SOURCE=%ROOT_DIR%Silver_77_Quests_Client"
set "SERVER_SOURCE=%ROOT_DIR%Silver_77_Quests_Server"
set "CLIENT_DEST=%ROOT_DIR%..\Mods_DONE\@Silver_77_Quests_Client"
set "SERVER_DEST=%ROOT_DIR%..\Mods_DONE\@Silver_77_Quests_Server"
set "CLIENT_BUILD_LOG=%TEMP%\silver77_quests_client_build.log"
set "SERVER_BUILD_LOG=%TEMP%\silver77_quests_server_build.log"

if not exist "%DAYZ_TOOLS%\AddonBuilder.exe" (
    echo [ERROR] AddonBuilder.exe not found:
    echo %DAYZ_TOOLS%\AddonBuilder.exe
    echo.
    echo Edit DAYZ_TOOLS in build_split_mods.bat.
    pause
    exit /b 1
)

call :PrepareMod "%CLIENT_SOURCE%" "%CLIENT_DEST%"
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

call :PrepareMod "%SERVER_SOURCE%" "%SERVER_DEST%"
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

echo.
echo ========================================
echo Building client PBO
echo ========================================
"%DAYZ_TOOLS%\AddonBuilder.exe" "%CLIENT_SOURCE%" "%CLIENT_DEST%\addons" -clear -project="%PROJECT_DIR%" -prefix=Silver_77_Quests -exclude="%ROOT_DIR%exclude.lst" > "%CLIENT_BUILD_LOG%" 2>&1
type "%CLIENT_BUILD_LOG%"
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Client build failed.
    pause
    exit /b %ERRORLEVEL%
)
findstr /C:"[FATAL]" "%CLIENT_BUILD_LOG%" >nul
if %ERRORLEVEL% EQU 0 (
    echo [ERROR] Client build reported a fatal AddonBuilder error.
    pause
    exit /b 1
)

echo.
echo ========================================
echo Building server PBO
echo ========================================
"%DAYZ_TOOLS%\AddonBuilder.exe" "%SERVER_SOURCE%" "%SERVER_DEST%\addons" -clear -project="%PROJECT_DIR%" -prefix=Silver_77_Quests_Server -exclude="%ROOT_DIR%exclude.lst" > "%SERVER_BUILD_LOG%" 2>&1
type "%SERVER_BUILD_LOG%"
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Server build failed.
    pause
    exit /b %ERRORLEVEL%
)
findstr /C:"[FATAL]" "%SERVER_BUILD_LOG%" >nul
if %ERRORLEVEL% EQU 0 (
    echo [ERROR] Server build reported a fatal AddonBuilder error.
    pause
    exit /b 1
)

echo.
echo ========================================
echo [OK] Split build finished
echo ========================================
echo Client mod: %CLIENT_DEST%
echo Server mod: %SERVER_DEST%
echo.
echo Server launch example:
echo -mod=@Silver_77_Quests_Client
echo -serverMod=@Silver_77_Quests_Server
echo.
pause
exit /b 0

:PrepareMod
set "SRC=%~1"
set "DST=%~2"

if not exist "%SRC%" (
    echo [ERROR] Source folder not found: %SRC%
    exit /b 1
)

if not exist "%DST%" mkdir "%DST%"
if not exist "%DST%\addons" mkdir "%DST%\addons"
if not exist "%DST%\key" mkdir "%DST%\key"

if exist "%SRC%\mod.cpp" copy /Y "%SRC%\mod.cpp" "%DST%\" >nul
if exist "%SRC%\README.md" copy /Y "%SRC%\README.md" "%DST%\" >nul
if exist "%SRC%\key\README.txt" copy /Y "%SRC%\key\README.txt" "%DST%\key\" >nul

exit /b 0
