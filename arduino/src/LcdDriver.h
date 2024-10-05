/*****************************************************************************
* | File        :   LCD_Driver.h
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* | This version:   V1.0
* | Date        :   2020-12-09
* | Info        :   
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H

#include "DEV_Config.h"
#include "DisplayDriver.h"


class LcdDriver : public DisplayDriver
{
public:	
	LcdDriver();
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

};

#ifdef ASIM
	extern UWORD asim_buffer[240*240];
#endif

#endif

