#pragma once

#include "unordered_set"
#include "unordered_map"

#include "Types.h"
#include "Constants.h"

struct nextRS {
	std::unordered_set<StmtId> directPrev;
	std::unordered_set<StmtId> directNext;

};

struct nextStarRS {
	std::unordered_set<StmtId> allPrev;
	std::unordered_set<StmtId> allNext;
};

class NextKB
{
public:
	/*
		Adds Next(s1, s2) to the nextTable where s2 can be executed immediately after s1
	*/
	void addNext(StmtId s1, StmtId s2);

	/*
		Returns TRUE if s2 can be executed immediately after s1, FALSE otherwise
	*/
	bool next(StmtId s1, StmtId s2);

	/*
		Returns a reference to directNext/directPrev of s for NodeType SUCCESSOR and PREDECESSOR respectively
	*/
	const std::unordered_set<StmtId>& getDirectNodes(StmtId s, NodeType type);

	/*
		Returns TRUE if directNext of s is non-empty, FALSE otherwise
	*/
	bool hasNext(StmtId s);

	/*
		Returns TRUE if directPrev of s is non-empty, FALSE otherwise
	*/
	bool hasPrev(StmtId s);

	/*
		Returns TRUE if a Next relation exists, FALSE otherwise
	*/
	bool hasNextRelation();

	/*
		Clears the nextStarTable
	*/
	void clear();

private:
	std::unordered_map<StmtId, nextRS> nextTable;
	std::unordered_map<StmtId, nextStarRS> nextStarTable;
};
