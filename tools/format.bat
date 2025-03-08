@echo off

for /R %%G in (*.c, *.h) do (
    echo Formatting: %%G
    clang-format -i -style=file:"tools/.clang-format" "%%G"
)

