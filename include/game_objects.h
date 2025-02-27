#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include "raylib.h"
#include <iostream>

class Paddle {
public:
  Paddle(float speed, bool rightPaddle) {
    ResetInitialValues(speed, rightPaddle);
  };

  float x, y;
  float speed;
  float width;
  float height;

  Rectangle GetRect() { return {x - width / 2, y - height / 2, width, height}; }

  void Draw() { DrawRectangleRec(GetRect(), WHITE); }

  void MoveUp() {
    if (y - height / 2 > 0) {
      y -= speed * GetFrameTime();
    }
  }

  void MoveDown() {
    if (y + height / 2 < GetScreenHeight()) {
      y += speed * GetFrameTime();
    }
  }

  void ResetInitialValues(float speed, bool rightPaddle) {
    x = rightPaddle ? GetScreenWidth() - 50 : 50;
    y = GetScreenHeight() / 2;
    this->speed = speed; // Fix self-assignment issue
    width = 10;
    height = 100;
  }
};

class Ball {
public:
  Ball(float speed) { ResetInitialValues(speed); };

  float x, y;
  float speedX, speedY;
  float radius;

  void Draw() { DrawCircle(x, y, radius, RED); }

  void Move() {
    x += speedX * GetFrameTime();
    y += speedY * GetFrameTime();

    if (y - radius < 0) {
      y = radius;
      speedY *= -1;
    }
    if (y + radius > GetScreenHeight()) {
      y = GetScreenHeight() - radius;
      speedY *= -1;
    }
  }

  void ResetInitialValues(float speed) {
    x = GetScreenWidth() / 2.0f;
    y = GetScreenHeight() / 2.0f;
    radius = 5;
    speedX = speed;
    speedY = speed;
    if (GetRandomValue(0, 1) == 0) {
      speedX *= -1;
    }
    if (GetRandomValue(0, 1) == 0) {
      speedY *= -1;
    }
  }

  void HandleCollision(Paddle *paddle) {
    if (CheckCollisionCircleRec({x, y}, radius, paddle->GetRect())) {
      if ((paddle->x < x && speedX < 0) || (paddle->x > x && speedX > 0)) {
        speedX *= -1.1f;
        speedY = (y - paddle->y) / (paddle->height / 2) * speedX;
      }
    }
  }

  void CheckWinCondition(bool &paused, std::string &winnerText, int &leftScore,
                         int &rightScore) {
    if (x < 0 && !paused) {
      paused = true;
      winnerText = "Right player wins!";
      rightScore++;
    }

    if (x > GetScreenWidth() && !paused) {
      paused = true;
      winnerText = "Left player wins!";
      leftScore++;
    }
  }
};

#endif // GAME_OBJECTS_H
