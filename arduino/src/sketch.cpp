
#ifndef ASIM
    #include <Arduino.h>
    #include "LcdDriver.h"
#else
    #include "AsimDriver.h"
#endif


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

static const uint8_t LCD_DC_PIN  = 7;
static const uint8_t  LCD_RST_PIN = 8;


static float prev_angle = BEGIN_ANGLE;

#ifdef ASIM
    static AsimDriver driver;
#else
    static LcdDriver driver(LCD_RST_PIN, LCD_DC_PIN);
#endif

static Paint paint(driver);

void paintVolumeText(uint8_t number)
{
    const uint16_t bcol = Paint::BLACK;
    const uint16_t fcol = Paint::GREEN;

    static uint8_t prev_number = 0;

    const FONT *font = &Font24;
    uint16_t yp = driver.height() / 2 - font->height / 2;

    uint8_t d0 = (number % 10);

    if ((prev_number > 99) && (number < 100))
    {
        for (uint16_t y = yp; y < yp + font->height; y++)
        {
            paint.horizontalLine(driver.width() / 2 - font->width * 3 / 2, y, font->width / 2 + 1, bcol);
            paint.horizontalLine(driver.width() / 2 + font->width, y, font->width / 2 + 1, bcol);
        }
    }
    else if ((prev_number > 9) && (number < 10))
    {
        for (uint16_t y = yp; y < yp + font->height; y++)
        {
            paint.horizontalLine(driver.width() / 2 - font->width, y, font->width / 2 + 1, bcol);
            paint.horizontalLine(driver.width() / 2 + font->width / 2, y, font->width / 2 + 1, bcol);
        }
    }

    if (number < 10)
    {
        paint.character(driver.width() / 2 - font->width / 2, yp, d0 + '0', font, bcol, fcol);
    }
    else if (number < 100)
    {
        uint8_t d1 = ((number / 10) % 10);
        paint.character(driver.width() / 2 - font->width, yp, d1 + '0', font, bcol, fcol);
        paint.character(driver.width() / 2, yp, d0 + '0', font, bcol, fcol);
    }
    else
    {
        paint.character(driver.width() / 2 - font->width * 3 / 2, yp, '1', font, bcol, fcol);
        paint.character(driver.width() / 2 - font->width * 1 / 2, yp, '0', font, bcol, fcol);
        paint.character(driver.width() / 2 + font->width * 1 / 2, yp, '0', font, bcol, fcol);
    }
    prev_number = number;

}

void setVolume(uint8_t level)
{
    // Set the volume to the desired level
    float ea = ((float)level * ANGLE_PERCENT_STEP) + BEGIN_ANGLE;

    if (ea > prev_angle)
    {
        paint.fillArc(driver.width() / 2, driver.height() / 2, INNER_RADIUS, OUTER_RADIUS, prev_angle - 0.1f, ea, RING_FCOL);
    }
    else
    {
        paint.fillArc(driver.width() / 2, driver.height() / 2, INNER_RADIUS, OUTER_RADIUS, ea - 0.1f, prev_angle, RING_BCOL);
    }

    paintVolumeText(level);
    prev_angle = ea;
}

void sketch_setup()
{
    #ifndef ASIM
      //  Config_Init();
    #endif

    paint.init();
    paint.string(driver.width()/2 - Font16.width * 7 /2, 80, "MDesign", &Font16, Paint::BLACK, Paint::GREEN);
    paint.fillArc(driver.width() / 2, driver.height() / 2, INNER_RADIUS, OUTER_RADIUS, BEGIN_ANGLE, END_ANGLE, RING_BCOL);
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
