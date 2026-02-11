#include <graphics.h>
#include <conio.h>
#include <math.h>
// this is perspective projection and will work only
// for circles not for ellipse
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
        putpixel(x2d + screen_x, screen_y - y2d, WHITE);
    }
}
int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    perspective_projection_cirlce(0, 0, 0, 90); // circle in 3d
    perspective_projection_cirlce(0, 80, 50, 40);
    getch();
    closegraph();
}
