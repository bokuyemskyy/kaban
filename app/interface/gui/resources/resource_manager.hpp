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
            {"pb.png", {static_cast<const unsigned char*>(pb_png), pb_png_len}},
            {"pw.png", {static_cast<const unsigned char*>(pw_png), pw_png_len}},
            {"nb.png", {static_cast<const unsigned char*>(nb_png), nb_png_len}},
            {"nw.png", {static_cast<const unsigned char*>(nw_png), nw_png_len}},
            {"bb.png", {static_cast<const unsigned char*>(bb_png), bb_png_len}},
            {"bw.png", {static_cast<const unsigned char*>(bw_png), bw_png_len}},
            {"rb.png", {static_cast<const unsigned char*>(rb_png), rb_png_len}},
            {"rw.png", {static_cast<const unsigned char*>(rw_png), rw_png_len}},
            {"qb.png", {static_cast<const unsigned char*>(qb_png), qb_png_len}},
            {"qw.png", {static_cast<const unsigned char*>(qw_png), qw_png_len}},
            {"kb.png", {static_cast<const unsigned char*>(kb_png), kb_png_len}},
            {"kw.png", {static_cast<const unsigned char*>(kw_png), kw_png_len}},
            {"mark.png", {static_cast<const unsigned char*>(mark_png), mark_png_len}},
            {"last.png", {static_cast<const unsigned char*>(last_png), last_png_len}},
            {"round.png", {static_cast<const unsigned char*>(round_png), round_png_len}},
            {"capture.mp3", {static_cast<const unsigned char*>(capture_mp3), capture_mp3_len}},
            {"move_self.mp3", {static_cast<const unsigned char*>(move_self_mp3), move_self_mp3_len}},
        };
        return resources;
    }
};
