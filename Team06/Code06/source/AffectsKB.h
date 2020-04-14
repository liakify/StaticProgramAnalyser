#pragma once

#include <stdexcept>
#include "unordered_map"
#include "unordered_set"

#include "Constants.h"
#include "Types.h"

namespace PKB {

    struct affectsRS {
        std::unordered_set<StmtId> directAffects;
        std::unordered_set<StmtId> directAffectedBy;
        bool processedDirectAffects = false;
        bool processedDirectAffectedBy = false;

        std::unordered_set<StmtId> allAffects;
        std::unordered_set<StmtId> allAffectedBy;
        bool processedAllAffects = false;
        bool processedAllAffectedBy = false;
    };

    class AffectsKB {
     public:
        /*
            Adds Affects(s1, s2) to the affectsTable where s2 is affected by s1
        */
        void addAffects(StmtId s1, StmtId s2);

        /*
            Adds s2 to the set of statements that is NOT affected by s1 in falseAffectsTable
        */
        void addNotAffects(StmtId s1, StmtId s2);

        /*
            Adds Affects*(s1, s2) to the affectsTable where s2 is directly or indirectly affected by s1
        */
        void addAffectsStar(StmtId s1, StmtId s2);

        /*
            Adds s2 to the set of statements that is NOT directly or indirectly affected by s1 in falseAffectsStarTable
        */
        void addNotAffectsStar(StmtId s1, StmtId s2);

        /*
            Returns TRUE if s2 is affected by s1, FALSE otherwise
        */
        bool affects(StmtId s1, StmtId s2);

        /*
            Returns TRUE if s2 is NOT affected by s1, FALSE otherwise
        */
        bool notAffects(StmtId s1, StmtId s2);

        /*
            Returns TRUE if Affects*(s1, s2) holds, FALSE otherwise
        */
        bool affectsStar(StmtId s1, StmtId s2);

        /*
            Returns TRUE if Affects*(s1, s2) does NOT hold, FALSE otherwise
        */
        bool notAffectsStar(StmtId s1, StmtId s2);

        /*
            Returns a reference to directAffects/directAffectedBy of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        const std::unordered_set<StmtId>& getDirectNodes(StmtId s, NodeType type);

        /*
            Returns a reference to allAffects/allAffectedBy of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        const std::unordered_set<StmtId>& getAllNodes(StmtId s, NodeType type);

        /*
            Returns processedDirectAffects/processedDirectAffectedBy of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        bool processedDirectAffects(StmtId s, NodeType type);

        /*
            Returns processedAllAffects/processedAllAffectedBy of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        bool processedAllAffects(StmtId s, NodeType type);

        /*
           Sets processedDirectAffects/processedDirectAffectedBy of s to TRUE for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        void setProcessedDirectAffects(StmtId s, NodeType type);

        /*
            Sets processedAllAffects/processedAllAffectedBy of s to TRUE for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        void setProcessedAllAffects(StmtId s, NodeType type);

        /*
            Returns TRUE if all Affects relations have been fully computed, FALSE otherwise
        */
        bool allAffectsFullyComputed();

        /*
            Returns TRUE if all Affects* relations have been fully computed, FALSE otherwise
        */
        bool allAffectsStarFullyComputed();

        /*
            Sets affectsFullyComputed to TRUE
        */
        void setAffectsFullyComputed();

        /*
            Returns TRUE if the affects relation is cached, FALSE otherwise
        */
        bool affectsIsCached(StmtId s1, StmtId s2);

        /*
            Returns TRUE if the affectsStar relation is cached, FALSE otherwise
        */
        bool affectsStarIsCached(StmtId s1, StmtId s2);

        /*
            Initializes numAssignStmts in AffectsKB to num
        */
        void init(int num);

        /*
            Clears the affectsTable
        */
        void clear();

     private:
        bool affectsFullyComputed = false;
        bool affectsStarFullyComputed = false;
        std::unordered_map<StmtId, affectsRS> affectsTable;

        std::unordered_map<StmtId, std::unordered_set<StmtId>> falseAffectsTable;
        std::unordered_map<StmtId, std::unordered_set<StmtId>> falseAffectsStarTable;

        int numAssignStmts;

        std::unordered_set<StmtId> stmtsProcessedDirectAffects;
        std::unordered_set<StmtId> stmtsProcessedDirectAffectedBy;

        std::unordered_set<StmtId> stmtsProcessedAllAffectsStar;
        std::unordered_set<StmtId> stmtsProcessedAllAffectedByStar;
    };
}