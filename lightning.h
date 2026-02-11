#ifndef LIGHTNING_H
#define LIGHTNING_H



/* Normalize a 3D vector to unit length */
void normalize(float *x, float *y, float *z)
{
    float len = sqrt((*x)*(*x) + (*y)*(*y) + (*z)*(*z));
    if (len != 0)
    {
        *x /= len;
        *y /= len;
        *z /= len;
    }
}

/* Diffuse lighting using Lambert's law */
float diffuseLight(float Nx, float Ny, float Nz,
                   float Lx, float Ly, float Lz,
                   float Kd, float Il)
{
    normalize(&Nx, &Ny, &Nz);
    normalize(&Lx, &Ly, &Lz);

    float dot = Nx*Lx + Ny*Ly + Nz*Lz;
    if (dot < 0) dot = 0;

    return Kd * Il * dot;
}

/* Specular lighting using Phong model */
float specularLight(float Nx, float Ny, float Nz,
                    float Lx, float Ly, float Lz,
                    float Vx, float Vy, float Vz,
                    float Ks, float Il, int ns)
{
    normalize(&Nx, &Ny, &Nz);
    normalize(&Lx, &Ly, &Lz);
    normalize(&Vx, &Vy, &Vz);

    float dotNL = Nx*Lx + Ny*Ly + Nz*Lz;
    if (dotNL < 0) return 0;

    /* Reflection vector R = 2(N·L)N − L */
    float Rx = 2*dotNL*Nx - Lx;
    float Ry = 2*dotNL*Ny - Ly;
    float Rz = 2*dotNL*Nz - Lz;

    normalize(&Rx, &Ry, &Rz);

    float dotRV = Rx*Vx + Ry*Vy + Rz*Vz;
    if (dotRV < 0) dotRV = 0;

    return Ks * Il * pow(dotRV, ns);
}

/* Draw a shaded sphere using Phong illumination */
void drawPhongSphere(int cx, int cy, int radius,
                     float Lx, float Ly, float Lz,
                     float Ka, float Kd, float Ks,
                     int ns, float Il,
                     float Rc, float Gc, float Bc)
{
    int x, y;

    for (x = -radius; x <= radius; x++)
    {
        for (y = -radius; y <= radius; y++)
        {
            if (x*x + y*y <= radius*radius)
            {
                float z = sqrt(radius*radius - x*x - y*y);

                /* Surface normal */
                float Nx = x;
                float Ny = y;
                float Nz = z;

                /* Light direction */
                float lx = Lx - x;
                float ly = Ly - y;
                float lz = Lz - z;

                /* Viewer direction (viewer at origin) */
                float vx = -x;
                float vy = -y;
                float vz = -z;

                /* Lighting components */
                float Ia = Ka * Il;
                float Id = diffuseLight(Nx, Ny, Nz,
                                        lx, ly, lz,
                                        Kd, Il);
                float Is = specularLight(Nx, Ny, Nz,
                                         lx, ly, lz,
                                         vx, vy, vz,
                                         Ks, Il, ns);

                float I = Ia + Id + Is;
                if (I > 1) I = 1;

                putpixel(cx + x, cy - y,
                         COLOR((int)(Rc*I),
                               (int)(Gc*I),
                               (int)(Bc*I)));
            }
        }
    }
}

/* Proton (Red) */
void draw_proton(int Pcx, int Pcy, int Pradius)
{
    float Lx = 200, Ly = 100, Lz = 200;
    float Ka = 0.2, Kd = 0.6, Ks = 0.4;
    int ns = 20;
    float Il = 1.0;

    drawPhongSphere(Pcx, Pcy, Pradius,
                    Lx, Ly, Lz,
                    Ka, Kd, Ks,
                    ns, Il,
                    255, 0, 0);
}

/* Neutron (White) */
void draw_neutron(int Ncx, int Ncy, int Nradius)
{
    float Lx = 200, Ly = 100, Lz = 200;
    float Ka = 0.2, Kd = 0.6, Ks = 0.4;
    int ns = 20;
    float Il = 1.0;

    drawPhongSphere(Ncx, Ncy, Nradius,
                    Lx, Ly, Lz,
                    Ka, Kd, Ks,
                    ns, Il,
                    255, 255, 255);
}

/* Electron (Sky Blue) */
void draw_electron(int Ecx, int Ecy, int Eradius)
{
    float Lx = 200, Ly = 100, Lz = 200;
    float Ka = 0.2, Kd = 0.6, Ks = 0.4;
    int ns = 20;
    float Il = 1.0;

    drawPhongSphere(Ecx, Ecy, Eradius,
                    Lx, Ly, Lz,
                    Ka, Kd, Ks,
                    ns, Il,
                    135, 206, 235);
}

#endif