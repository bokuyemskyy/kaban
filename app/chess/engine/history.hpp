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

   private:
    std::vector<HistoryEntry> m_entries{};
};