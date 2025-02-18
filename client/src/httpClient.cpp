#include <csignal>
#include <cstring>
#include <httpClient/httpClient.hpp>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 30720

namespace {
void log(const std::string &message) { std::cout << message << std::endl; }
} // namespace

namespace http {
std::ostream &operator<<(std::ostream &os, const httpRequest &req) {
  os << "Request Type: " << req.requestType << "\n"
     << "Content-Type: " << req.contentType << "\n"
     << "Content-Length: " << req.contentLenght << "\n"
     << "Body: " << req.body << "\n";
  return os;
}

void printHttpRequest(struct httpRequest &request) {
  std::cout << request << std::endl;
}

TcpClient::TcpClient() : m_socketFd(), m_incommingMessage(), m_hints() {
  m_hints.ai_family = AF_INET;
  m_hints.ai_socktype = SOCK_STREAM;
  m_hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo("localhost", "5002", &m_hints, &m_res)) {
    exitWithFailure("getaddrinfo failed");
  }

  m_socketFd = socket(m_res->ai_family, m_res->ai_socktype, m_res->ai_protocol);
  if (m_socketFd < 0) {
    exitWithFailure("failed to create socket");
  }

  int opt = 1;
  if (setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    exitWithFailure("failed to create socket");
  }

  if (connect(m_socketFd, m_res->ai_addr, m_res->ai_addrlen) == -1) {
    exitWithFailure("failed to connect");
  }
}

TcpClient::~TcpClient() { closeClient(); }

void TcpClient::exitWithFailure(const std::string &message) {
  std::cerr << message << std::endl;
  closeClient();
  exit(EXIT_FAILURE);
}

void TcpClient::closeClient() {
  if (m_socketFd > 0) {
    close(m_socketFd);
    m_socketFd = -1;
  }

  if (m_serverSocketFd > 0) {
    close(m_serverSocketFd);
    m_serverSocketFd = -1;
  }

  if (m_res) {
    freeaddrinfo(m_res);
    m_res = nullptr;
  }
  std::cout << "Client closed successfully." << std::endl;
}

struct httpRequest TcpClient::parseHttpResquest(const std::string &request) {
  struct httpRequest parsedResponse {};
  std::istringstream responseStream(request);

  std::string line;
  std::getline(responseStream, line);
  std::istringstream statusLine(line);
  statusLine >> parsedResponse.requestType;
  statusLine >> parsedResponse.route;

  while (std::getline(responseStream, line) && line != "\r") {
    auto colonPos = line.find(':');
    if (colonPos != std::string::npos) {
      std::string headerName = line.substr(0, colonPos);
      std::string headerValue = line.substr(colonPos + 2); // Skip ": "

      if (headerName == "Content-Type") {
        parsedResponse.contentType = headerValue;
      } else if (headerName == "Content-Length") {
        parsedResponse.contentLenght = std::stoi(headerValue);
      }
    }
  }

  std::getline(responseStream, parsedResponse.body, '\0');
  return parsedResponse;
}

std::string TcpClient::sendRequest(const std::string &route,
                                   const std::string &body) {

  long bytesSent;
  std::string message;
  std::ostringstream ss;

  ss << "HTTP/1.1 " << route
     << "\r\nContent-Type: text/plain\r\nContent-Length: ";

  ss << std::to_string(body.size()) << "\r\n\r\n" << body;

  bytesSent = write(m_socketFd, ss.str().c_str(), ss.str().size());

  /*if (bytesSent == ss.str().size()) {*/
  /*  log("Message sent successfully");*/
  /*} else {*/
  /*  log("Message not sent");*/
  /*}*/

  int bytesReceived;
  char buffer[BUFFER_SIZE] = {0};

  bytesReceived = recv(m_socketFd, buffer, sizeof(buffer) - 1, 0);
  if (bytesReceived < 0) {
    exitWithFailure("Receive failed");
  }

  buffer[bytesReceived] = '\0';

  auto response = parseHttpResquest(buffer);
  /*printHttpRequest(response);*/
  return response.body;
}

} // namespace http
