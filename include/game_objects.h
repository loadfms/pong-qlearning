#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include "raylib.h"

struct Ball {
  float x, y;
  float speedX, speedY;
  float radius;

  void Draw() { DrawCircle(x, y, radius, RED); }
};

struct Paddle {
  float x, y;
  float speed;
  float width, height;

  Rectangle GetRect() { return {x - width / 2, y - height / 2, width, height}; }

  void Draw() { DrawRectangleRec(GetRect(), WHITE); }
};

#endif // GAME_OBJECTS_H
