#include "ParentKB.h"

ParentKB::ParentKB(int numProgStmts)
	: numProgStmts(numProgStmts)
{
	parentTable = std::vector<std::vector<bool>>(numProgStmts, std::vector<bool>(numProgStmts));
	parentStarTable = std::vector<std::vector<bool>>(numProgStmts, std::vector<bool>(numProgStmts));
}

void ParentKB::addParent(int stmtId1, int stmtId2)
{
	parentTable[stmtId1][stmtId2] = true;
}

void ParentKB::addParentStar(int stmtId1, int stmtId2)
{
	parentStarTable[stmtId1][stmtId2] = true;
}

bool ParentKB::parent(int stmtId1, int stmtId2)
{
	return parentTable[stmtId1][stmtId2];
}

bool ParentKB::parentStar(int stmtId1, int stmtId2)
{
	return parentStarTable[stmtId1][stmtId2];
}
