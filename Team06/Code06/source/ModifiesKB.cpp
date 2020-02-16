#include "ModifiesKB.h"

/*
	TODO: Exception handling for invalid inputs
*/

void ModifiesKB::addStmtModifies(StmtId stmtId, VarId var)
{
	stmtVarTable[stmtId].insert(var);
	varStmtTable[var].insert(stmtId);
}

bool ModifiesKB::stmtModifies(StmtId stmtId, VarId var)
{
	std::unordered_set<VarId> varSet = stmtVarTable[stmtId];
	return varSet.find(var) != varSet.end();
}

std::unordered_set<VarId> ModifiesKB::getAllVarsModifiedByStmt(StmtId stmtId)
{
	return stmtVarTable[stmtId];
}

std::unordered_set<StmtId> ModifiesKB::getAllStmtsModifyVar(VarId var)
{
	return varStmtTable[var];
}
