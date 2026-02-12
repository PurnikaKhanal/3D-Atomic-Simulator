#ifndef PERS_ZBUFFER_H
#define PERS_ZBUFFER_H

#include <graphics.h>
#include <conio.h>
#include <math.h>
// declearing variables for z buffer
#define width 200
#define height 150
int zbuffer[width][height];
void initialize_zbuffer()
{
    int x, y;
    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            zbuffer[x][y] = 9999; // large value,far depth
        }
    }
}
void zbuffer_putpixel(int x, int y, int z, int color)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;
    if (z < zbuffer[x][y])
    {
        zbuffer[x][y] = z;
        putpixel(x, y, color);
    }
}
void pers(int x, int y, int z, int d, int *x2d, int *y2d)
{
    // as there is division by zero risk
    if (z + d == 0)
    {
        *x2d = 0;
        *y2d = 0;
        return;
    }
    *x2d = (int)((x * d) / (z + d));
    *y2d = (int)((y * d) / (z + d));
}
void perspective_projection_cirlce(int cx, int cy, int cz, int r)
{
    int x2d, y2d;
    float theta;
    // Screen center
    int screen_x = getmaxx() / 2;
    int screen_y = getmaxy() / 2;
    for (theta = 0; theta <= 360; theta += 1.0)
    {
        // convert to radians
        float rad = theta * 3.14159265 / 180.0;
        float x = cx + r * cos(rad);
        float y = cy + r * sin(rad);
        float z = cz;
        pers(x, y, z, 200, &x2d, &y2d);
        int sx = x2d + screen_x;
        int sy = screen_y - y2d;

        int zb_x = sx * width / getmaxx();
        int zb_y = sy * height / getmaxy();

        zbuffer_putpixel(zb_x, zb_y, cz, WHITE);
    }
}
#endif