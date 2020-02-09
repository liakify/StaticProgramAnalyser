#pragma once

#include <vector>

class FollowsKB 
{
public:
	FollowsKB(int numProgStmts);

	/*
		Adds Follows(stmtId1, stmtId2) relation to followsTable.
	*/
	void addFollows(int stmtId1, int stmtId2);

	/*
		Adds Follows*(stmtId1, stmtId2) relation to followStarTable.
	*/
	void addFollowStar(int stmtId1, int stmtId2);

	/*
		Returns TRUE if Follows(stmtId1, stmtId2) is true, FALSE otherwise.
	*/
	bool follows(int stmtId1, int stmtId2);

	/*
		Returns TRUE if Follows*(stmtId1, stmtId2) is true, FALSE otherwise.
	*/
	bool followStar(int stmtId1, int stmtId2);

private:
	const int numProgStmts;
	static std::vector<std::vector<bool>> followsTable;
	static std::vector<std::vector<bool>> followStarTable;
};
