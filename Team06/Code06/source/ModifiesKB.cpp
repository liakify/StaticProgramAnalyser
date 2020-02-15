#include "ModifiesKB.h"

/*
	TODO: Exception handling for invalid inputs
*/

void ModifiesKB::addStmtModifies(StmtId stmtId, VarName var)
{
	stmtVarTable[stmtId].insert(var);
	varStmtTable[var].insert(stmtId);
}

bool ModifiesKB::stmtModifies(StmtId stmtId, VarName var)
{
	std::unordered_set<VarName> varSet = stmtVarTable[stmtId];
	return varSet.find(var) != varSet.end();
}

std::unordered_set<VarName> ModifiesKB::getAllVarsModifiedByStmt(StmtId stmtId)
{
	return stmtVarTable[stmtId];
}

std::unordered_set<StmtId> ModifiesKB::getAllStmtsModifyVar(VarName var)
{
	return varStmtTable[var];
}
