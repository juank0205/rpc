#include "httpServer/routesHandler.hpp"
#include "httpServer/httpServer.hpp"

namespace http {

RoutesHandler::RoutesHandler() {
  routesMap["/auth"] = [this](const std::string &req) { handleAuth(req); };
  routesMap["/add"] = [this](const std::string &req) { handleAdd(req); };
  routesMap["/subs"] = [this](const std::string &req) { handleSubs(req); };
  routesMap["/mult"] = [this](const std::string &req) { handleMult(req); };
  routesMap["/div"] = [this](const std::string &req) { handleDiv(req); };
  routesMap["/pow"] = [this](const std::string &req) { handlePow(req); };
  routesMap["/list_invert"] = [this](const std::string &req) {
    handleListInvert(req);
  };
  routesMap["/list_repeated"] = [this](const std::string &req) {
    handleListRepeated(req);
  };
}

RoutesHandler::~RoutesHandler() {}

void RoutesHandler::executeRoute(struct httpRequest &request) {}

} // namespace http
