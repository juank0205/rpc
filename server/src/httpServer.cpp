#include <csignal>
#include <cstddef>
#include <cstring>
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

TcpServer::TcpServer() : m_socketFd(), m_incommingMessage(), m_hints() {
  TcpServer::instance = this;
  signal(SIGINT, handleSignal);

  m_hints.ai_family = AF_INET;
  m_hints.ai_socktype = SOCK_STREAM;
  m_hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo(NULL, "5002", &m_hints, &m_res)) {
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

  if (bind(m_socketFd, m_res->ai_addr, m_res->ai_addrlen)) {
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
  if (m_socketFd > 0) {
    close(m_socketFd);
    m_socketFd = -1;
  }

  if (m_newSocketFd > 0) {
    close(m_newSocketFd);
    m_newSocketFd = -1;
  }

  if (m_res) {
    freeaddrinfo(m_res);
    m_res = nullptr;
    std::cout << "Server closed successfully." << std::endl;
  }
}

void TcpServer::printListeningAddress() {
  char host[NI_MAXHOST], service[NI_MAXSERV];
  if (getnameinfo(m_res->ai_addr, m_res->ai_addrlen, host, sizeof(host),
                  service, sizeof(service), NI_NUMERICHOST | NI_NUMERICSERV))
    return;

  std::ostringstream ss;
  ss << "\n*** Listening on ADDRESS: " << host << " PORT: " << service
     << " ***\n\n";
  log(ss.str());
}

void TcpServer::startListen() {
  if (listen(m_socketFd, 20) < 0) {
    exitWithFailure("Socket listen failed");
  }

  printListeningAddress();

  struct sockaddr_storage incAddr;
  socklen_t incAddrsize = sizeof incAddr;
  int bytesReceived;
  char buffer[BUFFER_SIZE];

  while (true) {
    log("Waiting for new connection");
    m_newSocketFd =
        accept(m_socketFd, (struct sockaddr *)&incAddr, &incAddrsize);

    memset(buffer, 0, sizeof(buffer));
    bytesReceived = recv(m_newSocketFd, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0) {
      exitWithFailure("Failed to receive data");
    }

    auto request = parseHttpResquest(std::string(buffer));
    printHttpRequest(request);
    m_routesHanlder.executeRoute(request);

    const std::string responseBody = m_routesHanlder.getResponse();
    sendResponse(responseBody);

    close(m_newSocketFd);
  }
}

struct httpRequest TcpServer::parseHttpResquest(std::string request) {
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

void TcpServer::sendResponse(std::string body) {
  long bytesSent;
  std::string message;
  std::ostringstream ss;

  ss << "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";

  ss << std::to_string(body.size()) << "\r\n\r\n" << body;

  bytesSent = write(m_newSocketFd, ss.str().c_str(), ss.str().size());

  if (bytesSent == ss.str().size()) {
    log("Message sent successfully");
  } else {
    log("Message not sent");
  }
}
} // namespace http
