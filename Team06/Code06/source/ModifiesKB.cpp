#include "ModifiesKB.h"

/*
	TODO: Exception handling for invalid inputs
*/

ModifiesKB::ModifiesKB()
{

}

void ModifiesKB::addStmtModifies(int stmtId, std::string var)
{
	stmtVarTable[stmtId].insert(var);
	varStmtTable[var].insert(stmtId);
}

bool ModifiesKB::stmtModifies(int stmtId, std::string var)
{
	std::unordered_set<std::string> varSet = stmtVarTable[stmtId];
	return varSet.find(var) != varSet.end();
}

std::unordered_set<std::string> ModifiesKB::getAllVarsModifiedByStmt(int stmtId)
{
	return stmtVarTable[stmtId];
}

std::unordered_set<int> ModifiesKB::getAllStmtsModifyVar(std::string var)
{
	return varStmtTable[var];
}
