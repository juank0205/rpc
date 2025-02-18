#pragma once

#include <string>
#include <termios.h>

namespace http {

class InputManager {
public:
  InputManager();
  ~InputManager();

  const std::string handleDoubleInput();
  const std::string handleCredentials();
  const std::string handleSingleInput();

private: 
  struct termios m_oldt, m_newt;

  void getTerminalAttr();
  void disableTerminalEcho();
  void restoreTerminal();
};

}
