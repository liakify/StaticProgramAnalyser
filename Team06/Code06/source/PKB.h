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
            Returns TRUE if s2 cis affected by s1, FALSE otherwise
        */
        bool affects(StmtId s1, StmtId s2);

        /*
            Returns a reference to directAffects/directAffected of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        const std::unordered_set<StmtId>& affectsGetDirectNodes(StmtId s, NodeType type);

        /*
            Returns processedAllAffects/processedAllAffectedBy of s for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        bool affectsProcessedAll(StmtId s, NodeType type);

        /*
           Sets processedAllAffects/processedAllAffectedBy of s to TRUE for NodeType SUCCESSOR and PREDECESSOR respectively
        */
        void affectsSetProcessedAll(StmtId s, NodeType type);

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
        const std::unordered_set<StmtId>& nextStarGetDirectNodes(StmtId s, NodeType type);

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
            Clears the cached results
        */
        void clear();

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
