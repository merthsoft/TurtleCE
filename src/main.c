#include <ti/getcsc.h>
#include <sys/util.h>
#include <graphx.h>
#include <math.h>
#include <string.h>
#include <sys/rtc.h>
#include <stdlib.h>
#include <debug.h>

#include "turtle.h"
#include "palette.h"

#define MAX_TURTLES 100

int mode = 0;
int palette = 0;
int palette_choice = 0;
int num_turtles = 15;
int angle = 9;
int speed = 6;

bool color_change = true;
bool shift = true;

uint8_t color = 0;

Turtle** turtles;

void init_random_walk()
{
    for (int i = 0; i < num_turtles; i++) {
        Turtle* t = turtles[i];

        Turtle_PenUp(t);

        Turtle_SetColor(t, random() % 255 + 1);
        Turtle_Goto(t, random() % GFX_LCD_WIDTH, random() % GFX_LCD_HEIGHT);
        Turtle_SetAngle(t, random());
        
        Turtle_SetWrap(t, 1);
        Turtle_PenDown(t);
    }
}

void init_spiral()
{
    for (int i = 0; i < num_turtles; i++) {
        Turtle* t = turtles[i];

        Turtle_PenUp(t);

        Turtle_SetColor(t, 255.0f / num_turtles * i + 1);
        Turtle_Goto(t, GFX_LCD_WIDTH / 2, GFX_LCD_HEIGHT / 2);
        Turtle_SetAngle(t, 360.0f / num_turtles * i);

        Turtle_SetWrap(t, 0);
        Turtle_PenDown(t);
    }
}

void random_walk(Turtle* t, int angle, int speed)
{
    Turtle_Left(t, angle - random() % (angle * 2));
    Turtle_Forward(t, random() % speed + speed);
}

void spiral(Turtle* t, int angle, int speed)
{
    Turtle_Left(t, angle);
    Turtle_Forward(t, speed);
}

sk_key_t handle_shared_keys(bool blocking)
{
    sk_key_t key = 0;
    bool init = false;
    bool rebuild_palette = false;

    do {
        key = os_GetCSC();
    } while (blocking && !key);
    
    switch (key)
    {
        case sk_Yequ:
            init = true;
            mode = 0;
            angle = 9;
            speed = 6;
            break;
        case sk_Window:
            init = true;
            mode = 1;
            angle = 45;
            speed = 5;
            break;
        case sk_Add:
            init = true;
            num_turtles++;
            break;
        case sk_Sub:
            init = true;
            num_turtles--;
            break;
        case sk_Up:
            speed++;
            break;
        case sk_Down:
            speed--;
            break;
        case sk_Left:
            angle--;
            break;
        case sk_Right:
            angle++;
            break;
        case sk_0:
            for (int i = 0; i < num_turtles; i++)
                Turtle_Teleport(turtles[i], GFX_LCD_WIDTH / 2, GFX_LCD_HEIGHT / 2);
            break;
        case sk_DecPnt:
            for (int i = 0; i < num_turtles; i++)
                Turtle_Teleport(turtles[i], random() % GFX_LCD_WIDTH, random() % GFX_LCD_HEIGHT);
            break;
        case sk_1:
            for (int i = 0; i < num_turtles; i++)
                turtles[i]->color = 1;
            break;
        case sk_2:
            for (int i = 0; i < num_turtles; i++)
                turtles[i]->color = random() % 254 + 1;
            break;
        case sk_3:
            for (int i = 0; i < num_turtles; i++)
                turtles[i]->color = 255.0f / num_turtles * i + 1;
            break;
        case sk_Mode:
            mode = !mode;
            break;
        case sk_Alpha:
            for (int i = 0; i < num_turtles; i++)
                turtles[i]->wrap = !turtles[i]->wrap;
            break;
        case sk_Mul:
            color_change = !color_change;
            break;
        case sk_Div:
            shift = !shift;
            break;
        case sk_4:
            angle = 9;
            speed = 6;
            break;
        case sk_5:
            angle = 60;
            speed = 20;
            break;
        case sk_6:
            angle = 90;
            speed = 20;
            break;
        case sk_7:
            angle = 45;
            speed = 5;
            break;
        case sk_8:
            angle = 180;
            speed = 10;
            break;
        case sk_9:
            angle = 1;
            speed = 1;
            break;
        case sk_Comma:
            palette = 0;
            rebuild_palette = true;
            break;
        case sk_Sin:
            if (palette == 1)
                palette_choice = !palette_choice;
            else {
                palette = 1;
                palette_choice = 0;
            }
            rebuild_palette = true;
            break;
        case sk_Cos:
            if (palette == 2)
                palette_choice = !palette_choice;
            else {
                palette = 2;
                palette_choice = 0;
            }
            rebuild_palette = true;
            break;
        case sk_Tan:
            if (palette == 3)
                palette_choice = (palette_choice + 1) % 6;
            else {
                palette = 3;
                palette_choice = 0;
            }
            rebuild_palette = true;
            break;
        case sk_Apps:
            if (palette == 4)
                palette_choice = !palette_choice;
            else {
                palette = 4;
                palette_choice = 0;
            }
            rebuild_palette = true;
            break;
        case sk_LParen:
            color -= 16;
            rebuild_palette = true;
            break;
        case sk_RParen:
            color += 16;
            rebuild_palette = true;
            break;
        case sk_Chs:
            angle = random() % 360;
            speed = random() % 15;
            break;
    }

    if (rebuild_palette) {
        switch (palette)
        {
            case 0:
                palette_rainbow(gfx_palette);
                break;
            case 1:
                palette_value(gfx_palette, color, palette_choice);
                break;
            case 2:
                palette_saturation(gfx_palette, color, palette_choice);
                break;
            case 3:           
                palette_spectrum(gfx_palette, color, palette_choice + 1);
                break;
            case 4:
                palette_gray(gfx_palette, palette_choice);
                break;
        }
    }

    if (num_turtles < 1)
        num_turtles = 1;
    else if (num_turtles > MAX_TURTLES)
        num_turtles = MAX_TURTLES;
    
    if (init)
    {
        switch (mode)
        {
            case 0:
                init_spiral();
                break;
            case 1:
                init_random_walk();
                break;
        }
    }

    return key;
}

