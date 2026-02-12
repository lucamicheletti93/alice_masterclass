@echo off

REM === Visual Studio x64 environment ===
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64

REM === ROOT environment ===
call C:\ROOT\bin\thisroot.bat

REM === Vai nella directory del progetto ===
cd /d C:\alice_masterclass

REM === Avvia ROOT ed esegui la macro ===
root --web=off masterclass.C