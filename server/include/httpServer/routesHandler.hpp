#include "httpServer/httpServer.hpp"
#include <functional>
#include <string>
#include <unordered_map>

namespace http {

class RoutesHandler {
public:
  RoutesHandler();
  ~RoutesHandler();

private:
  std::unordered_map<std::string, std::function<void(const std::string&)>> routesMap;
  void executeRoute(struct httpRequest &request);

  void handleAuth(const std::string &request);
  void handleAdd(const std::string &request);
  void handleSubs(const std::string &request);
  void handleMult(const std::string &request);
  void handleDiv(const std::string &request);
  void handlePow(const std::string &request);
  void handleListInvert(const std::string &request);
  void handleListRepeated(const std::string &request);
};

}
