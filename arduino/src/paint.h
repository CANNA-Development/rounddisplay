#ifndef __GUI_PAINT_H
#define __GUI_PAINT_H

#include "DEV_Config.h"
#include "LCD_Driver.h"
#include "fonts.h"
#include "Debug.h"

#ifndef pgm_read_byte
  #define pgm_read_byte(x) (*(x))
#endif

//#include <avr/pgmspace.h>
/**
 * Image attributes
**/
typedef struct {
    UBYTE *Image;
    uint16_t Width;
    uint16_t Height;
    uint16_t WidthMemory;
    uint16_t HeightMemory;
    uint16_t Color;
    uint16_t WidthByte;
    uint16_t HeightByte;
} PAINT;
extern volatile PAINT Paint;

/**
 * image color
**/
#define DARKBLUE      0X01CF
#define WHITE         0xFFFF
#define BLACK         0x0000    
#define BLUE          0x001F  
#define BRED          0XF81F
#define GRED          0XFFE0
#define GBLUE         0X07FF
#define RED           0xF800
#define MAGENTA       0xF81F
#define GREEN         0x07E0
#define CYAN          0x7FFF
#define YELLOW        0xFFE0
#define BROWN         0XBC40 
#define BRRED         0XFC07 
#define GRAY          0X8430 
#define DARKBLUE      0X01CF  
#define LIGHTBLUE     0X7D7C   
#define GRAYBLUE      0X5458 
#define LIGHTGREEN    0X841F 
#define LGRAY         0XC618 
#define LGRAYBLUE     0XA651
#define LBBLUE        0X2B12 


#define IMAGE_BACKGROUND    WHITE
#define FONT_FOREGROUND     BLACK
#define FONT_BACKGROUND     WHITE


void paintPixel(uint16_t x, uint16_t y, uint16_t color);
void paintClear(uint16_t olor);
void paintHorLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);
void paintChar(uint16_t xp, uint16_t yp, const uint8_t ch, const sFONT *font, uint16_t bcol, uint16_t fcol);
void paintString(uint16_t xp, uint16_t up, const char * string, sFONT* Font, uint16_t bcol, uint16_t fcol);
void paintFillArc(WORD x, WORD y, WORD r1, WORD r2, float start, float end, uint16_t color);

#endif

