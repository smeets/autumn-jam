#include <bx/math.h>

struct Camera {
    float proj[16];
    float eye[3];

    float aspect;
    float near_plane, far_plane;
    float screen_width, screen_height;
    float size;

    Camera(float ortho_size, float width, float height, float near, float far);
    void resize(float width, float height);
};
