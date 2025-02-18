#include "httpClient/application.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace http {

Application::Application() : m_isAuthenticated(false) {
  m_menuOptions[1] = [this]() { handleAdd(); };
  m_menuOptions[2] = [this]() { handleSub(); };
  m_menuOptions[3] = [this]() { handleMult(); };
  m_menuOptions[4] = [this]() { handleDiv(); };
  m_menuOptions[5] = [this]() { handlePow(); };
  m_menuOptions[6] = [this]() { handleFibo(); };
  m_menuOptions[7] = [this]() { handleInvList(); };
  m_menuOptions[8] = [this]() { handleRepList(); };
}

Application::~Application() {}

void Application::start() {
  authenticateClient();
  showMenu();
}

void Application::authenticateClient() {
  std::string response;
  std::string credentials;

  while (!m_isAuthenticated) {
    credentials = m_inputManager.handleCredentials();
    response = m_tcpClient.sendRequest("/auth", credentials);

    if (response == "1") {
      std::cout << "\nAuthenticated successfully" << std::endl;
      m_isAuthenticated = true;
    } else {
      std::cout << "\nInvalid credentials\n" << std::endl;
    }
  }
}

void Application::showMenu() {
  int op = 1;

  while (op != 0) {
    std::cout << "************ Math Menu ************\n"
              << "1. Add\n"
              << "2. Subtract\n"
              << "3. Multiply\n"
              << "4. Divide\n"
              << "5. Power\n"
              << "6. Fibonacci\n"
              << "7. Invert List\n"
              << "8. Find Repeated Elements\n"
              << "0. Exit\n"
              << "***********************************\n"
              << "Enter your choice: ";

    std::cin >> op;

    if (m_menuOptions.find(op) != m_menuOptions.end()) {
      m_menuOptions[op]();
    } else if (op != 0) {
      std::cout << "Invalid option...\n";
    }
  }

  std::cout << "Closing...\n";
}

const std::string Application::readFile(const std::string &filename) {
  std::ifstream file("./res/" + filename);

  if (!file) {
    return "";
  }

  std::string content = "";
  std::getline(file, content);

  file.close();
  return content;
}

void Application::writeFile(const std::string &filename,
                            const std::string &content) {
  std::ofstream file("./res/" + filename);
  if (!file) {
    return;
  }
  file << content;
  file.close();
  std::cout << "Data written to ./res/" << filename << std::endl;
  return;
}

void Application::handleAdd() {
  std::string request = m_inputManager.handleDoubleInput();
  std::cout << m_tcpClient.sendRequest("/add", request) << std::endl;
}

void Application::handleSub() {
  std::string request = m_inputManager.handleDoubleInput();
  std::cout << m_tcpClient.sendRequest("/subs", request) << std::endl;
}

void Application::handleMult() {
  std::string request = m_inputManager.handleDoubleInput();
  std::cout << m_tcpClient.sendRequest("/mult", request) << std::endl;
}

void Application::handleDiv() {
  std::string request = m_inputManager.handleDoubleInput();
  std::cout << m_tcpClient.sendRequest("/div", request) << std::endl;
}

void Application::handlePow() {
  std::string request = m_inputManager.handleDoubleInput();
  std::cout << m_tcpClient.sendRequest("/pow", request) << std::endl;
}

void Application::handleFibo() {
  std::string request = m_inputManager.handleSingleInput();
  std::cout << m_tcpClient.sendRequest("/fibo", request) << std::endl;
}

void Application::handleInvList() {
  std::string list = readFile("invList.txt");
  std::cout << m_tcpClient.sendRequest("/list_inverted", list) << std::endl;
}

void Application::handleRepList() {
  std::string list = readFile("repList.txt");
  std::cout << m_tcpClient.sendRequest("/list_repeated", list) << std::endl;
}

} // namespace http
