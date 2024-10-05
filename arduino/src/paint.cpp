#include "paint.h"
#include "DEV_Config.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h> //memset()
#include <math.h>

#define FLT_EPSILON (1.0e-6f)


Paint::Paint(uint16_t width, uint16_t height)
    : _width(width), _height(height)
{
}

void Paint::init()
{
    LCD_Init();
    clear(BLACK);
}

void Paint::setPixel(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_SetCursor(x, y, x, y);
    LCD_WriteData_Word(color);
}


void Paint::clear(uint16_t color)
{
    for (uint16_t y = 0; y < _width; y++)
    {
        horizontalLine(0, y, _width, color);
    }
}

void Paint::character(uint16_t xp, uint16_t yp, const uint8_t ch, const sFONT *font, uint16_t bcol, uint16_t fcol)
{

    uint16_t offset = (ch - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const uint8_t *ptr = &font->table[offset];

    LCD_SetCursor(xp, yp, xp + font->Width - 1, yp + font->Height - 1);
    for (uint16_t page = 0; page < font->Height; page++)
    {
        for (uint16_t column = 0; column < font->Width; column++)
        {

            if (pgm_read_byte(ptr) & (0x80 >> (column % 8)))
            {
                LCD_WriteData_Word(fcol);
            }
            else
            {
                LCD_WriteData_Word(bcol);
            }

            if (column % 8 == 7)
            {
                ptr++;
            }
        }

        // next line?
        if (font->Width % 8 != 0)
        {
            ptr++;
        }
    }
}

/******************************************************************************
  function: Display the string
  parameter:
    xs           x coordinate
    ys           y coordinate
    pString          The first address of the English string to be displayed
    Font             Font structure pointer that displays a character size
    bcol : Select the background color of the English character
    fcol : Select the foreground color of the English character
******************************************************************************/
void Paint::string(uint16_t xp, uint16_t yp, const char *string, sFONT *Font, uint16_t bcol, uint16_t fcol)
{
    while (string[0] != '\0')
    {
        character(xp, yp, string[0], Font, bcol, fcol);
        string++;
        xp += Font->Width;
    }
}

void Paint::horizontalLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
    LCD_SetCursor(x, y, x + w - 1, y);
    for (uint16_t i = x; i < x + w; i++)
    {
        LCD_WriteData_Word(color);
    }
}

#define PI 3.141592653f
#define DEGTORAD (PI / 180.0f)

void Paint::writeFillArcHelper(uint16_t cx, uint16_t cy, uint16_t oradius, uint16_t iradius, float start, float end, uint16_t color)
{
    if ((start == 90.0) || (start == 180.0) || (start == 270.0) || (start == 360.0))
    {
        start -= 0.1;
    }

    if ((end == 90.0) || (end == 180.0) || (end == 270.0) || (end == 360.0))
    {
        end -= 0.1;
    }

    float s_cos = (cos(start * DEGTORAD));
    float e_cos = (cos(end * DEGTORAD));
    float sslope = s_cos / (sin(start * DEGTORAD));
    float eslope = e_cos / (sin(end * DEGTORAD));
    float swidth = 0.5 / s_cos;
    float ewidth = -0.5 / e_cos;
    --iradius;
    WORD ir2 = iradius * iradius + iradius;
    WORD or2 = oradius * oradius + oradius;

    bool start180 = !(start < 180.0);
    bool end180 = end < 180.0;
    bool reversed = start + 180.0 < end || (end < start && start < end + 180.0);

    WORD xs = -oradius;
    WORD y = -oradius;
    WORD ye = oradius;
    WORD xe = oradius + 1;
    if (!reversed)
    {
        if ((end >= 270 || end < 90) && (start >= 270 || start < 90))
        {
            xs = 0;
        }
        else if (end < 270 && end >= 90 && start < 270 && start >= 90)
        {
            xe = 1;
        }
        if (end >= 180 && start >= 180)
        {
            ye = 0;
        }
        else if (end < 180 && start < 180)
        {
            y = 0;
        }
    }
    do
    {
        WORD y2 = y * y;
        WORD x = xs;
        if (x < 0)
        {
            while (x * x + y2 >= or2)
            {
                ++x;
            }
            if (xe != 1)
            {
                xe = 1 - x;
            }
        }
        float ysslope = (y + swidth) * sslope;
        float yeslope = (y + ewidth) * eslope;
        WORD len = 0;
        do
        {
            bool flg1 = start180 != (x <= ysslope);
            bool flg2 = end180 != (x <= yeslope);
            WORD distance = x * x + y2;
            if (distance >= ir2 && ((flg1 && flg2) || (reversed && (flg1 || flg2))) && x != xe && distance < or2)
            {
                ++len;
            }
            else
            {
                if (len)
                {
                    horizontalLine(cx + x - len, cy + y, len, color);
                    len = 0;
                }
                if (distance >= or2)
                    break;
                if (x < 0 && distance < ir2)
                {
                    x = -x;
                }
            }
        } while (++x <= xe);
    } while (++y <= ye);
}

void Paint::fillArc(uint16_t x, uint16_t y, uint16_t r1, uint16_t r2, float start, float end, uint16_t color)
{
    if (r1 < r2)
    {
        WORD t = r1;
        r1 = r2;
        r2 = t;
        //_swap_WORD(r1, r2);
    }
    if (r1 < 1)
    {
        r1 = 1;
    }
    if (r2 < 1)
    {
        r2 = 1;
    }
    bool equal = fabsf(start - end) < FLT_EPSILON;
    start = fmodf(start, 360);
    end = fmodf(end, 360);
    if (start < 0)
        start += 360.0;
    if (end < 0)
        end += 360.0;
    if (!equal && (fabsf(start - end) <= 0.0001))
    {
        start = .0;
        end = 360.0;
    }
    writeFillArcHelper(x, y, r1, r2, start, end, color);
}
