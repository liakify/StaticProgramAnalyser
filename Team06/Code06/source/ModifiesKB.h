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
	void addStmtModifies(StmtId stmtId, VarName var);

	bool stmtModifies(StmtId stmtId, VarName var);

	std::unordered_set<VarName> getAllVarsModifiedByStmt(StmtId stmtId);

	std::unordered_set<StmtId> getAllStmtsModifyVar(VarName var);

private:
	static std::unordered_map<StmtId, std::unordered_set<VarName>> stmtVarTable;
	static std::unordered_map<VarName, std::unordered_set<StmtId>> varStmtTable;
};