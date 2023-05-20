echo off
@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@echo @                                        @
@echo @ USB HID Keypad Project Build           @
@echo @ Author: Aung Tun Linn                  @
@echo @ Date  : 20/May/2023                    @
@echo @                                        @
@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

if not exist build\ (
	@echo create build folder
	md build
)
cd build
cmake -G "MinGW Makefiles" ..
make
