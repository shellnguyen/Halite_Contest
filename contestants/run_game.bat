@echo OFF
set CONTESTANTS=%~dp0
call halite -d "240 160" "call %CONTESTANTS%/%1/run.bat" "call %CONTESTANTS%/%2/run.bat"
