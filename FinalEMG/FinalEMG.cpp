#include "FinalEMG.h"

/*Global Variables*/
volatile int16_t channel_data[4] = {0, 0, 0, 0};

ISR(TIMER1_COMPA_vect)
{
	channel_data[0] = SendConvertCommand(0);
	channel_data[1] = SendConvertCommand(1);
	channel_data[2] = SendConvertCommand(2);
	channel_data[3] = SendConvertCommand(3);
}

uint16_t SendReadCommand(uint8_t regnum) 
{
	uint16_t mask = regnum << 8;
	mask = 0b1100000000000000 | mask;
	digitalWrite(chipSelectPin, LOW);
	uint16_t out = SPI.transfer16(mask);
	digitalWrite(chipSelectPin, HIGH);

	return out;
}

uint16_t SendConvertCommand(uint8_t channelnum) 
{
	uint16_t mask = channelnum << 8;
	mask = 0b0000000000000000 | mask;
	digitalWrite(chipSelectPin, LOW);
	uint16_t out = SPI.transfer16(mask);
	digitalWrite(chipSelectPin, HIGH);

	return out;
}

uint16_t SendConvertCommandH(uint8_t channelnum) 
{
	uint16_t mask = channelnum << 8;
	mask = 0b0000000000000001 | mask;
	digitalWrite(chipSelectPin, LOW);
	uint16_t out = SPI.transfer16(mask);
	digitalWrite(chipSelectPin, HIGH);

	return out;
}

uint16_t SendWriteCommand(uint8_t regnum, uint8_t data) 
{
	uint16_t mask = regnum << 8;
	mask = 0b1000000000000000 | mask | data;
	digitalWrite(chipSelectPin, LOW);
	uint16_t out = SPI.transfer16(mask);
	digitalWrite(chipSelectPin, HIGH);

	return out;
}


void Calibrate() 
{
	digitalWrite(chipSelectPin, LOW);
	SPI.transfer16(0b0101010100000000);
	digitalWrite(chipSelectPin, HIGH);
	int i = 0;
	for (i = 0; i < 9; i++) 
	{
		SendReadCommand(40);
	}
}
