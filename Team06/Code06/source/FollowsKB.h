#pragma once

#include <vector>

class FollowsKB 
{
public:
	FollowsKB(int numProgStmts);

	void addFollows(int stmtId1, int stmtId2);

	void addFollowStar(int stmtId1, int stmtId2);

	bool follows(int stmtId1, int stmtId2);

	bool followStar(int stmtId1, int stmtId2);

private:
	const int numProgStmts;
	static std::vector<std::vector<bool>> followsTable;
	static std::vector<std::vector<bool>> followStarTable;
};
