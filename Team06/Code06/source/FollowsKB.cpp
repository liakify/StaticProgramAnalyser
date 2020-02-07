#include "FollowsKB.h"

FollowsKB::FollowsKB(int numProgStmts)
	: numProgStmts(numProgStmts)
{
	followsTable = std::vector<std::vector<bool>>(numProgStmts, std::vector<bool>(numProgStmts));
	followStarTable = std::vector<std::vector<bool>>(numProgStmts, std::vector<bool>(numProgStmts));
}

void FollowsKB::addFollows(int stmtId1, int stmtId2)
{
	followsTable[stmtId1][stmtId2] = true;
}

void FollowsKB::addFollowStar(int stmtId1, int stmtId2)
{
	followStarTable[stmtId1][stmtId2] = true;
}

bool FollowsKB::follows(int stmtId1, int stmtId2)
{
	return followsTable[stmtId1][stmtId2];
}

bool FollowsKB::followStar(int stmtId1, int stmtId2)
{
	return followStarTable[stmtId1][stmtId2];
}
