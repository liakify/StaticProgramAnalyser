#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

class PatternKB
{
public:
	void addLHSPattern(std::string pattern, int stmtId);

	void addRHSPattern(std::string pattern, int stmtId);

	std::unordered_set<int> getLHSPatternStmts(std::string pattern);

	std::unordered_set<int> getRHSPatternStmts(std::string pattern);

private:
	std::unordered_map<std::string, std::unordered_set<int>> lhsTable;
	std::unordered_map<std::string, std::unordered_set<int>> rhsTable;
};
