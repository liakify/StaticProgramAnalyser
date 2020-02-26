#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Types.h"

class UsesKB
{
public:
	void addStmtUses(StmtId stmtId, VarId varId);

	bool stmtUses(StmtId stmtId, VarId varId);

	void addProcUses(ProcId procId, VarId varId);

	bool procUses(ProcId procId, VarId varId);

	std::unordered_set<VarId> getAllVarsUsedByStmt(StmtId stmtId);

	std::unordered_set<StmtId> getAllStmtsUsingVar(VarId varId);

	std::unordered_set<VarId> getAllVarsUsedByProc(ProcId procId);

	std::unordered_set<ProcId> getAllProcUsingVar(VarId varId);

private:
	std::unordered_map<StmtId, std::unordered_set<VarId>> stmtVarTable;
	std::unordered_map<VarId, std::unordered_set<StmtId>> varStmtTable;
	std::unordered_map<ProcId, std::unordered_set<VarId>> procVarTable;
	std::unordered_map<VarId, std::unordered_set<ProcId>> varProcTable;
};
