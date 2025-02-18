#pragma once

#include <string>
#include <unordered_map>
namespace http {

class AuthManager {
public:
  AuthManager();
  ~AuthManager();

  bool validateUser(std::string &user, std::string &password);

private:
  std::unordered_map<std::string, std::string> users;
  void loadUsers();

};

}
