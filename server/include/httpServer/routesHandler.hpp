#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "httpServer/authManager.hpp"
#include "httpServer/repository.hpp"

namespace http {

class RoutesHandler {
public:
  RoutesHandler();
  ~RoutesHandler();

  inline const std::string &getResponse() const { return m_response; }

  void executeRoute(const struct httpRequest &request);
private:
  std::unordered_map<std::string, std::function<void(const std::string &)>>
      m_routesMap;
  std::string m_response;
  AuthManager m_authManager;
  Repository m_repository;  

  void handleAuth(const std::string &request);
  void handleAdd(const std::string &request);
  void handleSubs(const std::string &request);
  void handleMult(const std::string &request);
  void handleDiv(const std::string &request);
  void handlePow(const std::string &request);
  void handleFibo(const std::string &request);
  void handleListInvert(const std::string &request);
  void handleListRepeated(const std::string &request);
};

} // namespace http
