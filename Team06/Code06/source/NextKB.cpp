#include "NextKB.h"
#include "PKB.h"

NextKB::NextKB(PKB::PKB* pkb)
    : pkb(pkb), rtDE(pkb) {
}

void NextKB::addNext(StmtId s1, StmtId s2) {
    nextRS& nRS1 = nextTable[s1];
    nextRS& nRS2 = nextTable[s2];

    nRS1.directNext.insert(s2);
    nRS2.directPrev.insert(s1);
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
    int numStmts = pkb->stmtTable.size();
    if (s1 < 1 || s1 > numStmts || s2 < 1 || s2 > numStmts) {
        return false;
    }
    if (nextStarTable.find(s1) != nextStarTable.end()) {
        std::unordered_set<StmtId> allNext = nextStarTable.at(s1).allNext;
        if (allNext.find(s2) != allNext.end()) {  // cached
            return true;
        }
        if (nextStarTable.at(s1).processedAllNext) {  // s1 fully processed, s2 not in execution sequence
            return false;
        }
    }
    return rtDE.nextStar(s1, s2);
}

const std::unordered_set<StmtId>& NextKB::getDirectNodes(StmtId s, NodeType type) {
    try {
        if (type == NodeType::SUCCESSOR) {
            return nextTable.at(s).directNext;
        } else {
            return nextTable.at(s).directPrev;
        }
    }
    catch (const std::out_of_range&) {
        return EMPTY_RESULT;
    }
}

const std::unordered_set<StmtId>& NextKB::getAllNodes(StmtId s, NodeType type) {
    int numStmts = pkb->stmtTable.size();
    if (s < 1 || s > numStmts) {
        return EMPTY_RESULT;
    }
    if (nextStarTable.find(s) != nextStarTable.end()) {
        if (type == NodeType::SUCCESSOR) {
            if (nextStarTable.at(s).processedAllNext) {  // cached
                return nextStarTable.at(s).allNext;
            }
        } else {
            if (nextStarTable.at(s).processedAllPrev) {  // cached
                return nextStarTable.at(s).allPrev;
            }
        }
    }
    rtDE.processStmtAllNodes(s, type);
    if (type == NodeType::SUCCESSOR) {
        return nextStarTable.at(s).allNext;
    } else {
        return nextStarTable.at(s).allPrev;
    }
}

void NextKB::addToAll(StmtId s1, StmtId s2, NodeType type) {
    if (type == NodeType::SUCCESSOR) {
        nextStarTable.at(s1).allNext.insert(s2);
    } else {
        nextStarTable.at(s1).allPrev.insert(s2);
    }
}

bool NextKB::hasNext(StmtId s) {
    try {
        return nextTable.at(s).directNext.size() != 0;
    }
    catch (const std::out_of_range&) {
        return false;
    }
}

bool NextKB::hasPrev(StmtId s) {
    try {
        return nextTable.at(s).directNext.size() != 0;
    }
    catch (const std::out_of_range&) {
        return false;
    }
}

void NextKB::setProcessedAll(StmtId s, NodeType type) {
    if (type == NodeType::SUCCESSOR) {
        nextStarTable.at(s).processedAllNext = true;
    } else {
        nextStarTable.at(s).processedAllPrev = true;
    }
}

bool NextKB::hasNextRelation() {
    return nextTable.size() != 0;
}

void NextKB::clear() {
    nextStarTable.clear();
}
