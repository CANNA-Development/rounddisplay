#ifndef DISPLAYDRIVER_H
#define DISPLAYDRIVER_H

#include <stdint.h>

class DisplayDriver
{
public:    
    virtual void init();
    virtual void drawArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    virtual void write(uint16_t color);
    virtual uint16_t height() const;
    virtual uint16_t width() const;
};

#endif
