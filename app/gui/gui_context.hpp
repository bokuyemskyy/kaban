#pragma once

#include <imgui.h>

#include "engine.hpp"
#include "gui_state.hpp"
#include "texture_manager.hpp"
#include "window_registry.hpp"

struct GuiContext {
    GuiState&       state;
    ImGuiIO&        io;
    Engine&         engine;
    TextureManager& textureManager;
    WindowRegistry& windowRegistry;
};