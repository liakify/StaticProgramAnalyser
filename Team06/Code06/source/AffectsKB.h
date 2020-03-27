#pragma once

#include <stdexcept>
#include "unordered_map"
#include "unordered_set"

#include "Constants.h"
#include "Types.h"


struct affectsRS {
    std::unordered_set<StmtId> directAffects;
    std::unordered_set<StmtId> directAffectedBy;
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
        Returns TRUE if s2 cis affected by s1, FALSE otherwise
    */
    bool affects(StmtId s1, StmtId s2);

    /*
        Returns a reference to directAffects/directAffected of s for NodeType SUCCESSOR and PREDECESSOR respectively
    */
    const std::unordered_set<StmtId>& getDirectNodes(StmtId s, NodeType type);
    
    /*
        Returns processedAllAffects/processedAllAffectedBy of s for NodeType SUCCESSOR and PREDECESSOR respectively
    */
    bool processedAllAffects(StmtId s, NodeType type);
    
    /*
       Sets processedAllAffects/processedAllAffectedBy of s to TRUE for NodeType SUCCESSOR and PREDECESSOR respectively
    */
    void setProcessedAllAffects(StmtId s, NodeType type);

    /*
        Clears the affectsTable
    */
    void clear();

private:
    std::unordered_map<StmtId, affectsRS> affectsTable;
};