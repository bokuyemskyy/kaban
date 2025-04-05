#ifndef LAYOUT_MANAGER_HPP
#define LAYOUT_MANAGER_HPP

#include <cstdint>
#include <navigation.hpp>
#include <string>
#include <unordered_map>

#include "constants.hpp"
#include "utils.hpp"

struct PixelSector {
    float x, y, w, h;
};
struct Sector {
    float xRatio, yRatio, wRatio, hRatio;
};

class LayoutManager {
   private:
    std::unordered_map<std::string, Sector> m_sectors;
    Dimensions &m_dimensions;

   public:
    void attachDimensions(Dimensions &dimensions) { m_dimensions = dimensions; };
    void defineSector(const std::string &name, Sector sector) { m_sectors[name] = sector; }

    const Sector &getPixelSector(const std::string &name) const {
        const Sector *sector = &(sectors.at(name));
        return PixelSector(sector->xRatio *);
    }

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