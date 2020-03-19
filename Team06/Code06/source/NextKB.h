#pragma once

#include <stdexcept>
#include "unordered_map"
#include "unordered_set"

#include "Constants.h"
#include "RuntimeDesignExtractor.h"
#include "Types.h"

namespace PKB {
    class PKB;
}

struct nextRS {
    std::unordered_set<StmtId> directPrev;
    std::unordered_set<StmtId> directNext;
};

struct nextStarRS {
    bool processedAllPrev = false;
    bool processedAllNext = false;
    std::unordered_set<StmtId> allPrev;
    std::unordered_set<StmtId> allNext;
};

class NextKB {
 public:
    NextKB(PKB::PKB* pkb);

    /*
        Adds Next(s1, s2) to the nextTable where s2 can be executed immediately after s1
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
    const std::unordered_set<StmtId>& getDirectNodes(StmtId s, NodeType type);

    /*
        Returns a reference to allNext/allPrev of s for NodeType SUCCESSOR and PREDECESSOR respectively
    */
    const std::unordered_set<StmtId>& getAllNodes(StmtId s, NodeType type);

    /*
        Adds s2 to allNext/allPrev of s1 for NodeType SUCCESSOR and PREDECESSOR respectively
    */
    void addToAll(StmtId s1, StmtId s2, NodeType type);

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
    bool processedAll(StmtId s, NodeType type);

    /*
        Sets processedAllNext/processedAllPrev of s to TRUE for NodeType SUCCESSOR and PREDECESSOR respectively
    */
    void setProcessedAll(StmtId s, NodeType type);

    /*
        Returns TRUE if a Next relation exists, FALSE otherwise
    */
    bool hasNextRelation();

    /*
        Clears the nextStarTable
    */
    void clear();

 private:
    PKB::PKB* pkb;
    FrontEnd::RuntimeDesignExtractor rtDE;

    std::unordered_map<StmtId, nextRS> nextTable;
    std::unordered_map<StmtId, nextStarRS> nextStarTable;
};
