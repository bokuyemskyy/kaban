#ifndef LAYOUT_MANAGER_HPP
#define LAYOUT_MANAGER_HPP

#include <cstdint>
#include <navigation.hpp>
#include <string>
#include <unordered_map>

#include "constants.hpp"

struct Sector {
    float x, y, width, height;
};

class LayoutManager {
   private:
    std::unordered_map<std::string, Sector> sectors;

   public:
    void defineSector(const std::string &name, float x, float y, float w, float h) {
        sectors[name] = {x, y, w, h};
    }

    const Sector &getSector(const std::string &name) const { return sectors.at(name); }

    Square getBoardSquarePosition(float mouseX, float mouseY) const {
        Sector gameSector = getSector("Game");
        if (mouseX < gameSector.x || mouseX > gameSector.x + gameSector.width ||
            mouseY < gameSector.y || mouseY > gameSector.y + gameSector.height)
            return NO_SQ;

        float cellSizeX = gameSector.width / 8;
        float cellSizeY = gameSector.height / 8;

        auto col = static_cast<uint8_t>((mouseX - gameSector.x) / cellSizeX);
        auto row = static_cast<uint8_t>(((gameSector.height - mouseY) - gameSector.y) / cellSizeY);

        return Square(row * 8 + col);
    }
};

#endif