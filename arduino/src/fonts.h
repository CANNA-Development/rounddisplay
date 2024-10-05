#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

#define MAX_HEIGHT_FONT         41
#define MAX_WIDTH_FONT          32
#define OFFSET_BITMAP           


#ifdef ASIM
    #define PROGMEM
#else
    #include <avr/pgmspace.h>
#endif

struct FONT
{    
  const uint8_t *table;
  uint16_t width;
  uint16_t height;
};

extern FONT Font24;
extern FONT Font16;

extern const unsigned char Font16_Table[];

#endif
