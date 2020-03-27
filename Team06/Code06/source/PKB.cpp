#include <memory>
#include <string>
#include <vector>

#include "PKB.h"

namespace PKB {
    void PKB::addAffects(StmtId s1, StmtId s2) {
        affectsKB.addAffects(s1, s2);
    }

    bool PKB::affects(StmtId s1, StmtId s2) {
        std::shared_ptr<Statement> x, y;
        try {
            x = stmtTable.get(s1);
            y = stmtTable.get(s2);
        } catch (std::out_of_range&) {
            return false;
        }
        if (x->getType() != StmtType::ASSIGN || y->getType() != StmtType::ASSIGN) {
            return false;
        }
        if (affectsKB.affects(s1, s2)) {  // cached
            return true;
        }
        if (affectsKB.processedAllAffects(s1, NodeType::SUCCESSOR)) {  // allNext is fully processed for s1, i.e. no path from s1 to s2
            return false;
        }
        return rtDE.processAffects(s1, s2, this);
    }

    const std::unordered_set<StmtId>& PKB::affectsGetDirectNodes(StmtId s, NodeType type) {
        std::shared_ptr<Statement> x;
        try {
            x = stmtTable.get(s);
        } catch (std::out_of_range&) {
            return EMPTY_RESULT;
        }
        if (x->getType() != StmtType::ASSIGN) {
            return EMPTY_RESULT;
        }
        if (affectsKB.processedAllAffects(s, type)) {  // cached
            return affectsKB.getDirectNodes(s, type);
        }
        rtDE.processAffectsGetAllNodes(s, type, this);
        return affectsKB.getDirectNodes(s, type);
    }

    bool PKB::affectsProcessedAll(StmtId s, NodeType type) {
        return affectsKB.processedAllAffects(s, type);
    }

    void PKB::affectsSetProcessedAll(StmtId s, NodeType type) {
        affectsKB.setProcessedAllAffects(s, type);
    }

    void PKB::addNext(StmtId s1, StmtId s2) {
        nextKB.addNext(s1, s2);
    }

    bool PKB::next(StmtId s1, StmtId s2) {
        return nextKB.next(s1, s2);
    }

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
        return rtDE.processNextStar(s1, s2, this);
    }

    const std::unordered_set<StmtId>& PKB::nextStarGetDirectNodes(StmtId s, NodeType type) {
        return nextKB.getDirectNodes(s, type);
    }

    const std::unordered_set<StmtId>& PKB::nextStarGetAllNodes(StmtId s, NodeType type) {
        int numStmts = stmtTable.size();
        if (s < 1 || s > numStmts) {
            return EMPTY_RESULT;
        }
        if (nextKB.processedAll(s, type)) {  // cached
            return nextKB.getAllNodes(s, type);
        }
        rtDE.processNextStarGetAllNodes(s, type, this);
        return nextKB.getAllNodes(s, type);
    }

    void PKB::nextStarAddToAll(StmtId s1, StmtId s2, NodeType type) {
        nextKB.addToAll(s1, s2, type);
    }

    bool PKB::hasNext(StmtId s) {
        return nextKB.hasNext(s);
    }

    bool PKB::hasPrev(StmtId s) {
        return nextKB.hasPrev(s);
    }

    bool PKB::nextStarProcessedAll(StmtId s, NodeType type) {
        return nextKB.processedAll(s, type);
    }

    void PKB::nextStarSetProcessedAll(StmtId s, NodeType type) {
        nextKB.setProcessedAll(s, type);
    }

    bool PKB::hasNextRelation() {
        return nextKB.hasNextRelation();
    }

    void PKB::clear() {
        nextKB.clear();
        affectsKB.clear();
    }
}
