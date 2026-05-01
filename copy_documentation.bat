@echo off
chcp 65001 >nul
echo ========================================
echo Копирование документации
echo ========================================
echo.

set DEST_DIR=%~dp0..\Mods_DONE\@Silver_77_Quests

if not exist "%DEST_DIR%\Documentation" mkdir "%DEST_DIR%\Documentation"

echo Копирование файлов...
xcopy /Y /E /I "%~dp0Documentation\*" "%DEST_DIR%\Documentation\" >nul
copy /Y "%~dp0README.md" "%DEST_DIR%\" >nul
copy /Y "%~dp0mod.cpp" "%DEST_DIR%\" >nul

echo.
echo [OK] Документация скопирована в:
echo %DEST_DIR%\Documentation
echo.
pause
