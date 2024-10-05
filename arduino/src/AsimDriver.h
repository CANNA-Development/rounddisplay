#ifndef ASIM_DRIVER_H
#define ASIM_DRIVER_H

#ifdef ASIM

#include "DisplayDriver.h"

class AsimDriver : public DisplayDriver
{
public:	
	AsimDriver();
	void init();
	void drawArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
	void write(uint16_t color);	
	uint16_t height() const {return LCD_WIDTH;};
	uint16_t width() const {return LCD_HEIGHT;};

private:
	static const uint16_t LCD_WIDTH =  240;
	static const uint16_t LCD_HEIGHT =  240;

	uint16_t asim_x = 0;
	uint16_t asim_y = 0;
	uint16_t asim_xs = 0;
	uint16_t asim_xe = 0;
	uint16_t asim_ys = 0;
	uint16_t asim_ye = 0;


};

extern uint16_t asim_buffer[240*240];
#endif

#endif

