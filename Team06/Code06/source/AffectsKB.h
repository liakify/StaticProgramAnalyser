#pragma once

#include <stdexcept>
#include "unordered_map"
#include "unordered_set"

#include "Constants.h"
#include "Types.h"


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
        Sets affectsFullyComputed to TRUE
    */
    void setAffectsFullyComputed();

    /*
        Clears the affectsTable
    */
    void clear();

 private:
    bool affectsFullyComputed = false;
    std::unordered_map<StmtId, affectsRS> affectsTable;

    std::unordered_map<StmtId, std::unordered_set<StmtId>> falseAffectsTable;
    std::unordered_map<StmtId, std::unordered_set<StmtId>> falseAffectsStarTable;
};