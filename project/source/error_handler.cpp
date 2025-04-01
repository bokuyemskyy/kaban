#include <error_handler.hpp>
#include <iostream>

void errorCallback(int error, const char *description) {
  handleError(error, description);
}

void handleError(int error, const char *description) {
  std::cout << "Error " << error << ": " << description << std::endl;
}