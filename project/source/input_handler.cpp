#include <imgui.h>
#include <imgui_internal.h>

#include <game.hpp>
#include <input_handler.hpp>
#include <layout_manager.hpp>
#include <navigation.hpp>

void InputHandler::update() { handleInput(); }

void InputHandler::handleInput() {
    if (ImGui::GetCurrentContext() == nullptr) {
        return;
    }
    Square screenSquare =
        m_layoutManager->getBoardSquarePosition(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
    Square relativeSquare = screenSquare;  // getTurnSquare(screenSquare, m_game->getTurn());
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        if (!m_prevLMB) {
            m_game->holdPiece(relativeSquare);
            m_prevLMB = true;
        }
    } else {
        if (m_prevLMB) {
            m_game->releasePiece();
            m_prevLMB = false;
        }
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        if (!m_prevRMB) {
            m_game->holdPiece(relativeSquare);
            m_prevRMB = true;
        }
    } else {
        if (m_prevRMB) {
            m_game->releasePiece();
            m_prevRMB = false;
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
        m_renderer->terminate();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_X)) {
        // m_game->test();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_F)) {
        m_renderer->toggleDemoWindow();
    }
}
