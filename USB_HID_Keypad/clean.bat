@echo off
@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@echo @                                        @
@echo @ USB HID Keypad Project clean           @
@echo @ Author: Aung Tun Linn                  @
@echo @ Date  : 20/May/2023                    @
@echo @                                        @
@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

set folder="build"
cd /d %folder%
for /F "delims=" %%i in ('dir /b') do (rmdir "%%i" /s/q || del "%%i" /s/q)

::rmdir /s /q build