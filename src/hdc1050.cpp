/*
 * HDC1050 Temperature/Humidity Sensor Library
 * Kerry D. Wong
 * http://www.kerrywong.com
 * 10/2015
 */

#include "hdc1050.h"
#include <Wire.h>

HDC1050::HDC1050()
{
	configReg = 0x10; //POR default
}

void HDC1050::readRegister(byte regAddr, byte numOfBytes)
{
	Wire.beginTransmission(Addr);
	Wire.send(regAddr);
	Wire.endTransmission();

	if (regAddr == REG_Temperature || regAddr == REG_Humidity) {
		delay(50);
	}

	Wire.requestFrom(Addr, numOfBytes);
	
	while (!Wire.available());
	
	for (int i=0; i < numOfBytes; i++) {
		buf[i] = Wire.receive();
	}
}

unsigned int HDC1050::getManufacturerID()
{
	readRegister(REG_ManufactureID, 2);
	return buf[0] << 8 | buf[1];
}

unsigned int HDC1050::getDeviceID()
{
	readRegister(REG_DeviceID, 2);
	return buf[0] << 8 | buf[1];
}

char* HDC1050::getSerialID()
{
    static char s[4*3+1];
	readRegister(REG_SER_1, 2);
	sprintf(s, "%04x", buf[0] << 8 | buf[1]);
	readRegister(REG_SER_2, 2);
	sprintf(s + 4, "%04x", buf[0] << 8 | buf[1]);
	readRegister(REG_SER_3, 2);
	sprintf(s + 8, "%04x", buf[0] << 8 | buf[1]);
    s[sizeof(s)] = 0;
	
	return s;
}

uint32_t HDC1050::getU32SerialID()
{
    uint32_t id;

	readRegister(REG_SER_2, 2);
	id = buf[0] << 8 | buf[1];
    id <<= 16;
	readRegister(REG_SER_3, 2);
    id |= (buf[0] << 8 | buf[1]) & 0xffffUL;
	
	return id;
}

void HDC1050::updateConfigRegister()
{
	Wire.beginTransmission(Addr);
	Wire.send(REG_Config);
	Wire.send(configReg);
	Wire.send(byte(0x00));
	Wire.endTransmission();
}

void HDC1050::setTemperatureRes(byte res) 
{
	if (res > 1) res = T_RES_14;
	
	configReg &= ~(1 <<  BIT_T_RES);
	configReg |= res << BIT_T_RES;	
}

void HDC1050::setHumidityRes(byte res) 
{
	if (res > 2) res = H_RES_14;
	
	configReg &= ~(0x3 << BIT_H_RES);
	configReg |= res << BIT_H_RES;
}

void HDC1050::turnOnHeater(bool heaterOn) 
{
	if (heaterOn) 
		configReg |= 1 << BIT_HEATER;
	else
		configReg &= ~(1 << BIT_HEATER);
}

bool HDC1050::batteryOK()
{
	readRegister(REG_Config, 2);
	configReg = buf[0];
	
	return (configReg & (1 << BIT_BATTERY_OK)) == 0;
}

void HDC1050::getTemperatureHumidity(int8_t &t, uint8_t &h)
{
	readRegister(REG_Temperature, 4);
	unsigned int th, tl, hh, hl;

	th = buf[0];
	tl = buf[1];
	hh = buf[2];
	hl = buf[3];

	t =  (int8_t)((float)(th << 8 | tl) * 165.0 / 65536.0 - 40.0);
	h = (uint8_t)((float)(hh << 8 | hl) * 100.0 / 65536.0);
}

