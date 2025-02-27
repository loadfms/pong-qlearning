#ifndef AI_H
#define AI_H

#include "QLearning.h" // Include the QLearning header
#include "game_objects.h"

void MoveAI(Paddle &paddle, Ball &ball, QLearning &ai, bool debug);

void applyAction(Paddle &paddle, int action);

int getReward(int ballY, float ballRadius, int paddleY, int paddleHeight,
              int paddleSpeed);

void DrawZones(Ball &ball, Paddle &paddle);

#endif
