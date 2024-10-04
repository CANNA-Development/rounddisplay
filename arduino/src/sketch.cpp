
#ifndef ASIM
    #include <Arduino.h>
#endif
#include "LCD_Driver.h"
#include "paint.h"
#include "fonts.h"


static const uint16_t RING_FCOL = (0x03 << 11) + (0x1A << 5) + 0x1A;
static const uint16_t RING_BCOL = (0x02 << 11) + (0x06 << 5) + 0x06;

static const float BEGIN_ANGLE = -225.0f;
static const float END_ANGLE = 45.0f;
static const float ANGLE_RANGE = END_ANGLE - BEGIN_ANGLE;
static const float ANGLE_PERCENT_STEP = 2.7f;
static const uint16_t FONT_OFFSET = 10;
static const uint16_t INNER_RADIUS = 90;
static const uint16_t OUTER_RADIUS = 110;


static float prev_angle = BEGIN_ANGLE;


void paintVolumeText(uint8_t number)
{

    const uint16_t bcol = BLACK;
    const uint16_t fcol = GREEN;

    static uint8_t prev_number = 0;

    const sFONT *font = &Font24;
    uint16_t yp = LCD_HEIGHT / 2 - font->Height / 2;

    uint8_t d0 = (number % 10);

    if ((prev_number > 99) && (number < 100))
    {
        for (uint16_t y = yp; y < yp + font->Height; y++)
        {
            paintHorLine(LCD_WIDTH / 2 - font->Width * 3 / 2, y, font->Width / 2 + 1, bcol);
            paintHorLine(LCD_WIDTH / 2 + font->Width, y, font->Width / 2 + 1, bcol);
        }
    }
    else if ((prev_number > 9) && (number < 10))
    {
        for (WORD y = yp; y < yp + font->Height; y++)
        {
            paintHorLine(LCD_WIDTH / 2 - font->Width, y, font->Width / 2 + 1, bcol);
            paintHorLine(LCD_WIDTH / 2 + font->Width / 2, y, font->Width / 2 + 1, bcol);
        }
    }

    if (number < 10)
    {
        paintChar(LCD_WIDTH / 2 - font->Width / 2, yp, d0 + '0', font, bcol, fcol);
    }
    else if (number < 100)
    {
        uint8_t d1 = ((number / 10) % 10);
        paintChar(LCD_WIDTH / 2 - font->Width, yp, d1 + '0', font, bcol, fcol);
        paintChar(LCD_WIDTH / 2, yp, d0 + '0', font, bcol, fcol);
    }
    else
    {
        paintChar(LCD_WIDTH / 2 - font->Width * 3 / 2, yp, '1', font, bcol, fcol);
        paintChar(LCD_WIDTH / 2 - font->Width * 1 / 2, yp, '0', font, bcol, fcol);
        paintChar(LCD_WIDTH / 2 + font->Width * 1 / 2, yp, '0', font, bcol, fcol);
    }
    prev_number = number;

}

void setVolume(uint8_t level)
{
    // Set the volume to the desired level
    float ea = ((float)level * ANGLE_PERCENT_STEP) + BEGIN_ANGLE;

    if (ea > prev_angle)
    {
        paintFillArc(LCD_WIDTH / 2, LCD_HEIGHT / 2, INNER_RADIUS, OUTER_RADIUS, prev_angle - 0.1f, ea, RING_FCOL);
    }
    else
    {
        paintFillArc(LCD_WIDTH / 2, LCD_HEIGHT / 2, INNER_RADIUS, OUTER_RADIUS, ea - 0.1f, prev_angle, RING_BCOL);
    }

    paintVolumeText(level);
    prev_angle = ea;
}

void sketch_setup()
{
    #ifndef ASIM
        Config_Init();
    #endif
    LCD_Init();
    LCD_SetBacklight(1000);

    paintClear(BLACK);
    paintString(LCD_WIDTH/2 - Font16.Width * 7 /2, 80, "MDesign", &Font16, BLACK, GREEN);
    paintFillArc(LCD_WIDTH / 2, LCD_HEIGHT / 2, INNER_RADIUS, OUTER_RADIUS, BEGIN_ANGLE, END_ANGLE, RING_BCOL);
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
    loopCount++;
}
