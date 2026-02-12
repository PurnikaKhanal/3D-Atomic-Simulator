#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <cmath>
#include <windows.h>
#include "graphics_transformations.h"
#include "lightning.h"
#include "pers_zbuffer.h"

// Global variables
int screenCenterX, screenCenterY;
float animationAngle = 0.0f;
float zoom = 1.0f;
float rotationX = 0.0f;
float rotationY = 0.0f;
int lastMouseX = 0, lastMouseY = 0;
bool mouseActive = false;

const float NUCLEUS_RADIUS = 10;
const float INNER_ORBIT_RADIUS = 60;   // First shell (2 electrons)
const float OUTER_ORBIT_RADIUS = 100;  // Second shell (4 electrons)
const float PARTICLE_RADIUS = 5;

// Note: rotation/input helpers are implemented inline in graphics_transformations.h

// Draw nucleus with protons and neutrons with transformation
void drawNucleus() {
    int protonCount = 6;  // Carbon has 6 protons
    int neutronCount = 6; // Carbon-12 has 6 neutrons
    
    // Draw protons (distributed around nucleus)
    for (int i = 0; i < protonCount; i++) {
        float angle = (360.0f / protonCount) * i * 3.14159265f / 180.0f;
        int px = (int)(NUCLEUS_RADIUS * 0.6f * cos(angle));
        int py = (int)(NUCLEUS_RADIUS * 0.6f * sin(angle));
        
        // Apply zoom and rotation
        px = (int)(px * zoom);
        py = (int)(py * zoom);
        rotatePoint(px, py, rotationX, rotationY);
        
        px += screenCenterX;
        py += screenCenterY;
        draw_proton(px, py, (int)(PARTICLE_RADIUS * zoom));
    }
    
    // Draw neutrons (distributed around nucleus, offset)
    for (int i = 0; i < neutronCount; i++) {
        float angle = (360.0f / neutronCount) * i * 3.14159265f / 180.0f + 0.5f;
        int nx = (int)(NUCLEUS_RADIUS * 0.8f * cos(angle));
        int ny = (int)(NUCLEUS_RADIUS * 0.8f * sin(angle));
        
        // Apply zoom and rotation
        nx = (int)(nx * zoom);
        ny = (int)(ny * zoom);
        rotatePoint(nx, ny, rotationX, rotationY);
        
        nx += screenCenterX;
        ny += screenCenterY;
        draw_neutron(nx, ny, (int)(PARTICLE_RADIUS * zoom));
    }
}

// Draw inner shell electrons (2 electrons, diametrically opposite)
void drawInnerShellElectrons() {
    for (int i = 0; i < 2; i++) {
        // Two electrons on opposite sides of the nucleus
        float angle = (180.0f * i + animationAngle) * 3.14159265f / 180.0f;
        int ex = (int)(INNER_ORBIT_RADIUS * zoom * cos(angle));
        int ey = (int)(INNER_ORBIT_RADIUS * zoom * sin(angle));
        
        // Apply rotation
        rotatePoint(ex, ey, rotationX, rotationY);
        
        ex += screenCenterX;
        ey += screenCenterY;
        draw_electron(ex, ey, (int)(PARTICLE_RADIUS * zoom - 1));
    }
}

// Draw outer shell electrons (4 electrons, square configuration)
void drawOuterShellElectrons() {
    for (int i = 0; i < 4; i++) {
        // Four electrons distributed around nucleus
        float angle = (90.0f * i + animationAngle * 0.7f) * 3.14159265f / 180.0f;
        int ex = (int)(OUTER_ORBIT_RADIUS * zoom * cos(angle));
        int ey = (int)(OUTER_ORBIT_RADIUS * zoom * sin(angle));
        
        // Apply rotation
        rotatePoint(ex, ey, rotationX, rotationY);
        
        ex += screenCenterX;
        ey += screenCenterY;
        draw_electron(ex, ey, (int)(PARTICLE_RADIUS * zoom - 1));
    }
}

