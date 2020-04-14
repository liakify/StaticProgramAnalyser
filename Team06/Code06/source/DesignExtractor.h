#pragma once

#include <unordered_set>
#include <vector>

#include "Types.h"
#include "PKB.h"
#include "Simple.h"

using namespace SIMPLE;

namespace FrontEnd {
    /**
    * The DesignExtractor class is used for extracting secondary relationships
    * from the the parsed SIMPLE code, and populates the PKB accordingly.
    */
    class DesignExtractor {
     public:
        /**
        * Updates the PKB with all precomputable relationships.
        *
        * @param    pkb         Reference to PKB to be updated.
        * @return   PKB::PKB    The updated PKB instance.
        */
        PKB::PKB run(PKB::PKB& pkb);

     private:
        PKB::PKB pkb;

        /**
        * Functions to be called in run()
        * Note that populateUses/populateModifies must be called after
        * populateCalls/populateCallStar as the former is unable to detect
        * cyclic calls. 
        */
        void populateCalls();
        void populateCallStar();
        void populateFollows();
        void populateFollowStar();
        void populateParent();
        void populateParentStar();
        void populateUses();
        void populateModifies();
        void populatePattern();
        void populateNext();
        void updateStmtContainerId();
        void initAffectsKB();

        // Helper functions
        void populateParentKB(StmtId stmtId, StmtListId stmtLstId);

        void populateStmtUses(StmtId id);
        std::unordered_set<VarId> getAllUses(StmtListId sid);
        void populateStmtUsesKB(StmtId stmtId, std::unordered_set<VarId>& varSet);
        void populateProcUsesKB(ProcId procId, std::unordered_set<VarId>& varSet);

        void populateStmtModifies(StmtId id);
        std::unordered_set<VarId> getAllModifies(StmtListId sid);
        void populateStmtModifiesKB(StmtId stmtId, std::unordered_set<VarId>& varSet);
        void populateProcModifiesKB(ProcId procId, std::unordered_set<VarId>& varSet);

        void populateAssignPatternKB(StmtId stmtId, Expression exp);
        void populateIfPatternKB(StmtId stmtId, CondExpr cond);
        void populateWhilePatternKB(StmtId stmtId, CondExpr cond);

        std::unordered_set<ProcId> getAllCalls(StmtListId sid);
        void processCallStar(int numProc, std::vector<ProcId>& visited, NodeType type);
        void callStarDFS(ProcId root, std::vector<ProcId>& visited, NodeType type);

        void populateNextKB(StmtListId sid);
        void updateLastStmtId(StatementList& sl);
    };
}