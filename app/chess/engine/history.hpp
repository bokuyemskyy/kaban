#include "move.hpp"
#include "undo_info.hpp"

struct HistoryEntry {
    Move     move;
    UndoInfo undo_info;
};

class History {
   public:
    void push(Move move, UndoInfo undo_info) { m_entries.push_back({move, undo_info}); }

    HistoryEntry pop() {
        if (m_entries.empty()) return {};
        auto last = m_entries.back();
        m_entries.pop_back();
        return last;
    }
    [[nodiscard]] std::vector<Move> getMoveHistory() const {
        std::vector<Move> moves;
        moves.reserve(m_entries.size());

        std::ranges::transform(m_entries, std::back_inserter(moves),
                               [](const HistoryEntry& entry) { return entry.move; });

        return moves;
    }

   private:
    std::vector<HistoryEntry> m_entries{};
};