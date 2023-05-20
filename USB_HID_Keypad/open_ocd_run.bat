@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@echo @                                        @
@echo @ Open OCD Running                       @
@echo @                                        @
@echo @                                        @
@echo @                                        @
@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

cd D:\VSARM\sdk\pico\openocd-branch-rp2040-f8w14ec-windows\src\
D:
START openocd.exe -s ../tcl -f interface/picoprobe.cfg -f target/rp2040.cfg
