#pragma once

#include "httpClient/httpClient.hpp"
#include "httpClient/inputManager.hpp"
#include <functional>
#include <string>
#include <unordered_map>

namespace http {
class Application {
public:
  Application();
  ~Application();

  void start();

private:
  TcpClient m_tcpClient;
  InputManager m_inputManager;
  bool m_isAuthenticated;
  std::unordered_map<int, std::function<void()>> m_menuOptions;

  void authenticateClient();
  void showMenu();
  const std::string readFile(const std::string &filename);
  void writeFile(const std::string &filename, const std::string &content);

  void handleAdd();
  void handleSub();
  void handleMult();
  void handleDiv();
  void handlePow();
  void handleFibo();
  void handleInvList();
  void handleRepList();
};
} // namespace http
