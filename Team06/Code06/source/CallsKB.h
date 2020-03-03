#pragma once

#include "unordered_set"
#include "unordered_map"

#include "Types.h"

struct callsRS {
	std::unordered_set<ProcId> directCallers;
	std::unordered_set<ProcId> directCallees;
	std::unordered_set<ProcId> allCallers;
	std::unordered_set<ProcId> allCallees;
};

class CallsKB {
public:
	CallsKB();

	/*
	*	Adds Calls(p1, p2) to the callsTable where p1 directly calls p2
	*/
	void addCalls(ProcId p1, ProcId p2);

	/*
	*	Returns TRUE if p1 directly calls p2, FALSE otherwise
	*/
	bool calls(ProcId p1, ProcId p2);

	/*
	*	Returns TRUE if p1 directly or indirectly calls p2, FALSE otherwise
	*/
	bool callStar(ProcId p1, ProcId p2);

	/*
		Returns all ProcId with no callers
	*/
	std::unordered_set<ProcId>& getRoots();

	/*
		Returns all ProcId with no callees
	*/
	std::unordered_set<ProcId>& getLeaves();

	/*
		Returns all direct callees of p
	*/
	std::unordered_set<ProcId>& getDirectCallees(ProcId p);

	/*
		Returns all direct callers of p
	*/
	std::unordered_set<ProcId>& getDirectCallers(ProcId p);

	/*
		Returns allCallees of p1
	*/
	std::unordered_set<ProcId>& getAllCallees(ProcId p);

	/*
		Returns allCallees of p1
	*/
	std::unordered_set<ProcId>& getAllCallers(ProcId p);

	/*
		Adds callees to allCallees of p
	*/
	void setAllCallees(ProcId p, std::unordered_set<ProcId>& callees);

	/*
		Adds callees to allCallees of p
	*/
	void setAllCallers(ProcId p, std::unordered_set<ProcId>& callers);

private:
	std::unordered_map<ProcId, callsRS> callsTable;
};

