#include "include/AI.h"
#include "include/QLearning.h"
#include "include/game_objects.h"
#include "raylib.h"
#include <string>

QLearning qlearningRight(100, 3, 0.1, 0.9, 0.2);
QLearning qlearningLeft(100, 3, 0.1, 0.9, 0.2);

int main(void) {
  InitWindow(800, 450, "raylib [core] example - basic window");
  SetWindowPosition(100, 100); // Set a specific position for the window
  SetWindowState(FLAG_VSYNC_HINT);

  const int speedFactor = 1;
  const char *winnerText = nullptr;
  const float initialBallSpeed = 200 * speedFactor;
  const float initialPaddleSpeed = 500 * speedFactor;
  int leftScore = 0;
  int rightScore = 0;
  bool paused = false;

  Ball ball;

  ball.x = GetScreenWidth() / 2.0f;
  ball.y = GetScreenHeight() / 2.0f;
  ball.radius = 5;
  ball.speedX = initialBallSpeed;
  ball.speedY = initialBallSpeed;
  if (GetRandomValue(0, 1) == 0) {
    ball.speedX *= -1;
  }
  if (GetRandomValue(0, 1) == 0) {
    ball.speedY *= -1;
  }

  Paddle leftPaddle;
  leftPaddle.x = 50;
  leftPaddle.y = GetScreenHeight() / 2;
  leftPaddle.width = 10;
  leftPaddle.height = 100;
  leftPaddle.speed = initialPaddleSpeed;

  Paddle rightPaddle;
  rightPaddle.x = GetScreenWidth() - 50;
  rightPaddle.y = GetScreenHeight() / 2;
  rightPaddle.width = 10;
  rightPaddle.height = 100;
  rightPaddle.speed = initialPaddleSpeed;

  while (!WindowShouldClose()) {
    MoveAI(rightPaddle, ball, qlearningRight, true);
    MoveAI(leftPaddle, ball, qlearningLeft, false);

    qlearningRight.UpdateEpsilon();

    ball.x += ball.speedX * GetFrameTime();
    ball.y += ball.speedY * GetFrameTime();

    // Check if the ball goes out of bounds vertically
    if (ball.y - ball.radius < 0) {
      ball.y = ball.radius; // Ensure the ball stays within the top boundary
      ball.speedY *= -1;    // Reverse the vertical direction
    }
    if (ball.y + ball.radius > GetScreenHeight()) {
      ball.y = GetScreenHeight() -
               ball.radius; // Ensure the ball stays within the bottom boundary
      ball.speedY *= -1;    // Reverse the vertical direction
    }

    /*if (ball.y > GetScreenHeight() || ball.y < 0) {*/
    /*  ball.speedY *= -1;*/
    /*}*/

    if (IsKeyDown(KEY_UP)) {
      if (leftPaddle.y - leftPaddle.height / 2 > 0) {
        leftPaddle.y -= leftPaddle.speed * GetFrameTime();
      }
    }

    if (IsKeyDown(KEY_DOWN)) {
      if (leftPaddle.y + leftPaddle.height / 2 < GetScreenHeight()) {
        leftPaddle.y += leftPaddle.speed * GetFrameTime();
      }
    }

    if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius,
                                leftPaddle.GetRect())) {
      if (ball.speedX < 0) {
        ball.speedX *= -1.1f;
        ball.speedY =
            (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
      }
    }

    if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius,
                                rightPaddle.GetRect())) {
      if (ball.speedX > 0) {
        ball.speedX *= -1.1f;
        ball.speedY =
            (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
      }
    }

    if (ball.x < 0 && !paused) {
      paused = true;
      winnerText = "Right player wins!";
      rightScore++;
    }

    if (ball.x > GetScreenWidth() && !paused) {
      paused = true;
      winnerText = "Left player wins!";
      leftScore++;
    }

    /*if (winnerText && IsKeyPressed(KEY_SPACE)) {*/
    if (winnerText && paused) {
      ball.x = GetScreenWidth() / 2.0f;
      ball.y = GetScreenHeight() / 2.0f;
      ball.speedX = initialBallSpeed;
      ball.speedY = initialBallSpeed;

      leftPaddle.y = GetScreenHeight() / 2;
      rightPaddle.y = GetScreenHeight() / 2;

      if (GetRandomValue(0, 1) == 0) {
        ball.speedX *= -1;
      }
      if (GetRandomValue(0, 1) == 0) {
        ball.speedY *= -1;
      }

      winnerText = nullptr;
      paused = false;
    }

    BeginDrawing();

    ClearBackground(BLACK);

    ball.Draw();
    leftPaddle.Draw();
    rightPaddle.Draw();

    std::string leftScoreText = std::to_string(leftScore);
    std::string rightScoreText = std::to_string(rightScore);

    DrawText(leftScoreText.c_str(),
             GetScreenWidth() / 4 - MeasureText(leftScoreText.c_str(), 30) / 2,
             10, 30, WHITE);

    DrawText(rightScoreText.c_str(),
             GetScreenWidth() * 3 / 4 -
                 MeasureText(rightScoreText.c_str(), 30) / 2,
             10, 30, WHITE);

    if (winnerText) {
      DrawText(winnerText,
               GetScreenWidth() / 2 - MeasureText(winnerText, 30) / 2,
               GetScreenHeight() / 2, 30, WHITE);
    }

    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
