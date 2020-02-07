#include "UsesKB.h"

UsesKB::UsesKB(int numProgStmts)
	: numProgStmts(numProgStmts)
{
	stmtUsesTable = std::vector<std::vector<bool>>(numProgStmts, std::vector<bool>(numProgStmts));
	procUsesTable = std::vector<std::vector<bool>>(numProgStmts, std::vector<bool>(numProgStmts));
}

void UsesKB::addStmtUses(int stmtId, int varId)
{
	stmtUsesTable[stmtId][varId] = true;
}

void UsesKB::addProcUses(int procId, int varId)
{
	procUsesTable[procId][varId] = true;
}

bool UsesKB::stmtUses(int stmtId, int varId)
{
	return stmtUsesTable[stmtId][varId];
}

bool UsesKB::procUses(int procId, int varId)
{
	return procUsesTable[procId][varId];
}
