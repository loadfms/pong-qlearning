#include "include/AI.h"
#include "include/QLearning.h"
#include "include/game_objects.h"
#include "raylib.h"
#include <string>

QLearning qlearningRight(100, 3, 0.1, 0.9, 0.2);
QLearning qlearningLeft(100, 3, 0.1, 0.9, 0.2);
bool trainingAi = true;

int main(void) {
  InitWindow(800, 450, "raylib [core] example - basic window");
  SetWindowPosition(100, 100); // Set a specific position for the window
  SetWindowState(FLAG_VSYNC_HINT);

  const int speedFactor = 1;
  std::string winnerText = "";
  const float initialBallSpeed = 200 * speedFactor;
  const float initialPaddleSpeed = 500 * speedFactor;
  int leftScore = 0;
  int rightScore = 0;
  bool paused = false;

  Ball ball(initialBallSpeed);
  Paddle *rightPaddle = new Paddle(initialPaddleSpeed, true);
  Paddle *leftPaddle = new Paddle(initialPaddleSpeed, false);

  while (!WindowShouldClose()) {
    if (trainingAi) {
      MoveAI(*rightPaddle, ball, qlearningRight, true);
      MoveAI(*leftPaddle, ball, qlearningLeft, false);

      qlearningRight.UpdateEpsilon();
      qlearningLeft.UpdateEpsilon();
    }

    ball.Move();

    if (IsKeyDown(KEY_UP)) {
      leftPaddle->MoveUp();
    }

    if (IsKeyDown(KEY_DOWN)) {
      leftPaddle->MoveDown();
    }

    ball.HandleCollision(rightPaddle);
    ball.HandleCollision(leftPaddle);

    ball.CheckWinCondition(paused, winnerText, leftScore, rightScore);

    if (winnerText != "" &&
        ((trainingAi && paused) || IsKeyPressed(KEY_SPACE))) {
      ball.x = GetScreenWidth() / 2.0f;
      ball.y = GetScreenHeight() / 2.0f;
      ball.speedX = initialBallSpeed;
      ball.speedY = initialBallSpeed;

      leftPaddle->ResetInitialValues(initialPaddleSpeed, false);
      rightPaddle->ResetInitialValues(initialPaddleSpeed, true);

      ball.ResetInitialValues(initialBallSpeed);

      winnerText = "";
      paused = false;
    }

    BeginDrawing();

    ClearBackground(BLACK);

    ball.Draw();
    leftPaddle->Draw();
    rightPaddle->Draw();

    std::string leftScoreText = std::to_string(leftScore);
    std::string rightScoreText = std::to_string(rightScore);

    DrawText(leftScoreText.c_str(),
             GetScreenWidth() / 4 - MeasureText(leftScoreText.c_str(), 30) / 2,
             10, 30, WHITE);

    DrawText(rightScoreText.c_str(),
             GetScreenWidth() * 3 / 4 -
                 MeasureText(rightScoreText.c_str(), 30) / 2,
             10, 30, WHITE);

    if (winnerText != "" && trainingAi != true) {
      DrawText(winnerText.c_str(),
               GetScreenWidth() / 2 - MeasureText(winnerText.c_str(), 30) / 2,
               GetScreenHeight() / 2, 30, WHITE);
    }

    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();

  delete rightPaddle;
  delete leftPaddle;

  return 0;
}
