#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#define ISASIM (1)

#include "AsimDriver.h"
#include <string.h>

extern void sketch_setup();
extern void sketch_loop();


const uint16_t LCD_WIDTH =  240;
const uint16_t LCD_HEIGHT =  240;


int main()
{
    // run sketch setup first, so driver kan initialize and set up the display driver
    sketch_setup();

    const int factor = 1;
    const int screen_width = LCD_WIDTH * factor;
    const int screen_height = LCD_HEIGHT * factor;

    Image img = {
        .data = g_asim_buffer,
        .width = LCD_WIDTH,
        .height = LCD_HEIGHT,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R5G6B5
    };


    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(screen_width, screen_height, "ASIM window");


    SetTargetFPS(60);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        sketch_loop();
        BeginDrawing();
            Texture2D texture = LoadTextureFromImage(img);
            DrawTexture(texture, 0, 0, WHITE);
            //DrawText("FPS ", 50, 80, 20, LIGHTGRAY);
        EndDrawing();
    }
    return 0;
}
