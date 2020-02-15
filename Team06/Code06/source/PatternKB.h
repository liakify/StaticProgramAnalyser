#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Types.h"

class PatternKB
{
public:
	void addLHSPattern(Pattern pattern, StmtId stmtId);

	void addRHSPattern(Pattern pattern, StmtId stmtId);

	std::unordered_set<StmtId> getLHSPatternStmts(Pattern pattern);

	std::unordered_set<StmtId> getRHSPatternStmts(Pattern pattern);

private:
	std::unordered_map<Pattern, std::unordered_set<StmtId>> lhsTable;
	std::unordered_map<Pattern, std::unordered_set<StmtId>> rhsTable;
};
