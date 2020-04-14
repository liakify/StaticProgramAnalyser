#include "ParentKB.h"
#include <stdexcept>

namespace PKB {

    void ParentKB::addParent(StmtId stmtId1, StmtId stmtId2) {
        parentRS& pRS1 = parentTable[stmtId1];
        parentRS& pRS2 = parentTable[stmtId2];

        pRS1.directChildren.insert(stmtId2);
        pRS2.parent = stmtId1;
    }

    bool ParentKB::parent(StmtId stmtId1, StmtId stmtId2) {
        try {
            std::unordered_set<StmtId> stmtId1Children = parentTable.at(stmtId1).directChildren;
            return stmtId1Children.find(stmtId2) != stmtId1Children.end();
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    bool ParentKB::parentStar(StmtId stmtId1, StmtId stmtId2) {
        try {
            std::unordered_set<StmtId> stmtId1AllChildren = parentTable.at(stmtId1).allChildren;
            return stmtId1AllChildren.find(stmtId2) != stmtId1AllChildren.end();
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    StmtId ParentKB::getParent(StmtId stmtId) {
        try {
            return parentTable.at(stmtId).parent;
        } catch (const std::out_of_range&) {
            return INVALID_STMT_ID;
        }
    }

    const std::unordered_set<StmtId>& ParentKB::getDirectChildren(StmtId stmtId) {
        try {
            return parentTable.at(stmtId).directChildren;
        } catch (const std::out_of_range&) {
            return EMPTY_RESULT;
        }
    }

    StmtId ParentKB::getRootParent(StmtId stmtId) {
        return parentTable.at(stmtId).rootParent;
    }

    bool ParentKB::hasParent(StmtId stmtId) {
        try {
            return parentTable.at(stmtId).parent != 0;
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    bool ParentKB::hasDirectChildren(StmtId stmtId) {
        try {
            return parentTable.at(stmtId).directChildren.size() != 0;
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    const std::unordered_set<StmtId>& ParentKB::getAllParents(StmtId stmtId) {
        try {
            return parentTable.at(stmtId).allParents;
        } catch (const std::out_of_range&) {
            return EMPTY_RESULT;
        }
    }

    const std::unordered_set<StmtId>& ParentKB::getAllChildren(StmtId stmtId) {
        try {
            return parentTable.at(stmtId).allChildren;
        } catch (const std::out_of_range&) {
            return EMPTY_RESULT;
        }
    }

    void ParentKB::setAllChildren(StmtId stmtId, const std::unordered_set<StmtId>& children) {
        parentTable.at(stmtId).allChildren.insert(children.begin(), children.end());
    }

    void ParentKB::setAllParents(StmtId stmtId, const std::unordered_set<StmtId>& parents) {
        parentRS& pRS = parentTable.at(stmtId);
        for (const auto& p : parents) {
            pRS.allParents.insert(p);
            pRS.rootParent = std::min(pRS.rootParent, p);
        }
    }

    bool ParentKB::hasParentRelation() {
        return parentTable.size() != 0;
    }
}