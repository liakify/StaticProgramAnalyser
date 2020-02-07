#pragma once

#include <vector>

class UsesKB
{
public:
	UsesKB(int numProgStmts);

	void addStmtUses(int stmtId, int varId);

	void addProcUses(int procId, int varId);

	bool stmtUses(int stmtId, int varId);

	bool procUses(int procId, int varId);

private:
	const int numProgStmts;
	static std::vector<std::vector<bool>> stmtUsesTable;
	static std::vector<std::vector<bool>> procUsesTable;
};
