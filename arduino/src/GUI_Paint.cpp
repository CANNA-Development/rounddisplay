/*****************************************************************************
  | File        :   GUI_Paint.c
  | Author      :   Waveshare team
  | Function    : Achieve drawing: draw points, lines, boxes, circles and
                    their size, solid dotted line, solid rectangle hollow
                    rectangle, solid circle hollow circle.
  | Info        :
    Achieve display characters: Display a single character, string, number
    Achieve time display: adaptive size display time minutes and seconds
  ----------------
  | This version:   V1.0
  | Date        :   2018-11-15
  | Info        :

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documnetation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to  whom the Software is
  furished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

******************************************************************************/
#include "GUI_Paint.h"
#include "DEV_Config.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h> //memset()
#include <math.h>

#define FLT_EPSILON (1.0e-6f)

volatile PAINT Paint;

/******************************************************************************
  function: Create Image
  parameter:
    image   :   Pointer to the image cache
    width   :   The width of the picture
    Height  :   The height of the picture
    Color   :   Whether the picture is inverted
******************************************************************************/
void Paint_NewImage(UWORD Width, UWORD Height, UWORD Color)
{
    Paint.WidthMemory = Width;
    Paint.HeightMemory = Height;
    Paint.Color = Color;
    Paint.WidthByte = Width;
    Paint.HeightByte = Height;
    Paint.Width = Width;
    Paint.Height = Height;
}

/******************************************************************************
  function: Draw Pixels
  parameter:
    x     :   At point X
    y     :   At point Y
    color :   Painted colors
******************************************************************************/
void Paint_SetPixel(UWORD x, UWORD y, UWORD color)
{
    LCD_SetUWORD(x, y, color);
}

/******************************************************************************
  function: Clear the color of the picture
  parameter:
    Color   :   Painted colors
******************************************************************************/
void Paint_Clear(UWORD Color)
{
    LCD_SetCursor(0, 0, Paint.WidthByte-1, Paint.HeightByte);
    for (UWORD Y = 0; Y < Paint.HeightByte; Y++)
    {
        for (UWORD X = 0; X < Paint.WidthByte; X++)
        { 
            LCD_WriteData_Word(Color);
        }
    }
}

/******************************************************************************
  function: Clear the color of a window
  parameter:
    xs :   x starting point
    ys :   Y starting point
    xe   :   x end point
    ye   :   y end point
******************************************************************************/
void Paint_ClearWindows(UWORD xs, UWORD ys, UWORD xe, UWORD ye, UWORD color)
{
    for (UWORD y = ys; y < ye; y++)
    {
        for (UWORD x = xs; x < xe; x++)
        { 
            Paint_SetPixel(x, y, color);
        }
    }
}

/******************************************************************************
function:	Draw Point(Xpoint, Ypoint) Fill the color
parameter:
    Xpoint		:   The Xpoint coordinate of the point
    Ypoint		:   The Ypoint coordinate of the point
    Color		:   Set color
    Dot_Pixel	:	point size
******************************************************************************/
void Paint_DrawPoint(UWORD Xpoint, UWORD Ypoint, UWORD Color,
                     DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_FillWay)
{
    if (Xpoint > Paint.Width || Ypoint > Paint.Height)
    {
        Debug("Paint_DrawPoint Input exceeds the normal display range\r\n");
        return;
    }

    WORD XDir_Num, YDir_Num;
    if (Dot_FillWay == DOT_FILL_AROUND)
    {
        for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++)
        {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++)
            {
                if (Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0)
                    break;
                // printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel);
                Paint_SetPixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
            }
        }
    }
    else
    {
        for (XDir_Num = 0; XDir_Num < Dot_Pixel; XDir_Num++)
        {
            for (YDir_Num = 0; YDir_Num < Dot_Pixel; YDir_Num++)
            {
                Paint_SetPixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
            }
        }
    }
}

