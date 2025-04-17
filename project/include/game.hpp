#ifndef GAME_HPP
#define GAME_HPP

#include "position.hpp"

class Game {
   public:
   private:
    Position m_position;
    std::string m_gameName;
    std::pair<std::string, std::string> m_opponents;
};

#endif