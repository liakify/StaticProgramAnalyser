#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Types.h"

/*
	TODO: Proc Tables/API
*/

class ModifiesKB
{
public:
	void addStmtModifies(StmtId stmtId, VarId var);

	bool stmtModifies(StmtId stmtId, VarId var);

	std::unordered_set<VarId> getAllVarsModifiedByStmt(StmtId stmtId);

	std::unordered_set<StmtId> getAllStmtsModifyVar(VarId var);

private:
	std::unordered_map<StmtId, std::unordered_set<VarId>> stmtVarTable;
	std::unordered_map<VarId, std::unordered_set<StmtId>> varStmtTable;
};