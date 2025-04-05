#ifndef ERROR_BUS_HPP
#define ERROR_BUS_HPP

#include <string>

void errorCallback(int error, const char *description);

namespace ErrorBus {

using ErrorHandler = void (*)(int, const std::string &);

void setErrorHandler(ErrorHandler handler);
void handleError(int error, const char *description);

}  // namespace ErrorBus

#endif