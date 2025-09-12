#pragma once
#include "gui_context.hpp"

struct IGuiComponent {
    IGuiComponent(GuiContext& context) : m_context(context) {}
    virtual ~IGuiComponent() = default;
    virtual void draw()      = 0;

   protected:
    GuiContext& m_context;
};
