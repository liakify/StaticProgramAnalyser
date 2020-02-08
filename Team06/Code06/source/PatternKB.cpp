#include "PatternKB.h"

void PatternKB::addLHSPattern(std::string pattern, int stmtId)
{
	lhsTable[pattern].insert(stmtId);
}

void PatternKB::addRHSPattern(std::string pattern, int stmtId)
{
	rhsTable[pattern].insert(stmtId);
}

std::unordered_set<int> PatternKB::getLHSPatternStmts(std::string pattern)
{
	return lhsTable[pattern];
}

std::unordered_set<int> PatternKB::getRHSPatternStmts(std::string pattern)
{
	return rhsTable[pattern];
}
