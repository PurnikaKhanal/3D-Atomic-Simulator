#include <graphics.h>
#include <math.h>
#include <stdio.h>

#define PI 3.1415926535

/* Normalize a vector */
void normalize(float *x, float *y, float *z) {
    float len = sqrt((*x)*(*x) + (*y)*(*y) + (*z)*(*z));
    *x /= len;
    *y /= len;
    *z /= len;
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    int cx = 320, cy = 240;   // Sphere center (screen)
    int r = 100;              // Radius

    /* Light position */
    float Lx = 200, Ly = 100, Lz = 200;

    /* Lighting constants */
    float Ka = 0.2;   // ambient coefficient
    float Kd = 0.8;   // diffuse coefficient
    float Il = 1.0;   // light intensity

    int x, y;

    for (x = -r; x <= r; x++) {
        for (y = -r; y <= r; y++) {

            if (x*x + y*y <= r*r) {

                /* Sphere surface Z (positive hemisphere) */
                float z = sqrt(r*r - x*x - y*y);

                /* Surface normal */
                float Nx = x;
                float Ny = y;
                float Nz = z;
                normalize(&Nx, &Ny, &Nz);

                /* Light direction */
                float Ldx = Lx - x;
                float Ldy = Ly - y;
                float Ldz = Lz - z;
                normalize(&Ldx, &Ldy, &Ldz);

                /* Lambertian diffuse term */
                float NL = Nx*Ldx + Ny*Ldy + Nz*Ldz;
                if (NL < 0) NL = 0;

                float Id = Kd * Il * NL;
                float Ia = Ka;

                float I = Ia + Id;
                if (I > 1) I = 1;

                /* Color computation (proton = red) */
                int red   = (int)(255 * I);
                int green = 0;
                int blue  = 0;

                putpixel(cx + x, cy - y, COLOR(red, green, blue));
            }
        }
    }

    getch();
    closegraph();
    return 0;
}
