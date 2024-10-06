#ifndef _DRIVER_H
#define _DRIVER_H

#ifdef ASIM

#include "DisplayDriver.h"

class AsimDriver : public DisplayDriver
{
public:	
	AsimDriver(uint16_t wiidth, uint16_t height);
    void init() override;
	void drawArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h) override;
	void write(uint16_t color) override;	

    uint16_t height() const override;
    uint16_t width() const override;

private:

	uint16_t _x = 0;
	uint16_t _y = 0;
	uint16_t _xs = 0;
	uint16_t _xe = 0;
	uint16_t _ys = 0;
	uint16_t _ye = 0;
	uint16_t _width ;
	uint16_t _height;

};

extern uint16_t *g_asim_buffer;

#endif

#endif

