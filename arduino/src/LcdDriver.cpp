#include "LcdDriver.h"
#include <SPI.h>

LcdDriver::LcdDriver(uint8_t resetPin, uint8_t dcPin)
	: _resetPin(resetPin), _dcPin(dcPin)
{
}

void LcdDriver::writeByte(uint8_t da)
{
	digitalWrite(_dcPin, 1);
	SPI.transfer(da);
}

void LcdDriver::writeWord(uint16_t da)
{
	digitalWrite(_dcPin, 1);
	SPI.transfer(da >> 8);
	SPI.transfer(da);
}

void LcdDriver::writeRegister(uint8_t da)
{
	digitalWrite(_dcPin, 0);
	SPI.transfer(da);
}

void LcdDriver::writeCommand(uint8_t cmd, uint8_t val)
{
	writeRegister(cmd);
	writeByte(val);
}

void LcdDriver::writeCommand(uint8_t cmd, uint8_t val1, uint8_t val2)
{
	writeRegister(cmd);
	writeByte(val1);
	writeByte(val2);
}

void LcdDriver::writeCommand(uint8_t cmd, uint8_t val1, uint8_t val2, uint8_t val3)
{
	writeRegister(cmd);
	writeByte(val1);
	writeByte(val2);
	writeByte(val3);
}
void LcdDriver::writeCommandLen(uint8_t cmd, const uint8_t *vals, uint8_t len)
{
	writeRegister(cmd);
	for (size_t i = 0; i < len; ++i)
	{
		writeByte(vals[i]);
	}
}

void LcdDriver::init()
{
	pinMode(_resetPin, OUTPUT);
	pinMode(_dcPin, OUTPUT);
	// spi
	SPI.setDataMode(SPI_MODE3);
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.begin();

	digitalWrite(_resetPin, 1);

	// reset
	delay(100);
	// digitalWrite(_resetPin,0);
	// delay(100);
	digitalWrite(_resetPin, 1);
	delay(100);

	// start initialization
	writeRegister(0xEF);

	writeCommand(0xEB, 0x14);

	writeRegister(0xFE);
	writeRegister(0xEF);
	writeCommand(0xEB, 0x14);
	writeCommand(0x84, 0x40);
	writeCommand(0x85, 0xFF);
	writeCommand(0x86, 0xFF);
	writeCommand(0x87, 0xFF);
	writeCommand(0x88, 0x0A);
	writeCommand(0x89, 0x21);
	writeCommand(0x8A, 0x00);
	writeCommand(0x8B, 0x80);
	writeCommand(0x8C, 0x01);
	writeCommand(0x8D, 0x01);
	writeCommand(0x8E, 0xFF);
	writeCommand(0x8F, 0xFF);

	writeCommand(0xB6, 0x00, 0x20);
	writeCommand(0x36, 0x08);
	writeCommand(0x3A, 0x05);

	writeRegister(0x90);
	writeByte(0x08);
	writeByte(0x08);
	writeByte(0x08);
	writeByte(0x08);

	writeCommand(0xBD, 0x06);

	writeCommand(0xBC, 0x00);

	writeCommand(0xFF, 0x60, 0x01, 0x04);

	writeCommand(0xC3, 0x13);
	writeCommand(0xC4, 0x13);

	writeCommand(0xC9, 0x22);
	writeCommand(0xBE, 0x11);

	writeCommand(0xE1, 0x10, 0x0E);
	writeCommand(0xDF, 0x21, 0x0c, 0x02);

	writeRegister(0xF0);
	writeByte(0x45);
	writeByte(0x09);
	writeByte(0x08);
	writeByte(0x08);
	writeByte(0x26);
	writeByte(0x2A);

	writeRegister(0xF1);
	writeByte(0x43);
	writeByte(0x70);
	writeByte(0x72);
	writeByte(0x36);
	writeByte(0x37);
	writeByte(0x6F);

	writeRegister(0xF2);
	writeByte(0x45);
	writeByte(0x09);
	writeByte(0x08);
	writeByte(0x08);
	writeByte(0x26);
	writeByte(0x2A);

	writeRegister(0xF3);
	writeByte(0x43);
	writeByte(0x70);
	writeByte(0x72);
	writeByte(0x36);
	writeByte(0x37);
	writeByte(0x6F);

	writeRegister(0xED);
	writeByte(0x1B);
	writeByte(0x0B);

	writeRegister(0xAE);
	writeByte(0x77);

	writeRegister(0xCD);
	writeByte(0x63);

	writeRegister(0x70);
	writeByte(0x07);
	writeByte(0x07);
	writeByte(0x04);
	writeByte(0x0E);
	writeByte(0x0F);
	writeByte(0x09);
	writeByte(0x07);
	writeByte(0x08);
	writeByte(0x03);

	writeRegister(0xE8);
	writeByte(0x34);

	uint8_t v1[] = {0x18, 0x0D, 0x71, 0xED, 0x70, 0x70, 0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70};
	writeCommandLen(0x62, v1, sizeof(v1));

	uint8_t v2[] = {0x18, 0x11, 0x71, 0xF1, 0x70, 0x70, 0x18, 0x13, 0x71, 0xF3, 0x70, 0x70};
	writeCommandLen(0x63, v2, sizeof(v2));

	uint8_t v3[] = {0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07};
	writeCommandLen(0x64, v3, sizeof(v3));

	uint8_t v4[] = {0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00};
	writeCommandLen(0x66, v4, sizeof(v4));

	writeRegister(0x67);
	writeByte(0x00);
	writeByte(0x3C);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0x01);
	writeByte(0x54);
	writeByte(0x10);
	writeByte(0x32);
	writeByte(0x98);

	writeRegister(0x74);
	writeByte(0x10);
	writeByte(0x85);
	writeByte(0x80);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0x4E);
	writeByte(0x00);

	writeCommand(0x98, 0x3e, 0x07);

	writeRegister(0x35);
	writeRegister(0x21);

	writeRegister(0x11);
	delay(120);
	writeRegister(0x29);
	delay(20);
}

void LcdDriver::drawArea(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
	writeRegister(0x2a);
	writeByte(0x00);
	writeByte(xs);
	writeByte(0x00);
	writeByte(xe);

	writeRegister(0x2b);
	writeByte(0x00);
	writeByte(ys);
	writeByte(0x00);
	writeByte(ye);

	writeRegister(0x2c);
};
