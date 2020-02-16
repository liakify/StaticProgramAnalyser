#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Types.h"

/*
	TODO: Proc Tables/API
*/

class UsesKB
{
public:
	void addStmtUses(StmtId stmtId, VarId var);

	bool stmtUses(StmtId stmtId, VarId var);

	std::unordered_set<VarId> getAllVarsUsedByStmt(StmtId stmtId);

	std::unordered_set<StmtId> getAllStmtsUsingVar(VarId var);

private:
	std::unordered_map<StmtId, std::unordered_set<VarId>> stmtVarTable;
	std::unordered_map<VarId, std::unordered_set<StmtId>> varStmtTable;
};