void display_vars()
{
    char* text = malloc(500);
    gfx_SetDrawScreen();

    bool exit = false;
    while (!exit)
    {
        gfx_FillScreen(0);
        gfx_SetTextFGColor(1);

        gfx_PrintStringXY("Vars:", 0, 0);

        snprintf(text, 500, "Mode: %s", mode ? "Random walk" : "Spiral");
        gfx_PrintStringXY(text, 0, 10);

        snprintf(text, 500, "Num: %i", num_turtles);
        gfx_PrintStringXY(text, 0, 20);

        snprintf(text, 500, "Speed: %i", speed);
        gfx_PrintStringXY(text, 0, 30);

        snprintf(text, 500, "Angle: %i", angle);
        gfx_PrintStringXY(text, 0, 40);

        snprintf(text, 500, "Wrap: %s", turtles[0]->wrap ? "Yes" : "No");
        gfx_PrintStringXY(text, 0, 50);

        snprintf(text, 500, "Color change: %s", color_change ? "Yes" : "No");
        gfx_PrintStringXY(text, 0, 60);

        snprintf(text, 500, "Palette shift: %s", shift ? "Yes" : "No");
        gfx_PrintStringXY(text, 0, 70);

        snprintf(text, 500, "Palette: %i", palette);
        gfx_PrintStringXY(text, 0, 80);

        snprintf(text, 500, "Palette color: %i", color);
        gfx_PrintStringXY(text, 0, 90);

        snprintf(text, 500, "Palette choice: %i", palette_choice);
        gfx_PrintStringXY(text, 0, 100);

        gfx_PrintStringXY("Palette:", 0, 110);

        int x = 0;
        int y = 120;
        for (int index = 0; index < 256; index++)
        {
            gfx_SetColor(index);
            gfx_FillRectangle_NoClip(x, y, 5, 5);
            x += 5;
            if (x == 160)
            {
                x = 0;
                y += 5;
            }
        }

        sk_key_t key = handle_shared_keys(true);
        if (key == sk_Del || key == sk_Enter || key == sk_Vars || key == sk_Clear)
            exit = true;
    }

    gfx_SwapDraw();
    free(text);
    return;
}

int main(void)
{
    gfx_Begin();
    gfx_FillScreen(0);
    gfx_SetTextFGColor(5);

    gfx_PrintStringXY("Initializing...", 0, 0);
    Turtle_Init();

    gfx_palette[0] = 0;
    palette_rainbow(gfx_palette);

    srand(rtc_Time());

    turtles = malloc(MAX_TURTLES * sizeof(void*));
    memset(turtles, 0, MAX_TURTLES * sizeof(void*));

    for (int i = 0; i < MAX_TURTLES; i++) {
        turtles[i] = Turtle_Create();
    }

    bool move = true;

    init_spiral();

    gfx_FillScreen(0);
    gfx_SetDrawBuffer();

    bool exit = false;
    while (!exit)
    {
        gfx_BlitScreen();

        for (int i = 0; i < num_turtles; i++) {
            Turtle* t = turtles[i];
                
            if (move) {
                switch (mode)
                {
                    case 0:
                        spiral(t, angle, speed);
                        break;
                    case 1:
                        random_walk(t, angle, speed);
                        break;
                }

                if (color_change)
                    t->color = (t->color + 1) % 255 + 1;
            }

            if (t->x >= 0 && t->x < GFX_LCD_WIDTH
                && t->y >= 0 && t->y < GFX_LCD_HEIGHT)
            {
                gfx_SetColor(t->color);
                gfx_SetPixel(t->x, t->y);
            }
        }

        sk_key_t key = handle_shared_keys(false);
        switch (key)
        {
            case sk_Vars:
                display_vars();
                break;
            case sk_Del:
                exit = true;
                break;
            case sk_Clear:
                gfx_FillScreen(0);
                break;
            case sk_Enter:
                move = !move;
                break;
        }

        gfx_SwapDraw();

        if (shift)
            palette_shift(gfx_palette);
    }

    for (int i = 0; i < MAX_TURTLES; i++) {
        Turtle* t = turtles[i];
        Turtle_Delete(t);
    }

    free(turtles);

    gfx_End();
    Turtle_End();

    return 0;
}
