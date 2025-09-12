#pragma once

#include "square.hpp"
struct GuiState {
    bool   shouldClose    = false;
    bool   showDemoWindow = false;
    Square selectedSquare = Squares::NONE;
};