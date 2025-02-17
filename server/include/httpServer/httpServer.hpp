#pragma once

#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace http {
struct httpRequest {
  std::string requestType;
  std::string route;
  std::string contentType;
  int contentLenght;
  std::string body;
};

struct httpResponse {
  std::shared_ptr<std::string> content;
};

class TcpServer {
public:
  TcpServer();
  ~TcpServer();

  void startListen();
private:
  struct addrinfo hints, *res;
  int socketFd;
  long incommingMessage;
  int newSocketFd;
  static TcpServer *instance;

  void closeServer();
  void printListeningAddress();
  void exitWithFailure(const std::string &message);
  struct httpRequest parseHttpResquest(std::string request);
  void sendResponse(struct httpResponse);

  static void handleSignal(int sigint);
  void handleSignalImpl(int sigint);
};

} // namespace http
