#include "AffectsKB.h"

void AffectsKB::addAffects(StmtId s1, StmtId s2) {
    affectsRS& aRS1 = affectsTable[s1];
    affectsRS& aRS2 = affectsTable[s2];

    aRS1.directAffects.insert(s2);
    aRS2.directAffectedBy.insert(s1);
}

void AffectsKB::addAffectsStar(StmtId s1, StmtId s2) {
    affectsRS& aRS1 = affectsTable[s1];
    affectsRS& aRS2 = affectsTable[s2];

    aRS1.allAffects.insert(s2);
    aRS2.allAffectedBy.insert(s1);
}

bool AffectsKB::affects(StmtId s1, StmtId s2) {
    try {
        std::unordered_set<StmtId> dirAffects = affectsTable.at(s1).directAffects;
        return dirAffects.find(s2) != dirAffects.end();
    } catch (const std::out_of_range&) {
        return false;
    }
}

bool AffectsKB::affectsStar(StmtId s1, StmtId s2) {
    try {
        std::unordered_set<StmtId> allAffects = affectsTable.at(s1).allAffects;
        return allAffects.find(s2) != allAffects.end();
    } catch (const std::out_of_range&) {
        return false;
    }
}

const std::unordered_set<StmtId>& AffectsKB::getDirectNodes(StmtId s, NodeType type) {
    if (affectsTable.find(s) == affectsTable.end()) {
        return EMPTY_RESULT;
    }
    if (type == NodeType::SUCCESSOR) {
        return affectsTable.at(s).directAffects;
    } else {
        return affectsTable.at(s).directAffectedBy;
    }
}

const std::unordered_set<StmtId>& AffectsKB::getAllNodes(StmtId s, NodeType type) {
    if (affectsTable.find(s) == affectsTable.end()) {
        return EMPTY_RESULT;
    }
    if (type == NodeType::SUCCESSOR) {
        return affectsTable.at(s).allAffects;
    } else {
        return affectsTable.at(s).allAffectedBy;
    }
}

bool AffectsKB::processedDirectAffects(StmtId s, NodeType type) {
    try {
        if (type == NodeType::SUCCESSOR) {
            return affectsTable.at(s).processedDirectAffects;
        } else {
            return affectsTable.at(s).processedDirectAffectedBy;
        }
    } catch (const std::out_of_range&) {
        return false;
    }
}

bool AffectsKB::processedAllAffects(StmtId s, NodeType type) {
    try {
        if (type == NodeType::SUCCESSOR) {
            return affectsTable.at(s).processedAllAffects;
        } else {
            return affectsTable.at(s).processedAllAffectedBy;
        }
    } catch (const std::out_of_range&) {
        return false;
    }
}

void AffectsKB::setProcessedDirectAffects(StmtId s, NodeType type) {
    try {
        if (type == NodeType::SUCCESSOR) {
            affectsTable.at(s).processedDirectAffects = true;
        } else {
            affectsTable.at(s).processedDirectAffectedBy = true;
        }
    } catch (const std::out_of_range&) {
        return;
    }
}

void AffectsKB::setProcessedAllAffects(StmtId s, NodeType type) {
    try {
        if (type == NodeType::SUCCESSOR) {
            affectsTable.at(s).processedAllAffects = true;
        } else {
            affectsTable.at(s).processedAllAffectedBy = true;
        }
    } catch (const std::out_of_range&) {
        return;
    }
}

bool AffectsKB::allAffectsFullyComputed() {
    return affectsFullyComputed;
}

void AffectsKB::setAffectsFullyComputed() {
    affectsFullyComputed = true;
}

void AffectsKB::clear() {
    affectsTable.clear();
}