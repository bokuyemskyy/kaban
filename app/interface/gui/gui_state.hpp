#pragma once

#include "square.hpp"
struct GuiState {
    bool   should_close     = false;
    bool   show_demo_window = false;
    Square selected_square  = Squares::NONE;
};