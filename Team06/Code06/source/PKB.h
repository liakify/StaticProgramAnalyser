#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "StmtListTable.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "ConstTable.h"
#include "ProcTable.h"
#include "FollowsKB.h"
#include "ParentKB.h"
#include "UsesKB.h"
#include "ModifiesKB.h"
#include "PatternKB.h"
#include "CallsKB.h"
#include "NextKB.h"
#include "AffectsKB.h"

#include "RuntimeDesignExtractor.h"

namespace PKB {

    /**
         *  The PKB class is the component in SPA that stores the data extracted by the 
         *  Front-End Parser. It has virtually no logical functionality less input and ouput.
         *  However, in the event that additional computation is needed by a query, the PKB
         *  calls upon the Design Extractor in the Front End.
         */
    class PKB {
     public:
        /*
            Adds Affects(s1, s2) to the affectsTable where s2 is affected by s1
        */
        void addAffects(StmtId s1, StmtId s2);

        /*
            Adds Affects*(s1, s2) to the affectsTable where s2 is directly or indirectly affected by s1
        */
        void addAffectsStar(StmtId s1, StmtId s2);

        /*
            Returns TRUE if s2 cis affected by s1, FALSE otherwise
        */
        bool affects(StmtId s1, StmtId s2);

        /*
            Returns TRUE if Affects*(s1, s2) holds, FALSE otherwise
        */
        bool affectsStar(StmtId s1, StmtId s2);

        /*
            Returns a reference to directAffects/directAffected of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        const std::unordered_set<StmtId>& affectsGetDirectNodes(StmtId s, NodeType type);

        /*
            Returns a reference to allAffects/allAffectedBy of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        const std::unordered_set<StmtId>& affectsStarGetAllNodes(StmtId s, NodeType type);

        /*
            Returns processedDirectAffects/processedDirectAffectedBy of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        bool affectsProcessedDirect(StmtId s, NodeType type);

        /*
            Returns processedAllAffects/processedAllAffectedBy of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        bool affectsStarProcessedAll(StmtId s, NodeType type);

        /*
           Sets processedDirectAffects/processedDirectAffectedBy of s to TRUE for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        void affectsSetProcessedDirect(StmtId s, NodeType type);

        /*
            Sets processedAllAffects/processedAllAffectedBy of s to TRUE for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        void affectsStarSetProcessedAll(StmtId s, NodeType type);

        /*
            Returns TRUE if all Affects relations have been fully computed, FALSE otherwise
        */
        bool allAffectsFullyComputed();

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
            Adds Next(s1, s2) to NextKB where s2 can be executed immediately after s1
        */
        void addNext(StmtId s1, StmtId s2);

        /*
            Returns TRUE if s2 can be executed immediately after s1, FALSE otherwise
        */
        bool next(StmtId s1, StmtId s2);

        /*
            Returns TRUE if s2 can be executed in some execution sequence after s1, FALSE otherwise
        */
        bool nextStar(StmtId s1, StmtId s2);

        /*
            Returns a reference to directNext/directPrev of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        const std::unordered_set<StmtId>& nextGetDirectNodes(StmtId s, NodeType type);

        /*
            Returns a reference to allNext/allPrev of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        const std::unordered_set<StmtId>& PKB::nextStarGetAllNodes(StmtId s, NodeType type);

        /*
            Adds s2 to allNext/allPrev of s1 for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        void nextStarAddToAll(StmtId s1, StmtId s2, NodeType type);

        /*
            Returns TRUE if directNext of s is non-empty, FALSE otherwise
        */
        bool hasNext(StmtId s);

        /*
            Returns TRUE if directPrev of s is non-empty, FALSE otherwise
        */
        bool hasPrev(StmtId s);

        /*
            Returns processedAllNext/processedAllPrev of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        bool nextStarProcessedAll(StmtId s, NodeType type);

        /*
            Sets processedAllNext/processedAllPrev of s to TRUE for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        void nextStarSetProcessedAll(StmtId s, NodeType type);

        /*
            Returns TRUE if a Next relation exists, FALSE otherwise
        */
        bool hasNextRelation();

        /*
            Returns TRUE if the nextStar relation is cached, FALSE otherwise
        */
        bool nextStarIsCached(StmtId s1, StmtId s2);

        /*
            Clears the cached results
        */
        void clear();

        /*
            Following the DRY principle, the PKB layer does not expose the public APIs of the below public tables/KBs.

            Note that the internal data structures of these classes are still private,
            and thus cannot be modified directly via C++ library and require usage of the class' public API.
            Hence, coupling between PKB-PQL and FrontEnd-PKB is already minimised
            as any changes to internal implementations of these classes 
            are very unlikely to significantly impact either FrontEnd or PQL.

            Post DesignExtractor, these tables/KBs are also guaranteed to not be modified.
            Thus, the structure of PKB (w.r.t the below public tables) is guaranteed to not change. 
            Hence they do not necessarily need to be hidden and is safe for access.

            Additionally, adding boilerplate to the PKB layer that essentially only calls APIs of the below tables/KBs
            requires significant amounts of time to implement tedious refactoring, 
            introduces more overhead taking up precious evaluation time away from the Query Evaluator
            while serving no additional benefit.

            For NextKB and AffectsKB, due to involvement of the RuntimeDesignExtractor,
            PKB layer API is used to avoid multiple cyclic dependencies.
        */
        VarTable varTable;
        ProcTable procTable;
        ConstTable constTable;
        StmtListTable stmtListTable;
        StmtTable stmtTable;
        FollowsKB followsKB;
        ParentKB parentKB;
        CallsKB callsKB;
        UsesKB usesKB;
        ModifiesKB modifiesKB;
        PatternKB patternKB;

     private:
        NextKB nextKB;
        AffectsKB affectsKB;

        FrontEnd::RuntimeDesignExtractor rtDE;
    };
}
