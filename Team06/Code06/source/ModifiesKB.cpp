#include "ModifiesKB.h"

ModifiesKB::ModifiesKB(int numProgStmts)
	: numProgStmts(numProgStmts)
{
	stmtModifiesTable = std::vector<std::vector<bool>>(numProgStmts, std::vector<bool>(numProgStmts));
	procModifiesTable = std::vector<std::vector<bool>>(numProgStmts, std::vector<bool>(numProgStmts));
}

void ModifiesKB::addStmtModifies(int stmtId, int varId)
{
	stmtModifiesTable[stmtId][varId] = true;
}

void ModifiesKB::addProcModifies(int procId, int varId)
{
	procModifiesTable[procId][varId] = true;
}

bool ModifiesKB::stmtModifies(int stmtId, int varId)
{
	return stmtModifiesTable[stmtId][varId];
}

bool ModifiesKB::procModifies(int procId, int varId)
{
	return procModifiesTable[procId][varId];
}
