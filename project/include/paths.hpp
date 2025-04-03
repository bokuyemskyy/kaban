#include <filesystem>

/**
 *
 * @brief Retrieves the executable directory path
 * the path is used to locate application resources in resource_manager.hpp
 * @return A string containing the directory path of th executable
 *
 */
class Paths {
   public:
    static std::string getExecutablePath() {
        return std::filesystem::canonical("/proc/self/exe").parent_path().string();
    }
};