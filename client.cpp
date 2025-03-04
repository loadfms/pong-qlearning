#include "raylib.h"
#include <asio.hpp>
#include <string>

using asio::ip::udp;

const char *SERVER_IP = "127.0.0.1"; // Change to server IP if needed
const int SERVER_PORT = 54000;

int main() {
  InitWindow(400, 300, "Multiplayer Pong Client");
  SetWindowState(FLAG_VSYNC_HINT);

  asio::io_context io_context;
  udp::socket socket(io_context);
  socket.open(udp::v4());

  udp::endpoint server_endpoint(asio::ip::make_address(SERVER_IP), SERVER_PORT);

  while (!WindowShouldClose()) {
    std::string command = "";
    if (IsKeyDown(KEY_W))
      command = "LEFT_UP";
    if (IsKeyDown(KEY_S))
      command = "LEFT_DOWN";
    if (IsKeyDown(KEY_UP))
      command = "RIGHT_UP";
    if (IsKeyDown(KEY_DOWN))
      command = "RIGHT_DOWN";

    if (!command.empty()) {
      socket.send_to(asio::buffer(command), server_endpoint);
    }

    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawText("Client Running...", 100, 140, 20, WHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
