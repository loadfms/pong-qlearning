#include "include/AI.h"
#include "include/QLearning.h"
#include "include/game_objects.h"
#include <cmath>
#include <raylib.h>

void MoveAI(Paddle &paddle, Ball &ball, QLearning &qlearning, bool debug) {

  int state =
      static_cast<int>(ball.y / 50) * 3 + static_cast<int>(paddle.y / 50);

  // Choose an action (0: stay, 1: move up, 2: move down)
  int action = qlearning.ChooseAction(state);

  int newPaddleY = applyAction(paddle.y, action, paddle.speed,
                               GetScreenHeight(), paddle.height);
  paddle.y = newPaddleY;

  int nextState =
      static_cast<int>(ball.y / 50) * 3 + static_cast<int>(newPaddleY / 50);

  int reward = getReward(ball.y, ball.radius, paddle.y, paddle.height);

  qlearning.UpdateQValue(state, action, reward, nextState);

  if (debug) {
    DrawZones(ball, paddle);
  }
}

float applyAction(float paddleY, int action, float speed, float screenHeight,
                  float paddleHeight) {
  if (action == 1 && paddleY - paddleHeight / 2 > 0) {
    paddleY -= speed * GetFrameTime();
  } else if (action == 2 && paddleY + paddleHeight < screenHeight) {
    paddleY += speed * GetFrameTime();
  }
  return paddleY;
}

int getReward(int ballY, float ballRadius, int paddleY, int paddleHeight) {
  int reward = 0;
  // Calculate the vertical range of the ball (since it's a circle)*/
  float ballTop = ballY - ballRadius;
  float ballBottom = ballY + ballRadius;
  float paddleTop = paddleY - paddleHeight / 2;
  float paddleBottom = paddleY + paddleHeight / 2;

  // Reward if the ball is within the paddle's height*/
  if (ballBottom >= paddleTop && ballTop <= paddleBottom) {
    reward = 20; // Ball is within paddle's height*/
  } else {
    reward = -std::abs(ballY - paddleY); // Penalize if ball is not near paddle
  }

  // Reward for aligning paddle's center with ball's center*/
  /*if (std::abs(ballY - paddleY) <= paddleHeight / 2) {*/
  /*  reward += 5; // Reward for aligning centers*/
  /*}*/

  return reward;
}

void DrawZones(Ball &ball, Paddle &paddle) {
  // Define zone size (50px) and number of zones
  int ballzoneSize = 50;
  int paddleZoneSize = 50;

  // Calculate ball and paddle zones (similar to your reward function)
  int ballZone = static_cast<int>(ball.y / ballzoneSize);

  // Draw the ball zone (in red for visibility)
  Color ballZoneColor = YELLOW;
  Rectangle ballZoneRect = {0, static_cast<float>(ballZone * ballzoneSize),
                            static_cast<float>(GetScreenWidth()),
                            static_cast<float>(ballzoneSize)};
  DrawRectangleRec(ballZoneRect, ballZoneColor);

  // Draw the paddle zone (in blue for visibility)
  Color paddleZoneColor = BLUE;

  // Adjust the rectangle so it aligns with the paddle's center
  Rectangle paddleZoneRect = {
      0,
      static_cast<float>((paddle.y - paddle.height / 2) / paddleZoneSize) *
          paddleZoneSize,
      static_cast<float>(GetScreenWidth()), static_cast<float>(paddle.height)};

  DrawRectangleRec(paddleZoneRect, paddleZoneColor);
}
