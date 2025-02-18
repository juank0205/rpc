#include "httpClient/inputManager.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

namespace http {

InputManager::InputManager() { getTerminalAttr(); }
InputManager::~InputManager() {}

const std::string InputManager::handleDoubleInput() {
  int a, b;
  std::cout << "Enter the first number" << std::endl;
  std::cin >> a;
  std::cout << "Enter the second number" << std::endl;
  std::cin >> b;

  std::ostringstream ss;
  ss << a << " " << b;

  return ss.str();
}

const std::string InputManager::handleCredentials() {
  std::string a, b;
  std::cout << "Username: ";
  std::cin >> a;
  std::cout << "\nPassword: ";
  disableTerminalEcho();
  std::cin >> b;
  restoreTerminal();

  std::ostringstream ss;
  ss << a << " " << b;

  return ss.str();
}

const std::string InputManager::handleSingleInput() {
  int a, b;
  std::cout << "Enter the first number" << std::endl;
  std::cin >> a;

  std::ostringstream ss;
  ss << a;

  return ss.str();
}

void InputManager::getTerminalAttr() {
  tcgetattr(STDIN_FILENO, &m_oldt);
  m_newt = m_oldt;
}

void InputManager::disableTerminalEcho() {
  m_newt.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &m_newt);
}

void InputManager::restoreTerminal() {
  tcsetattr(STDIN_FILENO, TCSANOW, &m_oldt);
}

} // namespace http
