#pragma once

#include <vector>

class ModifiesKB
{
public:
	ModifiesKB(int numProgStmts);

	void addStmtModifies(int stmtId, int varId);

	void addProcModifies(int procId, int varId);

	bool stmtModifies(int stmtId, int varId);

	bool procModifies(int procId, int varId);

private:
	const int numProgStmts;
	static std::vector<std::vector<bool>> stmtModifiesTable;
	static std::vector<std::vector<bool>> procModifiesTable;
};
