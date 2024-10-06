#include "AsimDriver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ASIM

uint16_t *g_asim_buffer = NULL;

AsimDriver::AsimDriver(uint16_t width, uint16_t height)
    :_x(0)
    ,_y(0)
    ,_xs(0)
    ,_xe(0)
    ,_ys(0)
    ,_ye(0)
    ,_width{width}
    ,_height{height}

{
}

uint16_t AsimDriver::height() const {return _width;};
uint16_t AsimDriver::width() const {return _height;};

void AsimDriver::init()
{
    g_asim_buffer = (uint16_t *)malloc(_width * _height * sizeof(uint16_t));
    memset(g_asim_buffer, 0, _width * _height * sizeof(uint16_t));
}   

void AsimDriver::write(uint16_t color) 
{
    int idx = _x + _y * width();
    g_asim_buffer[idx] = color;

    _x++;
    if (_x > _xe) {
        _x = _xs;
        _y++;
    }

    if (_y > _ye) {
        _y = _ys;
    }
}  

void AsimDriver::drawArea(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t  ye)
{ 
	_xs = xs;
	_xe = xe;
	_ys = ys;
	_ye = ye;

	_x = _xs;
	_y = _ys;
};

#endif