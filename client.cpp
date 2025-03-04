#include <asio.hpp>
#include <iostream>
#include <poll.h>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>

using asio::ip::udp;

const char *SERVER_IP = "127.0.0.1"; // Change to server IP if needed
const int SERVER_PORT = 54000;

char getKeyPress() {
  struct termios oldt, newt;
  char ch = 0;
  struct pollfd fds = {STDIN_FILENO, POLLIN, 0};

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  if (poll(&fds, 1, 10) > 0) { // Check if there's input
    read(STDIN_FILENO, &ch, 1);
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}

int main() {
  asio::io_context io_context;
  udp::socket socket(io_context);
  socket.open(udp::v4());
  socket.non_blocking(true); // Make socket non-blocking

  bool hello = false;
  udp::endpoint server_endpoint(asio::ip::make_address(SERVER_IP), SERVER_PORT);

  std::cout << "Client started. Press W/S (Left) or UP/DOWN (Right) to move.\n";

  if (!hello) {
    socket.send_to(asio::buffer("HELLO_RIGHT_BRUXARIA"), server_endpoint);
    hello = true;
  }

  while (true) {
    char key = getKeyPress();
    std::string command = "";

    if (key == 'w')
      command = "LEFT_UP";
    if (key == 's')
      command = "LEFT_DOWN";
    if (key == '\033') { // Escape sequence for arrow keys
      getchar();         // Skip '['
      switch (getchar()) {
      case 'A':
        command = "RIGHT_UP";
        break;
      case 'B':
        command = "RIGHT_DOWN";
        break;
      }
    }

    if (!command.empty()) {
      socket.send_to(asio::buffer(command), server_endpoint);
    }

    // **Receive Ball Position**
    char data[1024] = {0};
    udp::endpoint sender_endpoint;
    asio::error_code error;
    size_t len = socket.receive_from(asio::buffer(data, 1024), sender_endpoint,
                                     0, error);

    if (!error && len > 0) {
      std::string receivedData(data, len);
      /*std::cout << "Received from server: " << receivedData << std::endl;*/
    }

    std::this_thread::sleep_for(
        std::chrono::milliseconds(10)); // Prevent CPU overuse
  }

  return 0;
}
