#ifndef AI_H
#define AI_H

#include "QLearning.h" // Include the QLearning header
#include "game_objects.h"

void MoveAI(Paddle &paddle, Ball &ball, QLearning &ai, bool debug);

float applyAction(float paddleY, int action, float speed, float screenHeight,
                  float paddleHeight);

/*int getReward(int ballZone, int paddleZone);*/

int getReward(int ballY, float ballRadius, int paddleY, int paddleHeight);

void DrawZones(Ball &ball, Paddle &paddle);

#endif
