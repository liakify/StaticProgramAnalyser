#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

/*
	TODO: Proc Tables/API
*/

class ModifiesKB
{
public:
	ModifiesKB();

	void addStmtModifies(int stmtId, std::string var);

	bool stmtModifies(int stmtId, std::string var);

	std::unordered_set<std::string> getAllVarsModifiedByStmt(int stmtId);

	std::unordered_set<int> getAllStmtsModifyVar(std::string var);

private:
	static std::unordered_map<int, std::unordered_set<std::string>> stmtVarTable;
	static std::unordered_map<std::string, std::unordered_set<int>> varStmtTable;
};