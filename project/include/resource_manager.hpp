#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>

#include "resources.hpp"

using Resource = std::pair<unsigned char*, int>;
/**
 * @brief A class that connects resource string filename to the correspondent data
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
    static Resource getResource(std::string filename) {
        std::ranges::replace(filename, '.', '_');
        std::ranges::replace(filename, '-', '_');

        auto it = resources.find(filename);
        if (it == resources.end()) return {nullptr, 0};
        return it->second;
    }
    static void loadResources() {
        resources["p_png"] = {p_png, p_png_len};
        resources["P_png"] = {P_png, P_png_len};
        resources["n_png"] = {n_png, n_png_len};
        resources["N_png"] = {N_png, N_png_len};
        resources["b_png"] = {b_png, b_png_len};
        resources["B_png"] = {B_png, B_png_len};
        resources["r_png"] = {r_png, r_png_len};
        resources["R_png"] = {R_png, R_png_len};
        resources["q_png"] = {q_png, q_png_len};
        resources["Q_png"] = {Q_png, Q_png_len};
        resources["k_png"] = {k_png, k_png_len};
        resources["K_png"] = {K_png, K_png_len};
        resources["mark_png"] = {mark_png, mark_png_len};
        resources["last_png"] = {last_png, last_png_len};
        resources["round_png"] = {round_png, round_png_len};
        resources["capture_mp3"] = {capture_mp3, capture_mp3_len};
        resources["move_self_mp3"] = {move_self_mp3, move_self_mp3_len};
    }
};

#endif