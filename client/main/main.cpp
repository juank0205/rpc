#include "httpClient/application.hpp"
#include <httpClient/httpClient.hpp>

int main(int argc, char *argv[]) {
  http::Application app;
  app.start();
  
  return 0;
}
