#include "NextKB.h"

namespace PKB {

    void NextKB::addNext(StmtId s1, StmtId s2) {
        nextRS& nRS1 = nextTable[s1];
        nextRS& nRS2 = nextTable[s2];

        nRS1.directNext.insert(s2);
        nRS2.directPrev.insert(s1);

        allStmtsWNext.insert(s1);
        allStmtsWPrev.insert(s2);
    }

    void NextKB::addNotNextStar(StmtId s1, StmtId s2) {
        falseNextStarTable[s1].insert(s2);
    }

    bool NextKB::next(StmtId s1, StmtId s2) {
        try {
            std::unordered_set<StmtId> dirNext = nextTable.at(s1).directNext;
            return dirNext.find(s2) != dirNext.end();
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    bool NextKB::nextStar(StmtId s1, StmtId s2) {
        if (nextStarTable.find(s1) == nextStarTable.end()) {
            initCacheEntry(s1);
        }
        if (nextStarTable.find(s2) == nextStarTable.end()) {
            initCacheEntry(s2);
        }

        std::unordered_set<StmtId> allNext = nextStarTable.at(s1).allNext;
        return allNext.find(s2) != allNext.end();
    }

    bool NextKB::notNextStar(StmtId s1, StmtId s2) {
        try {
            std::unordered_set<StmtId> falseRelations = falseNextStarTable.at(s1);
            return falseRelations.find(s2) != falseRelations.end();
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    const std::unordered_set<StmtId>& NextKB::getDirectNodes(StmtId s, NodeType type) {
        try {
            if (type == NodeType::SUCCESSOR) {
                return nextTable.at(s).directNext;
            } else {
                return nextTable.at(s).directPrev;
            }
        } catch (const std::out_of_range&) {
            return EMPTY_RESULT;
        }
    }

    const std::unordered_set<StmtId>& NextKB::getAllNodes(StmtId s, NodeType type) {
        if (type == NodeType::SUCCESSOR) {
            return nextStarTable.at(s).allNext;
        } else {
            return nextStarTable.at(s).allPrev;
        }
    }

    void NextKB::addToAll(StmtId s1, StmtId s2, NodeType type) {
        if (type == NodeType::SUCCESSOR) {
            nextStarTable[s1].allNext.insert(s2);
        } else {
            nextStarTable[s1].allPrev.insert(s2);
        }
    }

    bool NextKB::hasNext(StmtId s) {
        try {
            return nextTable.at(s).directNext.size() != 0;
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    bool NextKB::hasPrev(StmtId s) {
        try {
            return nextTable.at(s).directPrev.size() != 0;
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    bool NextKB::processedAll(StmtId s, NodeType type) {
        try {
            if (type == NodeType::SUCCESSOR) {
                return nextStarTable.at(s).processedAllNext;
            } else {
                return nextStarTable.at(s).processedAllPrev;
            }
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    void NextKB::setProcessedAll(StmtId s, NodeType type) {
        if (type == NodeType::SUCCESSOR) {
            nextStarTable.at(s).processedAllNext = true;
            if (allStmtsWNext.find(s) != allStmtsWNext.end()) {
                stmtsProcessedAllNextStar.insert(s);
            }
            if (stmtsProcessedAllNextStar.size() == allStmtsWNext.size()) {
                nextStarFullyComputed = true;
            }
        } else {
            nextStarTable.at(s).processedAllPrev = true;
            if (allStmtsWPrev.find(s) != allStmtsWPrev.end()) {
                stmtsProcessedAllPrevStar.insert(s);
            }
            if (stmtsProcessedAllPrevStar.size() == allStmtsWPrev.size()) {
                nextStarFullyComputed = true;
            }
        }
    }

    bool NextKB::hasNextRelation() {
        return nextTable.size() != 0;
    }

    bool NextKB::existsInNext(StmtId s) {
        return nextTable.find(s) != nextTable.end();
    }

    void NextKB::clear() {
        nextStarTable.clear();
        falseNextStarTable.clear();
        stmtsProcessedAllNextStar.clear();
        stmtsProcessedAllPrevStar.clear();
        nextStarFullyComputed = false;
    }

    void NextKB::initCacheEntry(StmtId s) {
        nextRS& nxtRS = nextTable.at(s);
        nextStarTable[s].allNext.insert(nxtRS.directNext.begin(), nxtRS.directNext.end());
        nextStarTable[s].allPrev.insert(nxtRS.directPrev.begin(), nxtRS.directPrev.end());
    }

    bool NextKB::nextStarIsCached(StmtId s1, StmtId s2) {
        return nextStar(s1, s2) || notNextStar(s1, s2);
    }
    bool NextKB::allNextStarFullyComputed() {
        return nextStarFullyComputed;
    }
}