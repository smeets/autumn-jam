#ifndef GAME_H
#define GAME_H

#include "core/camera.h"
#include "cube.h"

struct Player {};

struct State {
    CubeData cube[4];

  public:
    void enter();
    State* update(float dt);
    void exit();
};

struct Game {
    Player players[4];
    Camera camera;
    State* current_state;

  public:
    Game();
    void init();
    bool update(float dt);
    void reset();
};

#endif
