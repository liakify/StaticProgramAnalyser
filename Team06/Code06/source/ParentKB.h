#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

struct parentRS
{
	int parent = 0;
	int child = 0;
	std::unordered_set<int> allParents;
	std::unordered_set<int> allChildren;
};

class ParentKB
{
public:
	ParentKB();

	/*
		Adds Parent(stmtId1, stmtId2) relation to parentTable.
		Also adds Parent*(s, stmtId2) for all s in s1.allParents.
	*/
	void addParent(int stmtId1, int stmtId2);

	/*
		Returns TRUE if Parent(stmtId1, stmtId2) is true, FALSE otherwise.
	*/
	bool parent(int stmtId1, int stmtId2);

	/*
		Returns TRUE if Parent*(stmtId1, stmtId2) is true, FALSE otherwise.
	*/
	bool parentStar(int stmtId1, int stmtId2);

	/*
		Returns statement ID s for which Parent(s, stmtId) is true.
	*/
	int getParent(int stmtId);

	/*
		Returns statement ID s for which Parent(stmtId, s) is true.
	*/
	int getChild(int stmtId);

	/*
		Returns all statement IDs s for which Parent*(s, stmtId) is true.
	*/
	std::unordered_set<int> getAllParents(int stmtId);

	/*
		Returns all statement IDs s for which Parent*(stmtId, s) is true.
	*/
	std::unordered_set<int> getAllChildren(int stmtId);

private:
	static std::unordered_map<int, parentRS> parentTable;
};
