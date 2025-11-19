#Makefile by PistonHead2k

#--Configuration-----------------------------------------------------------------------------------#

#Where ELF, Obj, Etc are stored
FOLDER = ELF

#-mmcu means Microcontroler I.E. -mmcu=atmega328p, ...2560..., etc
MCU  = atmega328
#-O means Optimization I.E. -O0, -O1, -O2, -O3, -Os.
OPTM = -O3
#-l means for Library I.E. -lm for math library.
LIBS = -lm -lc

#-P means PORT. in Windows: COM1, COM2, COM3, etc.
PORT = COM15
#-b means Port Speed in Bauds.
BAUD = 115200

.PHONY: rs232

#------------#
defualt: burn
#------------#


#--C----------------------------------------------------------------------------------------------#
buildc:
	avr-gcc $(OPTM) -fno-exceptions -mmcu=$(MCU) main.c -o $(FOLDER)/main.elf $(LIBS)

burnc: buildc
	avr-objcopy -O ihex -R .eeprom $(FOLDER)/main.elf $(FOLDER)/main.hex

	avrdude -F -V -c arduino -p m328p -P $(PORT) -b $(BAUD) -U $(FOLDER)/main.hex

#-------------------------------------------------------------------------------------------------#

#--C++--------------------------------------------------------------------------------------------#
build:
	avr-c++ $(OPTM) -finput-charset=UTF-8 -fno-exceptions -mmcu=$(MCU) main.cpp -o $(FOLDER)/main.elf $(LIBS)

burn: build
	
	avr-objcopy -O ihex -R .eeprom $(FOLDER)/main.elf $(FOLDER)/main.hex

	avrdude -F -V -c arduino -p m328p -P $(PORT) -b $(BAUD) -U $(FOLDER)/main.hex

	$(MAKE) rs232

#-------------------------------------------------------------------------------------------------#

#--C++ Pedantic Build-----------------------------------------------------------------------------#
#Pedantic Build
test:
	avr-c++ -Wall -Werror -Wpedantic $(OPTM) -fno-exceptions -mmcu=$(MCU) main.cpp -o $(FOLDER)/main.elf

#-------------------------------------------------------------------------------------------------#

#--Size-------------------------------------------------------------------------------------------#
size:
	avr-size -C --mcu=$(MCU) $(FOLDER)/main.elf
	
#-------------------------------------------------------------------------------------------------#

#--rs232--------------------------------------------------------------------------------------------------------------------------------------------------------------#
MODE = RECEIVE
IOBAUD = 250000
PARITY = ODD
DATA = 8
STOP = 2

rs232: 
	powershell -executionpolicy bypass -NoProfile -File "USART.ps1" -Mode $(MODE) -Port $(PORT) -Baud $(IOBAUD) -Parity $(PARITY) -Data $(DATA) -Stop $(STOP)

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------#