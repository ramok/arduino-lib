import sys, time
import serial
import os

# We can use this script for resetting Arduino board. But unfortunately 
# this does not work with non-modified avrdude for which I need manual reset 
# to upload properly.

# Note. I'm using ArduPilot Mega board with Oilpan.

# TODO: Make it work with non-modified avrdude.
# TODO: Make it cross-platform.

# port = "COM7:" # Windows
port_name = "/dev/ttyUSB0" # Linux
pulse_width = 0.1


def main(argv):
	port = serial.Serial(port_name)

	port.setDTR(True)
	time.sleep(pulse_width)
	port.setDTR(False)

	port.close()
	
sys.exit(main(sys.argv))
