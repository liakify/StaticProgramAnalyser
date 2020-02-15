#include "UsesKB.h"

/*
	TODO: Exception handling for invalid inputs
*/

void UsesKB::addStmtUses(StmtId stmtId, VarName var)
{
	stmtVarTable[stmtId].insert(var);
	varStmtTable[var].insert(stmtId);
}

bool UsesKB::stmtUses(StmtId stmtId, VarName var)
{
	std::unordered_set<VarName> varSet = stmtVarTable[stmtId];
	return varSet.find(var) != varSet.end();
}

std::unordered_set<VarName> UsesKB::getAllVarsUsedByStmt(StmtId stmtId)
{
	return stmtVarTable[stmtId];
}

std::unordered_set<StmtId> UsesKB::getAllStmtsUsingVar(VarName var)
{
	return varStmtTable[var];
}