/******************************************************************************
function:	Draw a line of arbitrary slope
parameter:
    xs 锛歋tarting Xpoint point coordinates
    ys 锛歋tarting Xpoint point coordinates
    xe   锛欵nd point Xpoint coordinate
    ye   锛欵nd point Ypoint coordinate
    Color  The color of the line segment
******************************************************************************/
void Paint_DrawLine(UWORD xs, UWORD ys, UWORD xe, UWORD ye,
                    UWORD Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style)
{
    if (xs > Paint.Width || ys > Paint.Height ||
        xe > Paint.Width || ye > Paint.Height)
    {
        Debug("Paint_DrawLine Input exceeds the normal display range\r\n");
        return;
    }

    UWORD Xpoint = xs;
    UWORD Ypoint = ys;

    int dx = (int)xe - (int)xs >= 0 ? xe - xs : xs - xe;
    int dy = (int)ye - (int)ys <= 0 ? ye - ys : ys - ye;

    // Increment direction, 1 is positive, -1 is counter;
    int XAddway = xs < xe ? 1 : -1;
    int YAddway = ys < ye ? 1 : -1;

    // Cumulative error
    int Esp = dx + dy;
    char Dotted_Len = 0;

    for (;;)
    {
        Dotted_Len++;
        // Painted dotted line, 2 point is really virtual
        if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0)
        {
            // Debug("LINE_DOTTED\r\n");
            Paint_DrawPoint(Xpoint, Ypoint, IMAGE_BACKGROUND, Line_width, DOT_STYLE_DFT);
            Dotted_Len = 0;
        }
        else
        {
            Paint_DrawPoint(Xpoint, Ypoint, Color, Line_width, DOT_STYLE_DFT);
        }
        if (2 * Esp >= dy)
        {
            if (Xpoint == xe)
                break;
            Esp += dy;
            Xpoint += XAddway;
        }
        if (2 * Esp <= dx)
        {
            if (Ypoint == ye)
                break;
            Esp += dx;
            Ypoint += YAddway;
        }
    }
}

