@echo off
setlocal EnableExtensions DisableDelayedExpansion
rem Brisk Emerald safe two-profile launcher for Windows.
rem Each profile has its own complete 128 KiB .sav; the ROM is copied, not patched.
rem Important: mGBA/VBA must be set to save beside the ROM, not a shared save folder.

set "ROM=%~1"
if not defined ROM (
  echo BRISK EMERALD - TWO INDEPENDENT SAVE PROFILES
  echo.
  echo Enter the FULL path to your Brisk Emerald .gba ROM.
  set /p "ROM=ROM path: "
)
set "ROM=%ROM:"=%"
if not exist "%ROM%" (
  echo ERROR: ROM file does not exist: "%ROM%"
  exit /b 1
)
for %%F in ("%ROM%") do (
  set "SRCROM=%%~fF"
  set "SOURCE_DIR=%%~dpF"
  set "ROM_NAME=%%~nxF"
  set "ROM_STEM=%%~nF"
  set "ROM_EXT=%%~xF"
)
if /I not "%ROM_EXT%"==".gba" (
  echo ERROR: Choose a .gba ROM, not an archive.
  exit /b 1
)

set "PROFILE_DIR=%SOURCE_DIR%Brisk Two Save Profiles"
set "SLOT1=%PROFILE_DIR%\Slot 1"
set "SLOT2=%PROFILE_DIR%\Slot 2"
if not exist "%SLOT1%" mkdir "%SLOT1%"
if not exist "%SLOT2%" mkdir "%SLOT2%"
if not exist "%SLOT1%" exit /b 1
if not exist "%SLOT2%" exit /b 1

rem Sync the latest game ROM to both isolated folders, leaving save data untouched.
copy /B /Y "%SRCROM%" "%SLOT1%\%ROM_NAME%" >nul
if errorlevel 1 goto :copy_failed
copy /B /Y "%SRCROM%" "%SLOT2%\%ROM_NAME%" >nul
if errorlevel 1 goto :copy_failed

rem One-time copy of preexisting Brisk save into Slot 1 ONLY.
rem Never erase, move, overwrite, or re-import a profile's existing save.
if exist "%SLOT1%\%ROM_STEM%.sav" goto :menu
if exist "%SLOT1%\%ROM_STEM%.srm" goto :menu
if exist "%SOURCE_DIR%%ROM_STEM%.sav" (
  copy /B "%SOURCE_DIR%%ROM_STEM%.sav" "%SLOT1%\%ROM_STEM%.sav" >nul
  if errorlevel 1 goto :copy_failed
  echo Imported original .sav into Slot 1. Original file was NOT changed.
  goto :menu
)
if exist "%SOURCE_DIR%%ROM_STEM%.srm" (
  copy /B "%SOURCE_DIR%%ROM_STEM%.srm" "%SLOT1%\%ROM_STEM%.srm" >nul
  if errorlevel 1 goto :copy_failed
  echo Imported original .srm into Slot 1. Original file was NOT changed.
)

:menu
echo.
echo =============================================
echo       BRISK EMERALD - CHOOSE SAVE SLOT
echo =============================================
echo 1. Slot 1 - existing adventure / migrated save
echo 2. Slot 2 - separate adventure
echo Q. Exit
echo.
echo IMPORTANT: Configure your emulator to store
echo save files beside each ROM, not in a shared
echo emulator save directory.
echo.
choice /C 12Q /N /M "Slot [1/2/Q]: "
if errorlevel 3 exit /b 0
if errorlevel 2 (
  set "SELECTED=%SLOT2%\%ROM_NAME%"
  set "SAVE_DIR=%SLOT2%"
) else (
  set "SELECTED=%SLOT1%\%ROM_NAME%"
  set "SAVE_DIR=%SLOT1%"
)

rem Optional emulator executable as second command line argument.
if not "%~2"=="" (
  if not exist "%~2" (
    echo ERROR: Emulator executable "%~2" was not found.
    exit /b 1
  )
  start "" "%~2" "%SELECTED%"
) else (
  start "" "%SELECTED%"
)
if errorlevel 1 (
  echo Could not launch the ROM. Associate .gba with your emulator,
  echo or pass the emulator executable as the second argument.
  exit /b 1
)
echo.
echo Started "%SELECTED%"
echo Save folder: "%SAVE_DIR%"
echo.
exit /b 0

:copy_failed
echo ERROR: Could not copy the ROM or import a save.
echo Existing source saves have NOT been deleted.
exit /b 1
