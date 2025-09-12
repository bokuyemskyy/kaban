#pragma once

#include <span>
#include <string_view>
#include <unordered_map>

#include "resources.hpp"

using Resource = std::span<const unsigned char>;

class ResourceManager {
   public:
    static const Resource* get(std::string_view filename) {
        const auto& map = resource_map();
        auto        it  = map.find(filename);
        if (it == map.end()) return nullptr;
        return &it->second;
    }

   private:
    static const std::unordered_map<std::string_view, Resource>& resource_map() {
        static const std::unordered_map<std::string_view, Resource> resources = {
            {"p.png", {p_png, p_png_len}},
            {"P.png", {P_png, P_png_len}},
            {"n.png", {n_png, n_png_len}},
            {"N.png", {N_png, N_png_len}},
            {"b.png", {b_png, b_png_len}},
            {"B.png", {B_png, B_png_len}},
            {"r.png", {r_png, r_png_len}},
            {"R.png", {R_png, R_png_len}},
            {"q.png", {q_png, q_png_len}},
            {"Q.png", {Q_png, Q_png_len}},
            {"k.png", {k_png, k_png_len}},
            {"K.png", {K_png, K_png_len}},
            {"mark.png", {mark_png, mark_png_len}},
            {"last.png", {last_png, last_png_len}},
            {"round.png", {round_png, round_png_len}},
            {"capture.mp3", {capture_mp3, capture_mp3_len}},
            {"move_self.mp3", {move_self_mp3, move_self_mp3_len}},
        };
        return resources;
    }
};
