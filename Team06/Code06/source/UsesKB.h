#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

/*
	TODO: Proc Tables/API
*/

class UsesKB
{
public:
	UsesKB();

	void addStmtUses(int stmtId, std::string var);

	bool stmtUses(int stmtId, std::string var);

	std::unordered_set<std::string> getAllVarsUsedByStmt(int stmtId);

	std::unordered_set<int> getAllStmtsUsingVar(std::string var);

private:
	static std::unordered_map<int, std::unordered_set<std::string>> stmtVarTable;
	static std::unordered_map<std::string, std::unordered_set<int>> varStmtTable;
};
