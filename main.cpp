#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#define ISASIM (1)

#include "LCD_Driver.h"


extern void sketch_setup();
extern void sketch_loop();

int main()
{
    const int factor = 1;
    const int screen_width = LCD_WIDTH * factor;
    const int screen_height = LCD_HEIGHT * factor;


    Image img = {
        .data = asim_buffer,
        .width = LCD_WIDTH,
        .height = LCD_HEIGHT,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R5G6B5
    };


    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(screen_width, screen_height, "ASIM window");

    sketch_setup();

    SetTargetFPS(60);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        sketch_loop();
        BeginDrawing();
            //ClearBackground(RAYWHITE);
            Texture2D texture = LoadTextureFromImage(img);
            DrawTexture(texture, 0, 0, WHITE);
            //DrawText("HI THERE ", 50, 80, 20, LIGHTGRAY);
        EndDrawing();
    }
    return 0;
}
