#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Types.h"
#include "Constants.h"

class ModifiesKB {
 public:
    void addStmtModifies(StmtId stmtId, VarId varId);

    bool stmtModifies(StmtId stmtId, VarId varId);

    void addProcModifies(ProcId procId, VarId varId);

    bool procModifies(ProcId procId, VarId varId);

    const std::unordered_set<VarId>& getAllVarsModifiedByStmt(StmtId stmtId);

    const std::unordered_set<StmtId>& getAllStmtsModifyVar(VarId varId);

    const std::unordered_set<VarId>& getAllVarsModifiedByProc(ProcId procId);

    const std::unordered_set<ProcId>& getAllProcModifyVar(VarId varId);

 private:
    std::unordered_map<StmtId, std::unordered_set<VarId>> stmtVarTable;
    std::unordered_map<VarId, std::unordered_set<StmtId>> varStmtTable;
    std::unordered_map<ProcId, std::unordered_set<VarId>> procVarTable;
    std::unordered_map<VarId, std::unordered_set<ProcId>> varProcTable;
};