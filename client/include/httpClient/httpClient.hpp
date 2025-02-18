#pragma once

#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

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

class TcpClient {
public:
  TcpClient();
  ~TcpClient();

  std::string sendRequest(const std::string &route, const std::string &body);

private:
  struct addrinfo m_hints, *m_res;
  int m_socketFd;
  int m_serverSocketFd;
  long m_incommingMessage;

  struct httpRequest parseHttpResquest(const std::string &request);
  void exitWithFailure(const std::string &message);
  void closeClient();
};

} // namespace http
