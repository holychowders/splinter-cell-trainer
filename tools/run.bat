@echo off

if not exist build mkdir build
tools\build.bat && .\sct.exe
