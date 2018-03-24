#ifndef GAME_H
#define GAME_H

#include "content/character.h"
#include "core/camera.h"
#include "physics/line.h"

struct Player {};

struct State {
    Content::Character players[4];
    LineSegment segments[10];

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
