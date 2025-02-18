#include "httpServer/repository.hpp"
#include <unordered_map>
#include <vector>

namespace http {
Repository::Repository() {}
Repository::~Repository() {}

int Repository::fibonacci(int term) {
  int a = 0, b = 1;
  for (int i = 0; i < term; i++) {
    b = a + b;
    a = b - a;
  }

  return a;
}

void Repository::invertVector(std::vector<int> &numbers) {
  for (int i = 0; i < numbers.size() / 2; i++) {
    numbers[i] += numbers[numbers.size() - i - 1];
    numbers[numbers.size() - i - 1] =
        numbers[i] - numbers[numbers.size() - i - 1];
    numbers[i] -= numbers[numbers.size() - i - 1];
  }
}

int Repository::repeatedInVector(std::vector<int> &numbers) {
  std::unordered_map<int, int> count;
  int maxNum = -1, maxAmount = -1;
  for (int num : numbers) {
    if (count.find(num) != count.end())
      count[num]++;
    else
      count[num] = 1;

    if (count[num] > maxAmount) {
      maxNum = num;
      maxAmount = count[num];
    }
  }

  return maxNum;
}

} // namespace http
