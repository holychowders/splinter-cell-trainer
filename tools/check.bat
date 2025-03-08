@echo off
setlocal enabledelayedexpansion 

for /R %%G in (code\*.c, code\*.h) do (
    set check_cmd=clang-tidy -quiet -config-file="tools/.clang-tidy" "%%G" -- -std=c99
    echo ---------------------------------------------------------------------------------------------------------------
    echo !check_cmd!
    !check_cmd!
)

endlocal
