#pragma once

#include <stdexcept>
#include "unordered_map"
#include "unordered_set"

#include "Constants.h"
#include "Types.h"

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
    /*
        Adds Next(s1, s2) to the nextTable where s2 can be executed immediately after s1
    */
    void addNext(StmtId s1, StmtId s2);

    /*
        Adds s2 to the set of statements that CANNOT be executed in some execution sequence after s1 in falseNextStarTable
    */
    void addNotNextStar(StmtId s1, StmtId s2);

    /*
        Returns TRUE if s2 can be executed immediately after s1, FALSE otherwise
    */
    bool next(StmtId s1, StmtId s2);

    /*
        Returns TRUE if s2 can be executed in some execution sequence after s1, FALSE otherwise
    */
    bool nextStar(StmtId s1, StmtId s2);

    /*
        Returns TRUE if s2 CANNOT be executed in some execution sequence after s1, FALSE otherwise
    */
    bool notNextStar(StmtId s1, StmtId s2);

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
        Returns TRUE if s exists as a key in the nextTable, FALSE otherwise
    */
    bool existsInNext(StmtId s);

    /*
        Initialise nextStarTable entry for s with nextTable values
    */
    void initEntry(StmtId s);

    /*
        Clears the nextStarTable
    */
    void clear();

 private:
    std::unordered_map<StmtId, nextRS> nextTable;
    std::unordered_map<StmtId, nextStarRS> nextStarTable;  // Stores TRUE relations
    std::unordered_map<StmtId, std::unordered_set<StmtId>> falseNextStarTable; // Stores FALSE relations
};
