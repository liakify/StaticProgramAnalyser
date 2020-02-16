#include "UsesKB.h"

/*
	TODO: Exception handling for invalid inputs
*/

void UsesKB::addStmtUses(StmtId stmtId, VarId var)
{
	stmtVarTable[stmtId].insert(var);
	varStmtTable[var].insert(stmtId);
}

bool UsesKB::stmtUses(StmtId stmtId, VarId var)
{
	std::unordered_set<VarId> varSet = stmtVarTable[stmtId];
	return varSet.find(var) != varSet.end();
}

std::unordered_set<VarId> UsesKB::getAllVarsUsedByStmt(StmtId stmtId)
{
	return stmtVarTable[stmtId];
}

std::unordered_set<StmtId> UsesKB::getAllStmtsUsingVar(VarId var)
{
	return varStmtTable[var];
}
