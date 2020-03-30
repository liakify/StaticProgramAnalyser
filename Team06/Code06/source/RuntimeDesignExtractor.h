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

        void processAffectsGetDirectNodes(StmtId s, NodeType type, PKB::PKB* pkb);

        bool processAffectsStar(StmtId s1, StmtId s2, PKB::PKB* pkb);

        void processAffectsStarGetAllNodes(StmtId s, NodeType type, PKB::PKB* pkb);

     private:
        PKB::PKB* pkb;

        void addBiDirEdge(StmtId s1, StmtId s2, NodeType type);
        bool nextStarRecurse(StmtId curr, StmtId last, StmtId s2);
        void nextStarDFS(StmtId root, StmtId curr, std::unordered_set<StmtId>& visited, NodeType type);

        bool affectsDFS(StmtId root, VarId modifiedId, StmtId curr, std::unordered_set<StmtId>& visited, StmtId goal = -1);
        void affectedByDFS(StmtId root, std::unordered_set<VarId>& usedIds, StmtId curr, std::unordered_set<StmtId>& visited);

        bool affectsStarDFS(StmtId root, StmtId curr, std::unordered_set<StmtId>& visited, NodeType type, StmtId goal = -1);
        void populateAllAffects();
    };
}
