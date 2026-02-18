#include <imgui.h>

#include <cstring>
#include <iomanip>
#include <string>

#include "gui_component.hpp"
#include "gui_context.hpp"

class GameInfo : IGuiComponent {
   public:
    using IGuiComponent::IGuiComponent;

    void draw() override {
        ImGui::Text("Game Information");

        ImGui::Separator();

        ImGui::Text("Game Status:");

        std::string turn;
        std::string result;
        if (m_ctx.engine.isGameOver()) {
            turn = "-";
            if (m_ctx.engine.isCheck()) {
                std::string winner = (m_ctx.engine.getSideToMove() == Colors::WHITE ? "Black" : "White");

                result = "Won by " + winner;
            } else {
                result = "Draw";
            }
        } else {
            turn   = (m_ctx.engine.getSideToMove() == Colors::WHITE ? "White" : "Black");
            result = "In progress";
        }
        ImGui::Text("  Turn: %s", turn.c_str());
        ImGui::Text("  Result: %s", result.c_str());

        ImGui::Separator();

        ImGui::Text("Engine Evaluation:");
        ImGui::Text("  Score: %s", evalToString(m_ctx.engine.getCurrentEval()).c_str());
        ImGui::Text("  Best move: %s", m_ctx.engine.getCurrentBestMove().toString().c_str());
        ImGui::Text("  Depth: %d", m_ctx.engine.getCurrentDepth());

        ImGui::Separator();
        ImGui::Text("FEN");

        const std::string currentFen = m_ctx.engine.toFen();

        static std::array<char, 256> fenBuffer{};
        static std::string           lastFen;

        if (currentFen != lastFen) {
            strncpy(fenBuffer.data(), currentFen.c_str(), fenBuffer.size() - 1);
            fenBuffer[fenBuffer.size() - 1] = '\0';
            lastFen                         = currentFen;
        }

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##fen_input", fenBuffer.data(), fenBuffer.size());

        if (ImGui::Button("Copy")) {
            ImGui::SetClipboardText(fenBuffer.data());
        }

        ImGui::SameLine();

        if (ImGui::Button("Apply")) {
            try {
                m_ctx.engine.fromFen(fenBuffer.data());
                lastFen.clear();
            } catch (...) {
            }
        }

        ImGui::Separator();

        ImGui::Text("Move History:");

        if (ImGui::BeginChild("HistoryRegion", ImVec2(0, 0), true)) {
            const auto& history = m_ctx.engine.getMoveHistory();

            for (size_t i = 0; i < history.size(); ++i) {
                if (i % 2 == 0) {
                    int moveNum = (i / 2) + 1;
                    ImGui::Text("%d. %s", moveNum, history[i].toString().c_str());
                } else {
                    ImGui::SameLine();
                    ImGui::Text("%s", history[i].toString().c_str());
                }
            }

            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::EndChild();
        }
    }

   private:
    static std::string evalToString(int score) {
        const int MATE_THRESHOLD        = 10000;
        const int MAX_SCORE_FOR_DISPLAY = 500;

        if (std::abs(score) > MATE_THRESHOLD) {
            int movesToMate = (30000 - std::abs(score));
            if (score > 0)
                return "Mate in " + std::to_string(movesToMate);
            else
                return "Mate in " + std::to_string(movesToMate) + " (losing)";
        }

        double absScore = std::abs(score);
        double scaled;

        scaled = std::log1p(9.0 * (absScore / MAX_SCORE_FOR_DISPLAY) * (absScore / MAX_SCORE_FOR_DISPLAY));
        if (score < 0) scaled = -scaled;

        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << scaled;
        return out.str();
    }
};