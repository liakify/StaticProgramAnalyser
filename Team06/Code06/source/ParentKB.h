#pragma once

#include <vector>

class ParentKB
{
public:
	ParentKB(int numProgStmts);

	void addParent(int stmtId1, int stmtId2);

	void addParentStar(int stmtId1, int stmtId2);

	bool parent(int stmtId1, int stmtId2);

	bool parentStar(int stmtId1, int stmtId2);

private:
	const int numProgStmts;
	static std::vector<std::vector<bool>> parentTable;
	static std::vector<std::vector<bool>> parentStarTable;
};
