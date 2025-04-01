#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "layout_manager.hpp"
#include <GLFW/glfw3.h>
#include <game.hpp>
#include <renderer.hpp>

class InputHandler {
public:
  InputHandler(Renderer *renderer, Game *game)
      : m_renderer(renderer), m_game(game),
        m_layoutManager(renderer->getLayoutManager()) {};
  void update();
  void handleInput();

private:
  Renderer *m_renderer;
  Game *m_game;
  LayoutManager *m_layoutManager;

  bool m_prevLMB = false;
  bool m_prevRMB = false;
};

#endif
