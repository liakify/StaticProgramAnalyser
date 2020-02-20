#include "PatternKB.h"
#include <stdexcept>

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
	try {
		return lhsTable.at(pattern);
	}
	catch (const std::out_of_range & oor) {
		return {};
	}
}

std::unordered_set<StmtId> PatternKB::getRHSPatternStmts(Pattern pattern)
{
	try {
		return rhsTable.at(pattern);
	}
	catch (const std::out_of_range & oor) {
		return {};
	}
}
