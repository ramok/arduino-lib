avr-gcc  -I../include/EEPROM -I../include/Ethernet -I../include/Ethernet/utility -I../include/Firmata -I../include/LiquidCrystal -I../include/Matrix -I../include/SD -I../include/SD/utility -I../include/SPI -I../include/Servo -I../include/Sprite -I../include/Stepper -I../include/SoftwareSerial -I../include/Wire -I../include/Wire/utility -I../include -DF_CPU=16000000L -c -Os -mmcu=atmega328p ../src/EEPROM/*.cpp ../src/Ethernet/*.cpp ../src/Ethernet/utility/*.cpp ../src/Firmata/*.cpp ../src/LiquidCrystal/*.cpp ../src/Matrix/*.cpp ../src/SD/*.cpp ../src/SD/utility/*.cpp ../src/Sprite/*.cpp ../src/SPI/*.cpp ../src/Servo/*.cpp ../src/Stepper/*.cpp ../src/SoftwareSerial/*.cpp ../src/Wire/*.cpp ../src/Wire/utility/*.C ../src/*.c ../src/*.cpp  
avr-ar rcs libarduino.a *.o
rm *.o

