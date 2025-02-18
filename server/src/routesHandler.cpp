#include "httpServer/routesHandler.hpp"
#include "httpServer/httpServer.hpp"

#include <cmath>
#include <sstream>
#include <vector>

namespace http {

RoutesHandler::RoutesHandler() {
  m_routesMap["/auth"] = [this](const std::string &req) { handleAuth(req); };
  m_routesMap["/add"] = [this](const std::string &req) { handleAdd(req); };
  m_routesMap["/subs"] = [this](const std::string &req) { handleSubs(req); };
  m_routesMap["/mult"] = [this](const std::string &req) { handleMult(req); };
  m_routesMap["/div"] = [this](const std::string &req) { handleDiv(req); };
  m_routesMap["/pow"] = [this](const std::string &req) { handlePow(req); };
  m_routesMap["/fibo"] = [this](const std::string &req) { handleFibo(req); };
  m_routesMap["/list_inverted"] = [this](const std::string &req) {
    handleListInvert(req);
  };
  m_routesMap["/list_repeated"] = [this](const std::string &req) {
    handleListRepeated(req);
  };
}

RoutesHandler::~RoutesHandler() {}

void RoutesHandler::executeRoute(const struct httpRequest &request) {
  if (m_routesMap.find(request.route) != m_routesMap.end()) 
    m_routesMap[request.route](request.body);
  else
    m_response = "Invalid route";
}

void RoutesHandler::handleAuth(const std::string &request) {
  std::istringstream ss(request);
  std::string user, password;
  ss >> user >> password;

  if (ss) {
    bool validUser = m_authManager.validateUser(user, password);

    if (validUser)
      m_response = "1";
    else
     m_response = "0";
  }
}

void RoutesHandler::handleAdd(const std::string &request) {
  std::istringstream ss(request);
  std::ostringstream answerStream;
  int num1, num2;
  ss >> num1 >> num2;

  if (ss) {
    answerStream << num1 << " + " << num2 << " = " << (num1 + num2) << "\n";
  } else {
    answerStream << "Error: Invalid input for addition\n";
  }

  m_response = answerStream.str();
}

void RoutesHandler::handleSubs(const std::string &request) {
  std::istringstream ss(request);
  std::ostringstream answerStream;
  int num1, num2;
  ss >> num1 >> num2;

  if (ss) {
    answerStream << num1 << " - " << num2 << " = " << (num1 - num2) << "\n";
  } else {
    answerStream << "Error: Invalid input for subtraction\n";
  }

  m_response = answerStream.str();
}

void RoutesHandler::handleMult(const std::string &request) {
  std::istringstream ss(request);
  std::ostringstream answerStream;
  int num1, num2;
  ss >> num1 >> num2;

  if (ss) {
    answerStream << num1 << " * " << num2 << " = " << (num1 * num2) << "\n";
  } else {
    answerStream << "Error: Invalid input for multiplication\n";
  }

  m_response = answerStream.str();
}

void RoutesHandler::handleDiv(const std::string &request) {
  std::istringstream ss(request);
  std::ostringstream answerStream;
  int num1, num2;
  ss >> num1 >> num2;

  if (ss) {
    if (num2 != 0) {
      answerStream << num1 << " / " << num2 << " = " << (num1 / num2) << "\n";
    } else {
      answerStream << "Error: Division by zero\n";
    }
  } else {
    answerStream << "Error: Invalid input for division\n";
  }

  m_response = answerStream.str();
}

void RoutesHandler::handlePow(const std::string &request) {
  std::istringstream ss(request);
  std::ostringstream answerStream;
  int base, exponent;
  ss >> base >> exponent;

  if (ss) {
    answerStream << base << " ^ " << exponent << " = "
                 << std::pow(base, exponent) << "\n";
  } else {
    answerStream << "Error: Invalid input for power calculation\n";
  }

  m_response = answerStream.str();
}

void RoutesHandler::handleFibo(const std::string &request) {
  std::istringstream ss(request);
  std::ostringstream answerStream;
  int terms;
  ss >> terms;

  if (ss) {
    answerStream << m_repository.fibonacci(terms) << "\n";
  } else {
    answerStream << "Error: Invalid input for Fibonacci calculation\n";
  }

  m_response = answerStream.str();
}

void RoutesHandler::handleListInvert(const std::string &request) {
  std::istringstream ss(request);
  std::ostringstream answerStream;
  std::vector<int> numbers;
  int temp;
  
  while (ss >> temp) {
    numbers.push_back(temp);
  }

  if (numbers.empty()) {
    answerStream << "Error: Invalid input for list inversion\n";
  } else {
    m_repository.invertVector(numbers);
    for (int num : numbers) {
      answerStream << num << " ";
    }
    answerStream << "\n";
  }

  m_response = answerStream.str();
}

void RoutesHandler::handleListRepeated(const std::string &request) {
  std::istringstream ss(request);
  std::ostringstream answerStream;
  std::vector<int> numbers;
  int temp;
  
  while (ss >> temp) {
    numbers.push_back(temp);
  }

  if (numbers.empty()) {
    answerStream << "Error: Invalid input for list repetition check\n";
  } else {
    answerStream << m_repository.repeatedInVector(numbers) << "\n";
  }

  m_response = answerStream.str();
}


} // namespace http
