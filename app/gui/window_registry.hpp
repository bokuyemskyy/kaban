#pragma once

#include <string>
#include <vector>

class WindowRegistry {
   public:
    void                                          add(const char* name) { windows.emplace_back(name); }
    [[nodiscard]] const std::vector<std::string>& get() const { return windows; }
    void                                          clear() { windows.clear(); }

   private:
    std::vector<std::string> windows{};
};