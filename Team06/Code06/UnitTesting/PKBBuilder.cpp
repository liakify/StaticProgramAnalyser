#include "stdafx.h"
#include "PKBBuilder.h"
#include "Simple.h"

namespace UnitTesting {

    PKBBuilder& PKBBuilder::addVar(VarName var) {
        pkb.varTable.insertVar(var);
        return *this;
    }

    PKBBuilder& PKBBuilder::addPrintStmt(VarName var) {
        std::shared_ptr<SIMPLE::PrintStmt> printStmt(new SIMPLE::PrintStmt(pkb.varTable.getVarId(var)));
        pkb.stmtTable.insertStmt(printStmt);
        return *this;
    }

    PKBBuilder& PKBBuilder::addReadStmt(VarName var) {
        std::shared_ptr<SIMPLE::ReadStmt> readStmt(new SIMPLE::ReadStmt(pkb.varTable.getVarId(var)));
        pkb.stmtTable.insertStmt(readStmt);
        return *this;
    }

    PKBBuilder& PKBBuilder::addAssignStmt(VarName var) {
        // Note: Expression attribute is currently just a dummy value and is irrelevant
        std::shared_ptr<SIMPLE::AssignStmt> assignStmt(new SIMPLE::AssignStmt(pkb.varTable.getVarId(var), 
            SIMPLE::Expression(var, pkb.varTable.getVarId(var), ExprType::VAR)));
        pkb.stmtTable.insertStmt(assignStmt);
        return *this;
    }

    PKBBuilder& PKBBuilder::addIfStmt(VarName var) {
        
        return *this;
    }

    PKBBuilder& PKBBuilder::addWhileStmt(VarName var) {

        return *this;
    }

    PKBBuilder& PKBBuilder::addCallStmt(ProcName proc) {
        std::shared_ptr<SIMPLE::CallStmt> callStmt(new SIMPLE::CallStmt(proc));
        pkb.stmtTable.insertStmt(callStmt);
        return *this;
    }

    PKBBuilder& PKBBuilder::addFollowsRS(StmtId stmt1, StmtId stmt2) {
        pkb.followsKB.addFollows(stmt1, stmt2);
        return *this;
    }

    PKBBuilder& PKBBuilder::addFollowsStarRS(StmtId stmt1, std::unordered_set<StmtId> stmts) {
        pkb.followsKB.setAllFollowers(stmt1, stmts);
        return *this;
    }
    
    PKBBuilder& PKBBuilder::addFollowsStarRS(std::unordered_set<StmtId> stmts, StmtId stmt2) {
        pkb.followsKB.setAllFollowing(stmt2, stmts);
        return *this;
    }

    PKBBuilder& PKBBuilder::addParentRS(StmtId stmt1, StmtId stmt2) {
        pkb.parentKB.addParent(stmt1, stmt2);
        return *this;
    }

    PKBBuilder& PKBBuilder::addParentStarRS(StmtId stmt1, std::unordered_set<StmtId> stmts) {
        pkb.parentKB.setAllChildren(stmt1, stmts);
        return *this;
    }

    PKBBuilder& PKBBuilder::addParentStarRS(std::unordered_set<StmtId> stmts, StmtId stmt2) {
        pkb.parentKB.setAllParents(stmt2, stmts);
        return *this;
    }

    PKBBuilder& PKBBuilder::addStmtModifiesRS(StmtId stmt, VarName var) {
        pkb.modifiesKB.addStmtModifies(stmt, pkb.varTable.getVarId(var));
        return *this;
    }

    PKBBuilder& PKBBuilder::addProcModifiesRS(ProcName proc, VarName var) {
        pkb.modifiesKB.addProcModifies(pkb.procTable.getProcId(proc), pkb.varTable.getVarId(var));
        return *this;
    }

    PKBBuilder& PKBBuilder::addStmtUsesRS(StmtId stmt, VarName var) {
        pkb.usesKB.addStmtUses(stmt, pkb.varTable.getVarId(var));
        return *this;
    }

    PKBBuilder& PKBBuilder::addProcUsesRS(ProcName proc, VarName var) {
        pkb.usesKB.addProcUses(pkb.procTable.getProcId(proc), pkb.varTable.getVarId(var));
        return *this;
    }

    PKB::PKB& PKBBuilder::getPKB() {
        return pkb;
    }

}