#pragma once
#include "gui_context.hpp"

struct IGuiComponent {
    IGuiComponent(GuiContext& ctx) : m_ctx(ctx) {}
    virtual ~IGuiComponent() = default;
    virtual void draw()      = 0;

   protected:
    GuiContext& m_ctx;
};
