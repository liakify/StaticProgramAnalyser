#include "UsesKB.h"

/*
	TODO: Exception handling for invalid inputs
*/

UsesKB::UsesKB()
{

}

void UsesKB::addStmtUses(int stmtId, std::string var)
{
	stmtVarTable[stmtId].insert(var);
	varStmtTable[var].insert(stmtId);
}

bool UsesKB::stmtUses(int stmtId, std::string var)
{
	std::unordered_set<std::string> varSet = stmtVarTable[stmtId];
	return varSet.find(var) != varSet.end();
}

std::unordered_set<std::string> UsesKB::getAllVarsUsedByStmt(int stmtId)
{
	return stmtVarTable[stmtId];
}

std::unordered_set<int> UsesKB::getAllStmtsUsingVar(std::string var)
{
	return varStmtTable[var];
}
