#include "AsimDriver.h"
#include <SPI.h>

#ifdef ASIM

AsimDriver::AsimDriver()
{
}

void AsimDriver::write(uint8_t color) 
{
    int idx = asim_x + asim_y * width;
    asim_buffer[idx] = color;

    asim_x++;
    if (asim_x >= asim_xe) {
        asim_x = asim_xs;
        asim_y++;
    }
    // Check if we are done
    if (asim_y >= asim_ye) {
        asim_y = asim_ys;
    }
}  


void AsimDriver::drawArea(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t  ye)
{ 
	asim_xs = xs;
	asim_xe = xe;
	asim_ys = ys;
	asim_ye = ye;

	asim_x = asim_xs;
	asim_y = asim_ys;
};

#endif