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

#endif