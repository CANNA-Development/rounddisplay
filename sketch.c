
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image_70x70.h"
#include "fonts.h"

void Setup() {

    //Config_Init();
    LCD_Init();
    LCD_SetBacklight(1000);
    Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);
    Paint_Clear(BLACK);
    //Paint_DrawCircle(120,120, 116, BLUE ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    Paint_DrawLine  (120, 0, 120, 12,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
    Paint_DrawLine  (120, 228, 120, 240,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
    Paint_DrawLine  (0, 120, 12, 120,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
    Paint_DrawLine  (228, 120, 240, 120,GREEN ,DOT_PIXEL_4X4,LINE_STYLE_SOLID);
    Paint_DrawImage(image_70X70, 85, 25, 70, 70); 
    Paint_DrawString_EN(123, 123, "WAVESHARE",&Font16,  BLACK, GREEN);
    Paint_DrawLine  (120, 120, 70, 70,YELLOW ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
    Paint_DrawLine  (120, 120, 176, 64,BLUE ,DOT_PIXEL_3X3,LINE_STYLE_SOLID);
    Paint_DrawLine  (120, 120, 120, 210,RED ,DOT_PIXEL_2X2,LINE_STYLE_SOLID); 

    Paint_DrawArc(120,120, 116, RED ,DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    Paint_HLine(30, 60,240-60, GREEN);



}

int loopCount = 0;

void Loop() {
    //int num = (loopCount / 100 ) %9;

    float sa     = (loopCount * -0.5f);
    float ea = (loopCount * 1.0f);

    Paint_fillArc(120, 120, 80, 90, sa, ea, GREEN);

    // Paint_DrawLine(loopCount, loopCount, 10, 20, YELLOW, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
    //Paint_DrawString_EN(123, 123, "WAVESHARE",&Font16,  BLACK, GREEN);
    //Paint_DrawNum(100, 200, num, &Font24,BLACK, GREEN);

    loopCount++;
}