/******************************************************************************
function:	Draw a rectangle
parameter:
    xs Rectangular  Starting Xpoint point coordinates
    ys Rectangular  Starting Xpoint point coordinates
    xe   Rectangular  End point Xpoint coordinate
    ye   Rectangular  End point Ypoint coordinate
    Color  The color of the Rectangular segment
    Filled : Whether it is filled--- 1 solid 0 = Empty
******************************************************************************/
void Paint_DrawRectangle(UWORD xs, UWORD ys, UWORD xe, UWORD ye,
                         UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Filled)
{
    if (xs > Paint.Width || ys > Paint.Height ||
        xe > Paint.Width || ye > Paint.Height)
    {
        Debug("Input exceeds the normal display range\r\n");
        return;
    }

    if (Filled)
    {
        UWORD Ypoint;
        for (Ypoint = ys; Ypoint < ye; Ypoint++)
        {
            Paint_DrawLine(xs, Ypoint, xe, Ypoint, Color, Line_width, LINE_STYLE_SOLID);
        }
    }
    else
    {
        Paint_DrawLine(xs, ys, xe, ys, Color, Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(xs, ys, xs, ye, Color, Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(xe, ye, xe, ys, Color, Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(xe, ye, xs, ye, Color, Line_width, LINE_STYLE_SOLID);
    }
}

/******************************************************************************
function:	Use the 8-point method to draw a circle of the
            specified size at the specified position->
parameter:
    X_Center  Center X coordinate
    Y_Center  Center Y coordinate
    Radius    Circle Radius
    Color     The color of the Circle segment
    Filled    : Whether it is filled: 1 filling 0 = not
******************************************************************************/
void Paint_DrawArc(UWORD X_Center, UWORD Y_Center, UWORD Radius,
                   UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)

{
    if (X_Center > Paint.Width || Y_Center >= Paint.Height)
    {
        Debug("Paint_DrawCircle Input exceeds the normal display range\r\n");
        return;
    }
    // Draw a circle from(0, R) as a starting point
    WORD XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;

    // Cumulative error,judge the next point of the logo
    WORD Esp = 3 - (Radius << 1);

    if (Draw_Fill != DRAW_FILL_FULL)
    {
        while (XCurrent <= YCurrent)
        {
            Paint_DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT); // 1
            // Paint_DrawPoint(X_Center - XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//2
            // Paint_DrawPoint(X_Center - YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//3
            // Paint_DrawPoint(X_Center - YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//4
            // Paint_DrawPoint(X_Center - XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//5
            // Paint_DrawPoint(X_Center + XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//6
            // Paint_DrawPoint(X_Center + YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//7
            // Paint_DrawPoint(X_Center + YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//0

            if (Esp < 0)
                Esp += 4 * XCurrent + 6;
            else
            {
                Esp += 10 + 4 * (XCurrent - YCurrent);
                YCurrent--;
            }
            XCurrent++;
        }
    }
}

/******************************************************************************
function:	Use the 8-point method to draw a circle of the
            specified size at the specified position->
parameter:
    X_Center  Center X coordinate
    Y_Center  Center Y coordinate
    Radius    Circle Radius
    Color     The color of the Circle segment
    Filled    : Whether it is filled: 1 filling 0 = not
******************************************************************************/
void Paint_DrawCircle(UWORD X_Center, UWORD Y_Center, UWORD Radius,
                      UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
{
    if (X_Center > Paint.Width || Y_Center >= Paint.Height)
    {
        Debug("Paint_DrawCircle Input exceeds the normal display range\r\n");
        return;
    }

    // Draw a circle from(0, R) as a starting point
    WORD XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;

    // Cumulative error,judge the next point of the logo
    WORD Esp = 3 - (Radius << 1);

    WORD sCountY = 0;
    if (Draw_Fill == DRAW_FILL_FULL)
    {
        while (XCurrent <= YCurrent)
        { // Realistic circles
            for (sCountY = XCurrent; sCountY <= YCurrent; sCountY++)
            {
                Paint_DrawPoint(X_Center + XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); // 1
                Paint_DrawPoint(X_Center - XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); // 2
                Paint_DrawPoint(X_Center - sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); // 3
                Paint_DrawPoint(X_Center - sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); // 4
                Paint_DrawPoint(X_Center - XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); // 5
                Paint_DrawPoint(X_Center + XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); // 6
                Paint_DrawPoint(X_Center + sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); // 7
                Paint_DrawPoint(X_Center + sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            }
            if (Esp < 0)
                Esp += 4 * XCurrent + 6;
            else
            {
                Esp += 10 + 4 * (XCurrent - YCurrent);
                YCurrent--;
            }
            XCurrent++;
        }
    }
    else
    { // Draw a hollow circle
        while (XCurrent <= YCurrent)
        {
            Paint_DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT); // 1
            Paint_DrawPoint(X_Center - XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT); // 2
            Paint_DrawPoint(X_Center - YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT); // 3
            Paint_DrawPoint(X_Center - YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT); // 4
            Paint_DrawPoint(X_Center - XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT); // 5
            Paint_DrawPoint(X_Center + XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT); // 6
            Paint_DrawPoint(X_Center + YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT); // 7
            Paint_DrawPoint(X_Center + YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT); // 0

            if (Esp < 0)
                Esp += 4 * XCurrent + 6;
            else
            {
                Esp += 10 + 4 * (XCurrent - YCurrent);
                YCurrent--;
            }
            XCurrent++;
        }
    }
}

/******************************************************************************
  function: Show English characters
  parameter:
    Xpoint           x coordinate
    Ypoint           y coordinate
    Acsii_Char       To display the English characters
    Font             Font structure pointer that displays a character size
    Color_Background : Select the background color of the English character
    Color_Foreground : Select the foreground color of the English character
******************************************************************************/
void Paint_DrawChar(UWORD Xpoint, UWORD Ypoint, const char Acsii_Char,
                    sFONT *Font, UWORD Color_Background, UWORD Color_Foreground)
{

    UWORD Page, Column;

    if (Xpoint > Paint.Width || Ypoint > Paint.Height)
    {
        // Debug("Paint_DrawChar Input exceeds the normal display range\r\n");
        return;
    }
    UWORD Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];

    for (Page = 0; Page < Font->Height; Page++)
    {
        for (Column = 0; Column < Font->Width; Column++)
        {

            // To determine whether the font background color and screen background color is consistent
            if (FONT_BACKGROUND == Color_Background)
            { // this process is to speed up the scan
                if (pgm_read_byte(ptr) & (0x80 >> (Column % 8)))
                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
            }
            else
            {
                if (pgm_read_byte(ptr) & (0x80 >> (Column % 8)))
                {
                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                }
                else
                {
                    Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Background);
                }
            }
            // One pixel is 8 bits
            if (Column % 8 == 7)
            {
                ptr++;
            }
        } /* Write a line */
        if (Font->Width % 8 != 0)
        {
            ptr++;
        }
    } /* Write all */
}

/******************************************************************************
  function: Display the string
  parameter:
    xs           x coordinate
    ys           y coordinate
    pString          The first address of the English string to be displayed
    Font             Font structure pointer that displays a character size
    Color_Background : Select the background color of the English character
    Color_Foreground : Select the foreground color of the English character
******************************************************************************/
void Paint_DrawString(UWORD xs, UWORD ys, const char *pString, sFONT *Font, UWORD Color_Background, UWORD Color_Foreground)
{
    UWORD Xpoint = xs;
    UWORD Ypoint = ys;

    if (xs > Paint.Width || ys > Paint.Height)
    {
        // Debug("Paint_DrawString_EN Input exceeds the normal display range\r\n");
        return;
    }

    while (*pString != '\0')
    {
        // if X direction filled , reposition to(xs,Ypoint),Ypoint is Y direction plus the Height of the character
        if ((Xpoint + Font->Width) > Paint.Width)
        {
            Xpoint = xs;
            Ypoint += Font->Height;
        }

        // If the Y direction is full, reposition to(xs, ys)
        if ((Ypoint + Font->Height) > Paint.Height)
        {
            Xpoint = xs;
            Ypoint = ys;
        }
        Paint_DrawChar(Xpoint, Ypoint, *pString, Font, Color_Background, Color_Foreground);

        // The next character of the address
        pString++;

        // The next word of the abscissa increases the font of the broadband
        Xpoint += Font->Width;
    }
}

/******************************************************************************
  function: Display nummber
  parameter:
    xs           x coordinate
    ys           : Y coordinate
    Nummber          : The number displayed
    Font             Font structure pointer that displays a character size
    Color_Background : Select the background color of the English character
    Color_Foreground : Select the foreground color of the English character
******************************************************************************/
#define ARRAY_LEN 50
void Paint_DrawNum(UWORD Xpoint, UWORD Ypoint, WORD Nummber, sFONT *Font, UWORD Color_Background, UWORD Color_Foreground, uint8_t digits)
{

    WORD Num_Bit = 0, Str_Bit = 0;
    uint8_t Str_Array[ARRAY_LEN] = {0}, Num_Array[ARRAY_LEN] = {0};
    uint8_t *pStr = Str_Array;

    if (Xpoint > Paint.Width || Ypoint > Paint.Height)
    {
        // Debug("Paint_DisNum Input exceeds the normal display range\r\n");
        return;
    }

    // Converts a number to a string
    do
    {
        Num_Array[Num_Bit] = Nummber % 10 + '0';
        Num_Bit++;
        Nummber /= 10;
    } while (Nummber);

    // The string is inverted
    while (Num_Bit > 0)
    {
        Str_Array[Str_Bit] = Num_Array[Num_Bit - 1];
        Str_Bit++;
        Num_Bit--;
    }

    // show
    Paint_DrawString(Xpoint, Ypoint, (const char *)pStr, Font, Color_Background, Color_Foreground);
}
/******************************************************************************
function:	Display float number
parameter:
    xs           x coordinate
    ys           : Y coordinate
    Nummber          : The float data that you want to display
  Decimal_Point	 : Show decimal places
    Font             Font structure pointer that displays a character size
    Color            : Select the background color of the English character
******************************************************************************/
// void Paint_DrawFloatNum(UWORD Xpoint, UWORD Ypoint, double Nummber,  UBYTE Decimal_Point,
//                         sFONT* Font,  UWORD Color_Background, UWORD Color_Foreground)
// {
//   char Str[ARRAY_LEN] = {0};
//   dtostrf(Nummber,0,Decimal_Point+2,Str);
//   char * pStr= (char *)malloc((strlen(Str))*sizeof(char));
//   memcpy(pStr,Str,(strlen(Str)-2));
//   * (pStr+strlen(Str)-1)='\0';
//   if((*(pStr+strlen(Str)-3))=='.')
//   {
// 	*(pStr+strlen(Str)-3)='\0';
//   }
//   //show
//   Paint_DrawString_EN(Xpoint, Ypoint, (const char*)pStr, Font, Color_Foreground, Color_Background);
//   free(pStr);
//   pStr=NULL;
// }

/******************************************************************************
  function: Display time
  parameter:
    xs           x coordinate
    ys           : Y coordinate
    pTime            : Time-related structures
    Font             Font structure pointer that displays a character size
    Color            : Select the background color of the English character
******************************************************************************/
void Paint_DrawTime(UWORD xs, UWORD ys, PAINT_TIME *pTime, sFONT *Font,
                    UWORD Color_Background, UWORD Color_Foreground)
{
    uint8_t value[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    UWORD Dx = Font->Width;

    // Write data into the cache
    Paint_DrawChar(xs, ys, value[pTime->Hour / 10], Font, Color_Background, Color_Foreground);
    Paint_DrawChar(xs + Dx, ys, value[pTime->Hour % 10], Font, Color_Background, Color_Foreground);
    Paint_DrawChar(xs + Dx + Dx / 4 + Dx / 2, ys, ':', Font, Color_Background, Color_Foreground);
    Paint_DrawChar(xs + Dx * 2 + Dx / 2, ys, value[pTime->Min / 10], Font, Color_Background, Color_Foreground);
    Paint_DrawChar(xs + Dx * 3 + Dx / 2, ys, value[pTime->Min % 10], Font, Color_Background, Color_Foreground);
    Paint_DrawChar(xs + Dx * 4 + Dx / 2 - Dx / 4, ys, ':', Font, Color_Background, Color_Foreground);
    Paint_DrawChar(xs + Dx * 5, ys, value[pTime->Sec / 10], Font, Color_Background, Color_Foreground);
    Paint_DrawChar(xs + Dx * 6, ys, value[pTime->Sec % 10], Font, Color_Background, Color_Foreground);
}

/******************************************************************************
  function: Display image
  parameter:
    image            : Image start address
    xs           : X starting coordinates
    ys           : Y starting coordinates
    xe             : Image width
    ye             : Image height
******************************************************************************/
void Paint_DrawImage(const unsigned char *image, UWORD xs, UWORD ys, UWORD W_Image, UWORD H_Image)
{
    int i, j;
    for (j = 0; j < H_Image; j++)
    {
        for (i = 0; i < W_Image; i++)
        {
            if (xs + i < LCD_WIDTH && ys + j < LCD_HEIGHT) // Exceeded part does not display
                Paint_SetPixel(xs + i, ys + j, (pgm_read_byte(image + j * W_Image * 2 + i * 2 + 1)) << 8 | (pgm_read_byte(image + j * W_Image * 2 + i * 2)));
            // Using arrays is a property of sequential storage, accessing the original array by algorithm
            // j*W_Image*2          Y offset
            // i*2                  X offset
            // pgm_read_byte()
        }
    }
}

/**************************************************************************/
/*!
  @brief  Write a perfectly horizontal line
  @param  x       Left-most x coordinate
  @param  y       Left-most y coordinate
  @param  w       Width in pixels
  @param  color   16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Paint_HLine(UWORD x, UWORD y, UWORD w, UWORD color)
{
    LCD_SetCursor(x, y, x + w -1, y);
    for (WORD i = x; i < x + w; i++)
    {
        //Paint_SetPixel(i, y, color);
        LCD_WriteData_Word(color);
    }
}

#define PI 3.141592653f
#define DEGTORAD (PI / 180.0f)

/**************************************************************************/
/*!
  @brief  Arc drawer with fill
  @param  cx      Center-point x coordinate
  @param  cy      Center-point y coordinate
  @param  oradius Outer radius of arc
  @param  iradius Inner radius of arc
  @param  start   degree of arc start
  @param  end     degree of arc end
  @param  color   16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void writeFillArcHelper(UWORD cx, WORD cy, WORD oradius, WORD iradius, float start, float end, UWORD color)
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
                    Paint_HLine(cx + x - len, cy + y, len, color);
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

/**************************************************************************/
/*!
  @brief  Draw an arc outline
  @param  x       Center-point x coordinate
  @param  y       Center-point y coordinate
  @param  r1      Outer radius of arc
  @param  r2      Inner radius of arc
  @param  start   degree of arc start
  @param  end     degree of arc end
  @param  color   16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Paint_drawArc1(WORD x, WORD y, WORD r1, WORD r2, float start, float end, UWORD color)
{
    if (r1 < r2)
    {
        WORD t = r1;
        r1 = r2;
        r2 = t;
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

    //  startWrite();
    writeFillArcHelper(x, y, r1, r2, start, start, color);
    writeFillArcHelper(x, y, r1, r2, end, end, color);
    if (!equal && (fabsf(start - end) <= 0.0001))
    {
        start = .0;
        end = 360.0;
    }
    writeFillArcHelper(x, y, r1, r1, start, end, color);
    writeFillArcHelper(x, y, r2, r2, start, end, color);
    //  endWrite();
}

/**************************************************************************/
/*!
  @brief  Draw an arc with filled color
  @param  x       Center-point x coordinate
  @param  y       Center-point y coordinate
  @param  r1      Outer radius of arc
  @param  r2      Inner radius of arc
  @param  start   degree of arc start
  @param  end     degree of arc end
  @param  color   16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Paint_fillArc(WORD x, WORD y, WORD r1, WORD r2, float start, float end, UWORD color)
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

    // startWrite();
    writeFillArcHelper(x, y, r1, r2, start, end, color);
    // endWrite();
}
