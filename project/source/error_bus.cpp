#include "error_bus.hpp"

#include <iostream>
#include <stdexcept>

void errorHandler(int error, const std::string &description) {
    std::cerr << "Error " << error << ": " << description << "\n";
    throw std::runtime_error(description);
}

namespace ErrorBus {

ErrorHandler currentHandler = errorHandler;

void setErrorHandler(ErrorHandler handler) { currentHandler = handler; }

void handleError(int error, const char *description) {
    if (currentHandler != nullptr) {
        currentHandler(error, description);
    } else {
        throw std::runtime_error("No error handler set");
    }
}

}  // namespace ErrorBus

void errorCallback(int error, const char *description) {
    ErrorBus::handleError(error, description);
}