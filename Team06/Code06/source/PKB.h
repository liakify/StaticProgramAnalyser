#pragma once

#include <string>
#include <unordered_map>
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
        bool nextStar(StmtId s1, StmtId s2);

        const std::unordered_set<StmtId>& PKB::nextStarGetAllNodes(StmtId s, NodeType type);

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
        NextKB nextKB;

     private:
         FrontEnd::RuntimeDesignExtractor rtDE;
    };
}
