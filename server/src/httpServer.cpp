#include <csignal>
#include <cstddef>
#include <httpServer/httpServer.hpp>
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

void printHttpResponse(struct http::httpRequest response) {
  std::cout << "Type: " << response.requestType
            << "\nContentType: " << response.contentType
            << "\nContentLenght: " << response.contentLenght
            << "\nRoute: " << response.route << "\nBody: " << response.body
            << std::endl;
}
} // namespace

namespace http {
std::ostream &operator<<(std::ostream &os, const httpRequest &req) {
  os << "Request Type: " << req.requestType << "\n"
     << "Route: " << req.route << "\n"
     << "Content-Type: " << req.contentType << "\n"
     << "Content-Length: " << req.contentLenght << "\n"
     << "Body: " << req.body << "\n";
  return os;
}

void printHttpRequest(struct httpRequest &request) {
  std::cout << request << std::endl;
}

TcpServer *TcpServer::instance = nullptr;

TcpServer::TcpServer() : socketFd(), incommingMessage(), hints() {
  TcpServer::instance = this;
  signal(SIGINT, handleSignal);

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo(NULL, "5002", &hints, &res)) {
    exitWithFailure("getaddrinfo failed");
  }

  socketFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (socketFd < 0) {
    exitWithFailure("failed to create socket");
  }

  int opt = 1;
  if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    exitWithFailure("failed to create socket");
  }

  if (bind(socketFd, res->ai_addr, res->ai_addrlen)) {
    exitWithFailure("failed to bind");
  }
}

TcpServer::~TcpServer() {}

void TcpServer::exitWithFailure(const std::string &message) {
  std::cerr << message << std::endl;
  closeServer();
  exit(EXIT_FAILURE);
}

void TcpServer::handleSignal(int sigint) {
  std::cout << "Server terminated" << std::endl;
  if (instance != nullptr) {
    instance->handleSignalImpl(sigint);
  }
}

void TcpServer::handleSignalImpl(int sigint) { closeServer(); }

void TcpServer::closeServer() {
  if (socketFd > 0) {
    close(socketFd);
    socketFd = -1;
  }

  if (newSocketFd > 0) {
    close(newSocketFd);
    newSocketFd = -1;
  }

  if (res) {
    freeaddrinfo(res);
    res = nullptr;
    std::cout << "Server closed successfully." << std::endl;
  }
}

void TcpServer::printListeningAddress() {
  char host[NI_MAXHOST], service[NI_MAXSERV];
  if (getnameinfo(res->ai_addr, res->ai_addrlen, host, sizeof(host), service,
                  sizeof(service), NI_NUMERICHOST | NI_NUMERICSERV))
    return;

  std::ostringstream ss;
  ss << "\n*** Listening on ADDRESS: " << host << " PORT: " << service
     << " ***\n\n";
  log(ss.str());
}

void TcpServer::startListen() {
  if (listen(socketFd, 20) < 0) {
    exitWithFailure("Socket listen failed");
  }

  printListeningAddress();

  struct sockaddr_storage incAddr;
  socklen_t incAddrsize = sizeof incAddr;
  int bytesReceived;
  char buffer[BUFFER_SIZE];

  while (true) {
    log("Waiting for new connection");
    newSocketFd = accept(socketFd, (struct sockaddr *)&incAddr, &incAddrsize);

    bytesReceived = recv(newSocketFd, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0) {
      exitWithFailure("Failed to receive data");
    }

    auto request = parseHttpResquest(std::string(buffer));
    printHttpRequest(request);

    close(newSocketFd);
  }
}

struct httpRequest TcpServer::parseHttpResquest(std::string request) {
  struct httpRequest parsedResponse;
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

void TcpServer::sendResponse(struct httpResponse response) {
  long bytesSent;
  std::string message;
  std::ostringstream ss;

  ss << "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";

  ss << std::to_string(response.content->size()) << "\r\n\r\n"
     << response.content->c_str();
  bytesSent = write(newSocketFd, ss.str().c_str(), ss.str().size());

  if (bytesSent == ss.str().size()) {
    log("Message sent");
  } else {
    log("Message not sent");
  }
}
} // namespace http
