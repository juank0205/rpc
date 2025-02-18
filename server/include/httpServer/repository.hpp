#pragma once
#include <vector>

namespace http {

class Repository {
public:
  Repository();
  ~Repository();

  int fibonacci(int term);
  void invertVector(std::vector<int> &numbers);
  int repeatedInVector(std::vector<int> &numbers);
};

}
