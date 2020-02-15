#include "PatternKB.h"

void PatternKB::addLHSPattern(Pattern pattern, StmtId stmtId)
{
	lhsTable[pattern].insert(stmtId);
}

void PatternKB::addRHSPattern(Pattern pattern, StmtId stmtId)
{
	rhsTable[pattern].insert(stmtId);
}

std::unordered_set<StmtId> PatternKB::getLHSPatternStmts(Pattern pattern)
{
	return lhsTable[pattern];
}

std::unordered_set<StmtId> PatternKB::getRHSPatternStmts(Pattern pattern)
{
	return rhsTable[pattern];
}
