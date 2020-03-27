#include "AffectsKB.h"

void AffectsKB::addAffects(StmtId s1, StmtId s2) {
    affectsRS& aRS1 = affectsTable[s1];
    affectsRS& aRS2 = affectsTable[s2];

    aRS1.directAffects.insert(s2);
    aRS2.directAffectedBy.insert(s1);
}

bool AffectsKB::affects(StmtId s1, StmtId s2) {
    try {
        std::unordered_set<StmtId> dirAffects = affectsTable.at(s1).directAffects;
        return dirAffects.find(s2) != dirAffects.end();
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

void AffectsKB::clear() {
    affectsTable.clear();
}