#include "camera.h"
#include <bgfx/bgfx.h>
#include <bx/bx.h>

/* Camera(10, 800, 600, 0.01f, 1000.0f) */
Camera::Camera(float ortho_size, float width, float height, float near,
               float far)
    : aspect(width / height), near_plane(near), far_plane(far),
      screen_width(width), screen_height(height), size(ortho_size) {

    resize(screen_width, screen_height);

    eye[0] = 0.0f;
    eye[1] = 0.0f;
    eye[2] = -35.0f;
}

void Camera::resize(float width, float height) {
    screen_width = width;
    screen_height = height;
    aspect = screen_width / screen_height;

    float left = -size * aspect;
    float right = size * aspect;
    float bottom = -size;
    float top = size;

    bx::mtxOrtho(proj, left, right, bottom, top, near_plane, far_plane, 0,
                 bgfx::getCaps()->homogeneousDepth);
}
