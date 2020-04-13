#pragma once

#include <algorithm>
#include <climits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Types.h"
#include "Constants.h"

namespace PKB {

    struct parentRS {
        StmtId parent = 0;
        std::unordered_set<StmtId> directChildren;
        std::unordered_set<StmtId> allParents;
        std::unordered_set<StmtId> allChildren;
        StmtId rootParent = INT_MAX;
    };

    class ParentKB {
     public:
        /*
            Adds Parent(stmtId1, stmtId2) relation to parentTable.
        */
        void addParent(StmtId stmtId1, StmtId stmtId2);

        /*
            Returns TRUE if Parent(stmtId1, stmtId2) is true, FALSE otherwise.
        */
        bool parent(StmtId stmtId1, StmtId stmtId2);

        /*
            Returns TRUE if Parent*(stmtId1, stmtId2) is true, FALSE otherwise.
        */
        bool parentStar(StmtId stmtId1, StmtId stmtId2);

        /*
            Returns statement ID s for which Parent(s, stmtId) is true.
            If stmtId is not found or stmtId has no parent, INVALID_STMT_ID 0 is returned.
        */
        StmtId getParent(StmtId stmtId);

        /*
            Returns unordered_set of child statement IDs for which Parent(stmtId, s) is true.
            If stmtId is not found or stmtId has direct children, empty set is returned.
        */
        const std::unordered_set<StmtId>& getDirectChildren(StmtId stmtId);

        /*
            Returns the root parent of stmtId i.e. smallest StmtId s1 for which Parent*(s1, stmtId) is true,
            returns INT_MAX if no such root parent exists.
        */
        StmtId getRootParent(StmtId stmtId);

        /*
            Returns TRUE if stmtId has a parent, FALSE otherwise.
        */
        bool hasParent(StmtId stmtId);

        /*
            Returns TRUE if stmtId has direct children, FALSE otherwise.
        */
        bool hasDirectChildren(StmtId stmtId);

        /*
            Returns all statement IDs s for which Parent*(s, stmtId) is true.
        */
        const std::unordered_set<StmtId>& getAllParents(StmtId stmtId);

        /*
            Returns all statement IDs s for which Parent*(stmtId, s) is true.
        */
        const std::unordered_set<StmtId>& getAllChildren(StmtId stmtId);

        /*
            Sets allChildren of stmtId to children
        */
        void setAllChildren(StmtId stmtId, const std::unordered_set<StmtId>& children);

        /*
            Sets allParents of stmtId to parents
        */
        void setAllParents(StmtId stmtId, const std::unordered_set<StmtId>& parents);

        /*
            Returns TRUE if any Parent relation is present, FALSE otherwise.
        */
        bool hasParentRelation();

     private:
        std::unordered_map<StmtId, parentRS> parentTable;
    };
}