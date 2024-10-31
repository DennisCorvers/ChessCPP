@echo off
setlocal

rem Define output file
set "output_file=file_list.txt"

rem Clear the output file if it already exists
> "%output_file%" echo.

rem Loop through each file in current and subdirectories
for /R %%f in (*.cpp *.c) do (
    rem Get the relative path by removing the root path
    set "filepath=%%f"
    setlocal enabledelayedexpansion
    set "relpath=!filepath:%cd%\=!"
    echo !relpath! >> "%output_file%"
    endlocal
)

echo File list written to %output_file%
endlocal
pause
