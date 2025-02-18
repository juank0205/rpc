#pragma once

#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "httpServer/routesHandler.hpp"

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
  struct addrinfo m_hints, *m_res;
  int m_socketFd;
  long m_incommingMessage;
  int m_newSocketFd;
  static TcpServer *instance;
  RoutesHandler m_routesHanlder;

  void closeServer();
  void printListeningAddress();
  void exitWithFailure(const std::string &message);
  struct httpRequest parseHttpResquest(std::string request);
  void sendResponse(const std::string body);

  static void handleSignal(int sigint);
  void handleSignalImpl(int sigint);
};

} // namespace http
