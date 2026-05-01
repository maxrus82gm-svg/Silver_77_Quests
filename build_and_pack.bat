@echo off
chcp 65001 >nul
echo ========================================
echo Silver_77_Quests - Сборка мода
echo ========================================
echo.

REM Путь к DayZ Tools (измени если нужно)
set "DAYZ_TOOLS=C:\Program Files (x86)\Steam\steamapps\common\DayZ Tools\Bin\AddonBuilder"

REM Путь к исходникам (текущая папка)
set "SOURCE_DIR=%~dp0"
set "PROJECT_DIR=%SOURCE_DIR:~0,-1%"

REM Путь к готовому моду (создастся автоматически)
set "DEST_DIR=%~dp0..\Mods_DONE\@Silver_77_Quests"

REM Имя PBO
set "PBO_NAME=Silver_77_Quests"

echo Исходники: %SOURCE_DIR%
echo Назначение: %DEST_DIR%
echo.

REM Создаём структуру готового мода
if not exist "%DEST_DIR%" mkdir "%DEST_DIR%"
if not exist "%DEST_DIR%\addons" mkdir "%DEST_DIR%\addons"
if not exist "%DEST_DIR%\key" mkdir "%DEST_DIR%\key"
if not exist "%DEST_DIR%\Documentation" mkdir "%DEST_DIR%\Documentation"

REM Копируем mod.cpp и README
echo Копирование mod.cpp и README...
copy /Y "%SOURCE_DIR%mod.cpp" "%DEST_DIR%\" >nul
copy /Y "%SOURCE_DIR%README.md" "%DEST_DIR%\" >nul

REM Копируем документацию
echo Копирование документации...
xcopy /Y /E /I "%SOURCE_DIR%Documentation\*" "%DEST_DIR%\Documentation\" >nul

REM Копируем README из key
if exist "%SOURCE_DIR%key\README.txt" (
    copy /Y "%SOURCE_DIR%key\README.txt" "%DEST_DIR%\key\" >nul
)

echo.
echo ========================================
echo Запуск Addon Builder...
echo ========================================
echo.

if not exist "%DAYZ_TOOLS%\AddonBuilder.exe" (
    echo [ОШИБКА] AddonBuilder.exe не найден:
    echo %DAYZ_TOOLS%\AddonBuilder.exe
    echo.
    echo Исправь переменную DAYZ_TOOLS в build_and_pack.bat.
    pause
    exit /b 1
)

REM Запускаем Addon Builder
"%DAYZ_TOOLS%\AddonBuilder.exe" "%SOURCE_DIR%" "%DEST_DIR%\addons" -clear -project="%PROJECT_DIR%" -prefix=%PBO_NAME% -exclude="%SOURCE_DIR%exclude.lst"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo [OK] Сборка завершена успешно!
    echo ========================================
    echo.
    echo Готовый мод: %DEST_DIR%
    echo PBO файл: %DEST_DIR%\addons\%PBO_NAME%.pbo
    echo.
) else (
    echo.
    echo ========================================
    echo [ОШИБКА] Сборка завершилась с ошибкой!
    echo ========================================
    echo.
)

pause
