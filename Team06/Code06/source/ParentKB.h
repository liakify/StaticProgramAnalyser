#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Types.h"

struct parentRS
{
	StmtId parent = 0;
	std::unordered_set<StmtId> directChildren;
	std::unordered_set<StmtId> allParents;
	std::unordered_set<StmtId> allChildren;
};

class ParentKB
{
public:
	/*
		Adds Parent(stmtId1, stmtId2) relation to parentTable.
	*/
	void addParent(StmtId stmtId1, StmtId stmtId2);

	/*
		Returns TRUE if Parent(stmtId1, stmtId2) is true, FALSE otherwise.
	*/
	bool parent(StmtId stmtId1, StmtId stmtId2);

	/*
		Returns TRUE if Parent*(stmtId1, stmtId2) is true, FALSE otherwise.
	*/
	bool parentStar(StmtId stmtId1, StmtId stmtId2);

	/*
		Returns statement ID s for which Parent(s, stmtId) is true.
		If stmtId is not found or stmtId has no parent, INVALID_STMT_ID 0 is returned.
	*/
	StmtId getParent(StmtId stmtId);

	/*
		Returns unordered_set of child statement IDs for which Parent(stmtId, s) is true.
		If stmtId is not found or stmtId has direct children, empty set is returned.
	*/
	std::unordered_set<StmtId> getDirectChildren(StmtId stmtId);

	/*
		Returns TRUE if stmtId has a parent, FALSE otherwise.
	*/
	bool hasParent(StmtId stmtId);

	/*
		Returns TRUE if stmtId has direct children, FALSE otherwise.
	*/
	bool hasDirectChildren(StmtId stmtId);

	/*
		Returns all statement IDs s for which Parent*(s, stmtId) is true.
	*/
	std::unordered_set<StmtId> getAllParents(StmtId stmtId);

	/*
		Returns all statement IDs s for which Parent*(stmtId, s) is true.
	*/
	std::unordered_set<StmtId> getAllChildren(StmtId stmtId);

	/*
		Sets allChildren of stmtId to children
	*/
	void setAllChildren(StmtId stmtId, std::unordered_set<StmtId> children);

	/*
		Sets allParents of stmtId to parents
	*/
	void setAllParents(StmtId stmtId, std::unordered_set<StmtId> parents);

private:
	std::unordered_map<StmtId, parentRS> parentTable;
};
