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
	std::unordered_set<ProcId> getRoots();

	/*
		Returns all ProcId with no callees
	*/
	std::unordered_set<ProcId> getLeaves();

	/*
		Returns a reference to directCallees/directCallers of p for NodeType SUCCESSOR and PREDECESSOR respectively
	*/
	std::unordered_set<ProcId>& getDirectNodes(ProcId p, NodeType type);

	/*
		Returns a reference to allCallees/allCallers of p for NodeType SUCCESSOR and PREDECESSOR respectively
	*/
	std::unordered_set<ProcId>& getAllNodes(ProcId p, NodeType type);

	/*
		Adds proc to allCallees/allCallers of p for NodeType SUCCESSOR and PREDECESSOR respectively
	*/
	void addToAll(ProcId p, ProcId proc, NodeType type);

	/*
		Adds procs to allCallees/allCallers of p for NodeType SUCCESSOR and PREDECESSOR respectively
	*/
	void addToAll(ProcId p, std::unordered_set<ProcId>& procs, NodeType type);

private:
	std::unordered_map<ProcId, callsRS> callsTable;
};

