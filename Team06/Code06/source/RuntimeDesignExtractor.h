#pragma once

#include <memory>
#include <unordered_set>

#include "Simple.h"
#include "Types.h"

using namespace SIMPLE;

namespace PKB {
    class PKB;
}

namespace FrontEnd {
    class RuntimeDesignExtractor {
     public:
        bool processNextStar(StmtId s1, StmtId s2, PKB::PKB* pkb);

        void processNextStarGetAllNodes(StmtId s, NodeType type, PKB::PKB* pkb);

        bool processAffects(StmtId s1, StmtId s2, PKB::PKB* pkb);

        void processAffectsGetAllNodes(StmtId s, NodeType type, PKB::PKB* pkb);

     private:
        PKB::PKB* pkb;

        void addBiDirEdge(StmtId s1, StmtId s2, NodeType type);
        bool nextStarRecurse(StmtId curr, StmtId last, StmtId s2);
        void nextStarDFS(StmtId root, StmtId curr, std::unordered_set<StmtId>& visited, NodeType type);

        void affectsDFS(StmtId root, VarId modifiedId, StmtId curr, std::unordered_set<StmtId>& visited, std::unordered_set<StmtId>& result, StmtId goal = -1);
        void affectedByDFS(StmtId root, std::unordered_set<VarId> usedId, StmtId curr, std::unordered_set<StmtId>& visited, std::unordered_set<StmtId>& result);
    };
}
