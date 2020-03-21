#include "NextKB.h"

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
    if (nextTable.find(s1) == nextTable.end() || nextTable.find(s2) == nextTable.end()) {
        return false;
    }
    if (nextStarTable.find(s1) == nextStarTable.end()) {
        initCache(s1);
    }
    if (nextStarTable.find(s2) == nextStarTable.end()) {
        initCache(s2);
    }

    std::unordered_set<StmtId> allNext = nextStarTable.at(s1).allNext;
    return allNext.find(s2) != allNext.end();
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
    if (nextTable.find(s) == nextTable.end()) {
        return EMPTY_RESULT;
    }
    if (nextStarTable.find(s) == nextStarTable.end()) {
        initCache(s);
    }
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
    }
    catch (const std::out_of_range&) {
        return false;
    }
}

bool NextKB::hasPrev(StmtId s) {
    try {
        return nextTable.at(s).directPrev.size() != 0;
    }
    catch (const std::out_of_range&) {
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
    try {
        if (type == NodeType::SUCCESSOR) {
            nextStarTable.at(s).processedAllNext = true;
        } else {
            nextStarTable.at(s).processedAllPrev = true;
        }
    } catch (const std::out_of_range&) {
        return;
    }
}

bool NextKB::hasNextRelation() {
    return nextTable.size() != 0;
}

void NextKB::clear() {
    nextStarTable.clear();
}

void NextKB::initCache(StmtId s) {
    nextRS& nxtRS = nextTable.at(s);
    nextStarTable[s].allNext.insert(nxtRS.directNext.begin(), nxtRS.directNext.end());
    nextStarTable[s].allPrev.insert(nxtRS.directPrev.begin(), nxtRS.directPrev.end());
}
