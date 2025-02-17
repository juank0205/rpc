#include <httpServer/httpServer.hpp>

int main(int argc, char *argv[]) {
  http::TcpServer server = http::TcpServer();
  server.startListen();
  return 0;
}
