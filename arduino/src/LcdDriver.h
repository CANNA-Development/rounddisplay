#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include "DisplayDriver.h"


class LcdDriver : public DisplayDriver
{
public:	
	LcdDriver(uint8_t resetPin, uint8_t dcPin);
	void init();
	void drawArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
	void write(uint16_t color) {writeWord(color);};	
	uint16_t height() const {return LCD_WIDTH;};
	uint16_t width() const {return LCD_HEIGHT;};

private:
	static const uint16_t LCD_WIDTH =  240;
	static const uint16_t LCD_HEIGHT =  240;

	void writeRegister(uint8_t da);
	void writeWord(uint16_t da);
	void writeByte(uint8_t da); 

	void writeCommand(uint8_t cmd, uint8_t val1); 
	void writeCommand(uint8_t cmd, uint8_t val1, uint8_t val2);
	void writeCommand(uint8_t cmd, uint8_t val1, uint8_t val2, uint8_t val3); 
    void writeCommandLen(uint8_t cmd, const uint8_t* vals, uint8_t len);

	uint8_t _resetPin;
	uint8_t _dcPin;
};


#endif

