#include "include/game_objects.h"
#include "raylib.h"
#include <asio.hpp>
#include <atomic>
#include <iostream>
#include <string>
#include <thread>

using asio::ip::udp;

const int PORT = 54000;
std::atomic<bool> running(true);

void sendBallPosition(udp::socket &socket, std::vector<udp::endpoint> &clients,
                      Ball &ball) {
  std::string ballData = std::to_string(ball.x) + "," + std::to_string(ball.y);

  for (auto &client : clients) {
    asio::error_code error;
    socket.send_to(asio::buffer(ballData), client, 0, error);
    if (error) {
      std::cerr << "Send error: " << error.message() << std::endl;
    }
  }
}

void receiveCommands(udp::socket &socket, std::vector<udp::endpoint> &clients,
                     Paddle *leftPaddle, Paddle *rightPaddle) {
  char data[1024];
  socket.non_blocking(true);

  while (running) {
    udp::endpoint remote_endpoint;
    asio::error_code error;
    size_t len = socket.receive_from(asio::buffer(data, 1024), remote_endpoint,
                                     0, error);

    if (error && error != asio::error::would_block) {
      std::cerr << "Receive error: " << error.message() << std::endl;
      continue;
    }

    if (!error) {
      std::string command(data, len);

      auto it = std::find(clients.begin(), clients.end(), remote_endpoint);
      if (it == clients.end()) {
        clients.push_back(remote_endpoint);
      }

      if (command == "LEFT_UP")
        leftPaddle->MoveUp();
      if (command == "LEFT_DOWN")
        leftPaddle->MoveDown();
      if (command == "RIGHT_UP")
        rightPaddle->MoveUp();
      if (command == "RIGHT_DOWN")
        rightPaddle->MoveDown();
    }

    std::this_thread::sleep_for(
        std::chrono::milliseconds(10)); // Prevent CPU overuse
  }
}

int main() {
  InitWindow(800, 450, "Multiplayer Pong Server");
  SetWindowState(FLAG_VSYNC_HINT);
  SetWindowPosition(100, 100);

  std::string state = "RUNNING";
  std::string winnerText = "";
  int leftScore = 0;
  int rightScore = 0;

  asio::io_context io_context;
  udp::socket socket(io_context, udp::endpoint(udp::v4(), PORT));
  udp::endpoint remote_endpoint;

  const float initialBallSpeed = 200;
  const float initialPaddleSpeed = 500;
  Ball ball(initialBallSpeed);
  Paddle leftPaddle(initialPaddleSpeed, false);
  Paddle rightPaddle(initialPaddleSpeed, true);

  std::vector<udp::endpoint> clients;
  std::thread networkThread(receiveCommands, std::ref(socket),
                            std::ref(clients), &leftPaddle, &rightPaddle);

  while (!WindowShouldClose()) {
    ball.Move();
    ball.HandleCollision(&rightPaddle);
    ball.HandleCollision(&leftPaddle);

    ball.CheckWinCondition(state, winnerText, leftScore, rightScore);

    sendBallPosition(socket, clients, ball);

    if (winnerText != "" && IsKeyPressed(KEY_SPACE)) {
      ball.x = GetScreenWidth() / 2.0f;
      ball.y = GetScreenHeight() / 2.0f;
      ball.speedX = initialBallSpeed;
      ball.speedY = initialBallSpeed;

      leftPaddle.ResetInitialValues(initialPaddleSpeed, false);
      rightPaddle.ResetInitialValues(initialPaddleSpeed, true);

      ball.ResetInitialValues(initialBallSpeed);

      winnerText = "";
      state = "RUNNING";
    }

    BeginDrawing();
    ClearBackground(BLACK);
    ball.Draw();
    leftPaddle.Draw();
    rightPaddle.Draw();

    if (winnerText != "") {
      DrawText(winnerText.c_str(),
               GetScreenWidth() / 2 - MeasureText(winnerText.c_str(), 30) / 2,
               GetScreenHeight() / 2, 30, WHITE);
    }

    DrawFPS(10, 10);
    EndDrawing();
  }

  running = false;
  socket.close();
  networkThread.join();

  CloseWindow();
  return 0;
}
