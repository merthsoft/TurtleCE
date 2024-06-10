#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <graphx.h>
#include <string.h>
#include <debug.h>

#include "turtle.h"

float* sin_table;
float* cos_table;

#define angle_to_rads(a) (a * 0.017453292519943295f)

void Turtle_Init()
{
    sin_table = malloc(360 * sizeof(float));
    cos_table = malloc(360 * sizeof(float));

    for (int i = 0; i < 361; i++)
    {
        float r = angle_to_rads(i);
        sin_table[i] = sin(r);
        cos_table[i] = cos(r);
    }
}

void Turtle_End()
{
    free(sin_table);
    free(cos_table);
}

Turtle* Turtle_Create()
{
    Turtle* t = malloc(sizeof(Turtle));
    memset(t, 0, sizeof(Turtle));
    return t;
}

void Turtle_Delete(Turtle* t)
{
    free(t);
}

float fwrap(float x, float min, float max) {
    if (min > max)
        return fwrap(x, max, min);
    return (x >= 0 ? min : max) + fmodf(x, max - min);
}

void move(Turtle* t, float newX, float newY)
{
    float oldX = t->x;
    float oldY = t->y;

    t->x = newX;
    t->y = newY;

    if (t->pen)
    {
        gfx_SetColor(t->color);
        gfx_Line(oldX, oldY, newX, newY);
    }

    if (t->wrap)
    {
        t->x = fwrap(newX, 0, GFX_LCD_WIDTH);
        t->y = fwrap(newY, 0, GFX_LCD_HEIGHT);

        if (t->pen)
        {
            if (t->x < newX)
                oldX = fwrap(oldX, 0, GFX_LCD_WIDTH) - GFX_LCD_WIDTH;
            else if (t->x > newX)
                oldX = fwrap(oldX, 0, GFX_LCD_WIDTH) + GFX_LCD_WIDTH;
            
            if (t->y < newY)
                oldY = fwrap(oldY, 0, GFX_LCD_HEIGHT) - GFX_LCD_HEIGHT;
            else if (t->y > newY)
                oldY = fwrap(oldY, 0, GFX_LCD_HEIGHT) + GFX_LCD_HEIGHT;

            gfx_Line(oldX, oldY, t->x, t->y);
        }
    }
}

void Turtle_Forward(Turtle* t, float amount)
{
    int angle = (int)t->angle;
    float deltaX = amount * sin_table[angle];
    float deltaY = amount * -cos_table[angle];

    move(t, t->x + deltaX, t->y + deltaY);
}

void clip_angle(Turtle* t)
{
    t->angle = fwrap(t->angle, 0, 360);
}

void Turtle_Right(Turtle* t, float angle)
{
    t->angle -= angle;
    clip_angle(t);
}

void Turtle_Left(Turtle* t, float angle)
{
    t->angle += angle;
    clip_angle(t);
}

void Turtle_SetAngle(Turtle* t, float angle)
{
    t->angle = angle;
    clip_angle(t);
}

void Turtle_Goto(Turtle* t, float x, float y)
{
    move(t, x, y);
}

void Turtle_Teleport(Turtle* t, float x, float y)
{
    bool pen = t->pen;
    t->pen = false;
    move(t, x, y);
    t->pen = pen;
}

void Turtle_SetPen(Turtle* t, bool pen)
{
    t->pen = !!pen;
}

void Turtle_PenUp(Turtle* t)
{
    t->pen = 0;
}

void Turtle_PenDown(Turtle* t)
{
    t->pen = 1;
}

void Turtle_SetColor(Turtle* t, uint8_t color)
{
    t->color = color;
}

void Turtle_SetWrap(Turtle* t, bool wrap)
{
    t->wrap = !!wrap;
}