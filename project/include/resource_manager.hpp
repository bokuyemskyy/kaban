#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <string>
#include <unordered_map>

using Resource = std::pair<const unsigned char*, const unsigned int>;

/**
 * @brief A class that provides a brigde between the program and its resources
 *
 * it manages file (image, sound) dependencies from resources.hpp
 * resources.hpp is a packed bunch of files from resources/ folder
 * to build resources.hpp there is a shell script pack_resources.sh
 *
 */
class ResourceManager {
   private:
    static std::unordered_map<std::string, Resource> resources;

   public:
    /**
     * @brief retrieves the data of the given file
     *
     * @param filename the filename
     * @return pair of pointer to the first element to array and array's length
     */
    static Resource getResource(std::string filename);
    /**
     * @brief connects resource filename strings to the correspondent data
     */
    static void loadResources();
};

#endif