#ifndef DISPLAYDRIVER_H
#define DISPLAYDRIVER_H

#include <stdint.h>

class DisplayDriver
{
public:    
    virtual void init() = 0;
    virtual void drawArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
    virtual void write(uint16_t color) = 0;
    virtual uint16_t height() const = 0;
    virtual uint16_t width() const = 0;
};

#endif
