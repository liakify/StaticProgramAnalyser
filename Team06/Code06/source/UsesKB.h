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
	void addStmtUses(StmtId stmtId, VarName var);

	bool stmtUses(StmtId stmtId, VarName var);

	std::unordered_set<VarName> getAllVarsUsedByStmt(StmtId stmtId);

	std::unordered_set<StmtId> getAllStmtsUsingVar(VarName var);

private:
	static std::unordered_map<StmtId, std::unordered_set<VarName>> stmtVarTable;
	static std::unordered_map<VarName, std::unordered_set<StmtId>> varStmtTable;
};
