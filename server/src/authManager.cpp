#include "httpServer/authManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <limits.h>

namespace http {

AuthManager::AuthManager() { loadUsers(); }
AuthManager::~AuthManager() {}

void AuthManager::loadUsers() {
  char buffer[PATH_MAX];
  std::ifstream usersFile("./res/users.txt");
  if (!usersFile) {
    std::cerr << "Failed to open file" << std::endl;
  }

  std::string line, username, password;

  while (std::getline(usersFile, line)) {
    std::istringstream ss(line);
    if (ss >> username >> password) {
      users[username] = password;
    }
  }

  usersFile.close();
}

bool AuthManager::validateUser(std::string &user, std::string &password) {
  if (users.find(user) != users.end()) {
    if (password == users[user])
      return true;
  }
  return false;
}

} // namespace http
