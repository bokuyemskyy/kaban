#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>

#include "resources.hpp"

struct Resource {
    const unsigned char* data;
    unsigned int         length;
};

/**
 * @brief A class that provides a brigde between the program and its resources
 *
 * it manages file (image, sound) dependencies from resources.hpp
 * resources.hpp is a packed bunch of files from resources/ folder
 * to build resources.hpp there is a shell script pack_resources.sh
 */
class ResourceManager {
   private:
    /**
     * @brief A function to get the resources map
     *
     * a function-local instead of static variable
     * @return the map reference
     */
    static std::unordered_map<std::string, Resource>& getMap() {
        static std::unordered_map<std::string, Resource> resources;
        return resources;
    }

   public:
    /**
     * @brief retrieves the data of the given file
     *
     * @param filename the filename
     * @return pair: pointer to the first element to array, array's length
     */
    static Resource getResource(std::string filename) {
        std::ranges::replace(filename, '.', '_');
        std::ranges::replace(filename, '-', '_');

        auto& map = getMap();
        auto  it  = map.find(filename);
        if (it == map.end()) return {.data = nullptr, .length = 0};
        return it->second;
    }
    /**
     * @brief connects resource filename strings to the correspondent data
     */
    static void loadResources() {
        auto& map = getMap();
        map.emplace("p_png", Resource(p_png, p_png_len));
        map.emplace("P_png", Resource(P_png, P_png_len));
        map.emplace("n_png", Resource(n_png, n_png_len));
        map.emplace("N_png", Resource(N_png, N_png_len));
        map.emplace("b_png", Resource(b_png, b_png_len));
        map.emplace("B_png", Resource(B_png, B_png_len));
        map.emplace("r_png", Resource(r_png, r_png_len));
        map.emplace("R_png", Resource(R_png, R_png_len));
        map.emplace("q_png", Resource(q_png, q_png_len));
        map.emplace("Q_png", Resource(Q_png, Q_png_len));
        map.emplace("k_png", Resource(k_png, k_png_len));
        map.emplace("K_png", Resource(K_png, K_png_len));
        map.emplace("mark_png", Resource(mark_png, mark_png_len));
        map.emplace("last_png", Resource(last_png, last_png_len));
        map.emplace("round_png", Resource(round_png, round_png_len));
        map.emplace("capture_mp3", Resource(capture_mp3, capture_mp3_len));
        map.emplace("move_self_mp3", Resource(move_self_mp3, move_self_mp3_len));
    }

    static void init() { loadResources(); }
};