// Draw orbital paths as reference
void drawOrbitalPaths() {
    // Inner orbital path
    float innerTheta;
    for (innerTheta = 0; innerTheta <= 360; innerTheta += 5.0f) {
        float rad = innerTheta * 3.14159265f / 180.0f;
        int x = (int)(INNER_ORBIT_RADIUS * zoom * cos(rad));
        int y = (int)(INNER_ORBIT_RADIUS * zoom * sin(rad));
        
        rotatePoint(x, y, rotationX, rotationY);
        x += screenCenterX;
        y += screenCenterY;
        putpixel(x, y, DARKGRAY);
    }
    
    // Outer orbital path
    float outerTheta;
    for (outerTheta = 0; outerTheta <= 360; outerTheta += 5.0f) {
        float rad = outerTheta * 3.14159265f / 180.0f;
        int x = (int)(OUTER_ORBIT_RADIUS * zoom * cos(rad));
        int y = (int)(OUTER_ORBIT_RADIUS * zoom * sin(rad));
        
        rotatePoint(x, y, rotationX, rotationY);
        x += screenCenterX;
        y += screenCenterY;
        putpixel(x, y, DARKGRAY);
    }
}

// Main animation loop
void animateAtom() {
    int x, y;
    int maxIterations = 3600; // ~60 seconds at 60 FPS
    int iteration = 0;
    int page = 0; // for double-buffering: 0 and 1
    
    while (iteration < maxIterations && !(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
        // Handle keyboard and mouse input
        handleKeyboardInput();
        handleMouseInput();

        // Draw to the active page (double-buffering)
        setactivepage(page);
        cleardevice();
        
        // Draw title (smaller clear font)
        setcolor(WHITE);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        outtextxy(10, 8, "Carbon Atom (C-12): 6 Protons, 6 Neutrons, 6 Electrons");

        // Draw instructions (smaller, spaced)
        setcolor(LIGHTGRAY);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        outtextxy(10, 26, "WASD: Rotate | Q/E or +/-: Zoom | R: Reset | ESC: Exit | Mouse: Drag");
        outtextxy(10, 42, "Right-Click to zoom in | Middle-Click to zoom out");
        
        // Draw orbital paths
        drawOrbitalPaths();
        
        // Draw nucleus
        drawNucleus();
        
        // Draw electron shells
        drawInnerShellElectrons();
        drawOuterShellElectrons();
        
        // Draw counter/stats
        char stats[200];
        sprintf(stats, "Frame: %d | Zoom: %.2fx | RotX: %.1f | RotY: %.1f", 
                iteration, zoom, rotationX, rotationY);
        setcolor(YELLOW);
        outtextxy(10, getmaxy() - 30, stats);
        
        // Update animation
        animationAngle += 2.0f;
        if (animationAngle >= 360.0f) {
            animationAngle -= 360.0f;
        }
        
        iteration++;

        // Show the page we just drew and flip
        setvisualpage(page);
        page = 1 - page;

        // Delay to limit framerate (~60 FPS)
        delay(16);
    }
}

int main() {
    int gdriver = DETECT, gmode, errorcode;
    
    // Initialize graphics
    initgraph(&gdriver, &gmode, "");
    errorcode = graphresult();
    
    if (errorcode != grOk) {
        printf("Graphics Error: %s\n", grapherrormsg(errorcode));
        return 1;
    }
    
    // Get screen dimensions
    int maxX = getmaxx();
    int maxY = getmaxy();
    screenCenterX = maxX / 2;
    screenCenterY = maxY / 2;
    
    // Initialize z-buffer
    initialize_zbuffer();
    
    // Set background color
    setbkcolor(BLACK);
    cleardevice();
    
    // Run animation
    animateAtom();
    
    // Cleanup
    if (kbhit()) {
        getch();
    }
    
    closegraph();
    return 0;
}
