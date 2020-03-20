#include <string>
#include <vector>

#include "PKB.h"

namespace PKB {
    bool PKB::nextStar(StmtId s1, StmtId s2) {
        int numStmts = stmtTable.size();
        if (s1 < 1 || s1 > numStmts || s2 < 1 || s2 > numStmts) {
            return false;
        }
        if (nextKB.nextStar(s1, s2)) {  // cached
            return true;
        }
        if (nextKB.processedAll(s1, NodeType::SUCCESSOR)) {  // allNext is fully processed for s1, i.e. no path from s1 to s2
            return false;
        }
        return rtDE.nextStar(s1, s2, this);
    }

    const std::unordered_set<StmtId>& PKB::nextStarGetAllNodes(StmtId s, NodeType type) {
        int numStmts = stmtTable.size();
        if (s < 1 || s > numStmts) {
            return EMPTY_RESULT;
        }
        if (nextKB.processedAll(s, type)) {  // cached
            return nextKB.getAllNodes(s, type);
        }
        rtDE.processNextStarStmtAllNodes(s, type, this);
        return nextKB.getAllNodes(s, type);
    }

    void PKB::clear() {
        nextKB.clear();
    }
}
