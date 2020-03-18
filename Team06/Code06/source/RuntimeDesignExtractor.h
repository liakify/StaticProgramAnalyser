#pragma once

#include "Simple.h"

using namespace SIMPLE;

namespace PKB {
    class PKB;
}

namespace FrontEnd {
    class RuntimeDesignExtractor {
     public:
        RuntimeDesignExtractor(PKB::PKB* pkb);

        bool nextStar(StmtId s1, StmtId s2);

        void processStmtAllNodes(StmtId s, NodeType type);

     private:
        PKB::PKB* pkb;

        void addBiDirEdge(StmtId s1, StmtId s2);
        bool nextStarRecurse(StmtId curr, StmtId last, StmtId s2);
        void nextStarDFS(StmtId root, StmtId curr, std::unordered_set<StmtId>& visited, NodeType type);
    };
}
