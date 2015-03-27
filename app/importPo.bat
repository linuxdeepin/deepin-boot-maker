@ECHO off

SET PATH=D:\Dev\QtStatic\5.3\vs2010\bin\;%PATH%

lupdate.exe %1

SETLOCAL enabledelayedexpansion
for /f "delims=" %%i in ('dir /b/a-d/s po\*.ts') do (
    SET TsFile=%%i
    ECHO !TsFile!
    SET LangFile=!TsFile:~0,-3!
    ECHO !LangFile!
    lconvert.exe -i !LangFile!.po -if po -o !LangFile!.ts -of ts
)

lrelease.exe %1
