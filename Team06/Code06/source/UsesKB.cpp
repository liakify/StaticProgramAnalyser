#include "UsesKB.h"
#include <stdexcept>

void UsesKB::addStmtUses(StmtId stmtId, VarId varId) {
    stmtVarTable[stmtId].insert(varId);
    varStmtTable[varId].insert(stmtId);
}

bool UsesKB::stmtUses(StmtId stmtId, VarId varId) {
    try {
        std::unordered_set<VarId> varSet = stmtVarTable.at(stmtId);
        return varSet.find(varId) != varSet.end();
    }
    catch (const std::out_of_range &) {
        return false;
    }
}

void UsesKB::addProcUses(ProcId procId, VarId varId) {
    procVarTable[procId].insert(varId);
    varProcTable[varId].insert(procId);
}

bool UsesKB::procUses(ProcId procId, VarId varId) {
    try {
        std::unordered_set<VarId> varSet = procVarTable.at(procId);
        return varSet.find(varId) != varSet.end();
    }
    catch (const std::out_of_range&) {
        return false;
    }
}

const std::unordered_set<VarId>& UsesKB::getAllVarsUsedByStmt(StmtId stmtId) {
    try {
        return stmtVarTable.at(stmtId);
    }
    catch (const std::out_of_range &) {
        return EMPTY_RESULT;
    }
}

const std::unordered_set<StmtId>& UsesKB::getAllStmtsUsingVar(VarId varId) {
    try {
        return varStmtTable.at(varId);
    }
    catch (const std::out_of_range &) {
        return EMPTY_RESULT;
    }
}

const std::unordered_set<VarId>& UsesKB::getAllVarsUsedByProc(ProcId procId) {
    try {
        return procVarTable.at(procId);
    }
    catch (const std::out_of_range&) {
        return EMPTY_RESULT;
    }
}

const std::unordered_set<ProcId>& UsesKB::getAllProcUsingVar(VarId varId) {
    try {
        return varProcTable.at(varId);
    }
    catch (const std::out_of_range&) {
        return EMPTY_RESULT;
    }
}
