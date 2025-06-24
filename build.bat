@REM This needs to be ran with x86 Native Developer Tools using administrator privileges   

@echo off
cl /EHsc macro.cpp /Ilibrary library\interception.lib User32.lib /Fe:macro.exe /link /SUBSYSTEM:CONSOLE
pause