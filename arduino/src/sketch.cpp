
#ifndef ASIM
    #include <Arduino.h>
#endif
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image_70x70.h"
#include "fonts.h"


static const UWORD RING_FCOL = (0x03 << 11) + (0x1A << 5) + 0x1A;
static const UWORD RING_BCOL = (0x02 << 11) + (0x06 << 5) + 0x06;

static const float BEGIN_ANGLE = -225.0f;
static const float END_ANGLE = 45.0f;
static const float ANGLE_RANGE = END_ANGLE - BEGIN_ANGLE;
static const float ANGLE_PERCENT_STEP = 2.7f;
static const UWORD FONT_OFFSET = 10;
static const UWORD INNER_RADIUS = 90;
static const UWORD OUTER_RADIUS = 110;


static float prev_angle = BEGIN_ANGLE;

void Paint_Digit(UWORD xp, UWORD yp, const uint8_t digit, const sFONT *font, UWORD bcol, UWORD fcol)
{

    UWORD offset = (digit + '0' - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const uint8_t *ptr = &font->table[offset];

    LCD_SetCursor(xp,yp,xp + font->Width,yp + font->Height);
    for (UWORD page = 0; page < font->Height; page++)
    {
        for (UWORD column = 0; column < font->Width; column++)
        {

            if (pgm_read_byte(ptr) & (0x80 >> (column % 8)))
            {
                //Paint_SetPixel(xp + column, yp + page, fcol);
                LCD_WriteData_Word(fcol);  
            }
            else
            {
                //Paint_SetPixel(xp + column, yp + page, bcol);
                LCD_WriteData_Word(bcol);  
            }
            // One pixel is 8 bits
            if (column % 8 == 7)
            {
                ptr++;
            }
        } /* Write a line */
        if (font->Width % 8 != 0)
        {
            ptr++;
        }
    }
}

void Paint_VolumeText(uint8_t number)
{

    const UWORD bcol = BLACK;
    const UWORD fcol = GREEN;

    static uint8_t prev_number = 0;

    const sFONT *font = &Font24;
    UWORD yp = LCD_HEIGHT / 2 - font->Height / 2;

    uint8_t d0 = (number % 10);

    if ((prev_number > 99) && (number < 100))
    {
        for (WORD y = yp; y < yp + font->Height; y++)
        {
            Paint_HLine(LCD_WIDTH / 2 - font->Width * 3 / 2, y, font->Width / 2 + 1, bcol);
            Paint_HLine(LCD_WIDTH / 2 + font->Width, y, font->Width / 2 + 1, bcol);
        }
    }
    else if ((prev_number > 9) && (number < 10))
    {
        for (WORD y = yp; y < yp + font->Height; y++)
        {
            Paint_HLine(LCD_WIDTH / 2 - font->Width, y, font->Width / 2 + 1, bcol);
            Paint_HLine(LCD_WIDTH / 2 + font->Width / 2, y, font->Width / 2 + 1, bcol);
        }
    }

    if (number < 10)
    {
        Paint_Digit(LCD_WIDTH / 2 - font->Width / 2, yp, d0, font, bcol, fcol);
    }
    else if (number < 100)
    {
        uint8_t d1 = ((number / 10) % 10);
        Paint_Digit(LCD_WIDTH / 2 - font->Width, yp, d1, font, bcol, fcol);
        Paint_Digit(LCD_WIDTH / 2, yp, d0, font, bcol, fcol);
    }
    else
    {
        Paint_Digit(LCD_WIDTH / 2 - font->Width * 3 / 2, yp, 1, font, bcol, fcol);
        Paint_Digit(LCD_WIDTH / 2 - font->Width * 1 / 2, yp, 0, font, bcol, fcol);
        Paint_Digit(LCD_WIDTH / 2 + font->Width * 1 / 2, yp, 0, font, bcol, fcol);
    }
    prev_number = number;

    // Paint_DrawLine(120, 90, 120, 150, YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // Paint_DrawLine(104, 90, 104, 150, YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // Paint_DrawLine(136, 90, 136, 150, YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
}

void setVolume(uint8_t level)
{
    // Set the volume to the desired level
    // float sa     = (level * -0.00005f) + 135.0f;
    //    float ea = (level * .0f) + 225.0f;
    float ea = ((float)level * ANGLE_PERCENT_STEP) + BEGIN_ANGLE;

    if (ea > prev_angle)
    {
        Paint_fillArc(LCD_WIDTH / 2, LCD_HEIGHT / 2, INNER_RADIUS, OUTER_RADIUS, prev_angle - 0.1f, ea, RING_FCOL);
        //printf("filling from: level: %d %5.2f to %5.2f\n", level, prev_angle, ea);
    }
    else
    {
        Paint_fillArc(LCD_WIDTH / 2, LCD_HEIGHT / 2, INNER_RADIUS, OUTER_RADIUS, ea - 0.1f, prev_angle, RING_BCOL);
        //printf("clearing from: level: %d %5.2f to %5.2f\n", level, ea, prev_angle);
    }

    Paint_VolumeText(level);
    prev_angle = ea;
}

void sketch_setup()
{
    #ifndef ASIM
        Config_Init();
    #endif
    LCD_Init();
    LCD_SetBacklight(1000);

    Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, BLACK);
    Paint_Clear(BLACK);
    Paint_DrawString(LCD_WIDTH/2 - Font16.Width * 7 /2, 80, "MDesign", &Font16, BLACK, GREEN);
    Paint_fillArc(LCD_WIDTH / 2, LCD_HEIGHT / 2, INNER_RADIUS, OUTER_RADIUS, BEGIN_ANGLE, END_ANGLE, RING_BCOL);
}

int loopCount = 0;
int level = 40;
bool up = true;


void sketch_loop1(){

    if ( level ==0) {
        level = 99;
    } else {
        level = 0;
    }
    setVolume(level);

}

void sketch_loop()
{
    // int num = (loopCount / 100 ) %9;

    //if ((loopCount) % 5 == 0)
    { 
        if (up)
        {
            level++;
        }
        else
        {
            level--;
        }
        if (level == 100)
        {
            up = false;
        }
        else if (level == 0)
        {
            up = true;
        }
        setVolume(level);
    }
    // Paint_DrawLine(loopCount, loopCount, 10, 20, YELLOW, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
    // Paint_DrawString(123, 123, "WAVESHARE",&Font16,  BLACK, GREEN);
    // Paint_DrawNum(100, 200, num, &Font24,BLACK, GREEN);

    loopCount++;
}
