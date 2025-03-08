@echo off

if not exist build mkdir build
clang -std=c99 -pedantic -g -Wall -Wextra code\main.c -o build\sct.exe && copy build\sct.exe . && .\sct.exe
