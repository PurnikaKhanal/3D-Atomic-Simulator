#ifndef GRAPHICS_TRANSFORMATIONS_H
#define GRAPHICS_TRANSFORMATIONS_H

#include <graphics.h>
#include <cmath>
#include <windows.h>

// Extern globals defined in main.cpp that transformation/input helpers use
extern float rotationX;
extern float rotationY;
extern float zoom;
extern int lastMouseX;
extern int lastMouseY;
extern bool mouseActive;

// Apply 2D rotation transformation to a point (in-place).
inline void rotatePoint(int& x, int& y, float angleX, float angleY) {
    float radY = angleY * 3.14159265f / 180.0f;
    float cosY = cosf(radY);
    float sinY = sinf(radY);

    int tempX = x;
    x = (int)(tempX * cosY - y * sinY);
    y = (int)(tempX * sinY + y * cosY);

    float radX = angleX * 3.14159265f / 180.0f;
    float cosX = cosf(radX);
    int tempY = y;
    y = (int)(tempY * cosX);
}

// Handle keyboard input for interactive control (uses extern globals).
inline void handleKeyboardInput() {
    if (GetAsyncKeyState('W') & 0x8000) rotationX -= 2.0f;
    if (GetAsyncKeyState('S') & 0x8000) rotationX += 2.0f;
    if (GetAsyncKeyState('A') & 0x8000) rotationY -= 2.0f;
    if (GetAsyncKeyState('D') & 0x8000) rotationY += 2.0f;

    if (GetAsyncKeyState('Q') & 0x8000 || GetAsyncKeyState(187) & 0x8000) {
        zoom += 0.05f;
        if (zoom > 3.0f) zoom = 3.0f;
    }
    if (GetAsyncKeyState('E') & 0x8000 || GetAsyncKeyState(189) & 0x8000) {
        zoom -= 0.05f;
        if (zoom < 0.5f) zoom = 0.5f;
    }

    if (GetAsyncKeyState('R') & 0x8000) {
        rotationX = 0.0f;
        rotationY = 0.0f;
        zoom = 1.0f;
    }

    if (rotationY >= 360.0f) rotationY -= 360.0f;
    if (rotationY < 0.0f) rotationY += 360.0f;
    if (rotationX > 89.0f) rotationX = 89.0f;
    if (rotationX < -89.0f) rotationX = -89.0f;
}

// Handle mouse input for interactive control (uses extern globals).
inline void handleMouseInput() {
    int mouseX, mouseY;

    if (ismouseclick(WM_LBUTTONDOWN)) {
        getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        mouseActive = true;
    }

    if (ismouseclick(WM_MOUSEMOVE)) {
        getmouseclick(WM_MOUSEMOVE, mouseX, mouseY);
        if (mouseActive) {
            int deltaX = mouseX - lastMouseX;
            int deltaY = mouseY - lastMouseY;

            rotationY += deltaX * 0.5f;
            rotationX += deltaY * 0.5f;

            if (rotationY >= 360.0f) rotationY -= 360.0f;
            if (rotationY < 0.0f) rotationY += 360.0f;
            if (rotationX > 89.0f) rotationX = 89.0f;
            if (rotationX < -89.0f) rotationX = -89.0f;

            lastMouseX = mouseX;
            lastMouseY = mouseY;
        }
    }

    if (ismouseclick(WM_LBUTTONUP)) {
        getmouseclick(WM_LBUTTONUP, mouseX, mouseY);
        mouseActive = false;
    }

    if (ismouseclick(WM_RBUTTONDOWN)) {
        getmouseclick(WM_RBUTTONDOWN, mouseX, mouseY);
        zoom += 0.1f;
        if (zoom > 3.0f) zoom = 3.0f;
    }

    if (ismouseclick(WM_MBUTTONDOWN)) {
        getmouseclick(WM_MBUTTONDOWN, mouseX, mouseY);
        zoom -= 0.1f;
        if (zoom < 0.5f) zoom = 0.5f;
    }
}

#endif // GRAPHICS_TRANSFORMATIONS_H
