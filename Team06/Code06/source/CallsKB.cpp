#include "CallsKB.h"
#include <stdexcept>

CallsKB::CallsKB()
	: callsTable{ {-1, callsRS()} } {

}

void CallsKB::addCalls(ProcId p1, ProcId p2) {
	callsRS& cRS1 = callsTable[p1];
	callsRS& cRS2 = callsTable[p2];

	cRS1.directCallees.insert(p2);
	cRS2.directCallers.insert(p1);
}

bool CallsKB::calls(ProcId p1, ProcId p2) {
	try {
		std::unordered_set<ProcId> callees = callsTable.at(p1).directCallees;
		return callees.find(p2) != callees.end();
	}
	catch (const std::out_of_range&) {
		return false;
	}
}

bool CallsKB::callStar(ProcId p1, ProcId p2) {
	try {
		std::unordered_set<ProcId> callees = callsTable.at(p1).allCallees;
		return callees.find(p2) != callees.end();
	}
	catch (const std::out_of_range&) {
		return false;
	}
}

std::unordered_set<ProcId>& CallsKB::getDirectNodes(ProcId p, NodeType type)
{
	try {
		if (type == NodeType::SUCCESSOR) {
			return callsTable.at(p).directCallees;
		}
		else {
			return callsTable.at(p).directCallers;
		}
	}
	catch (const std::out_of_range&) {
		return callsTable[-1].directCallees;
	}
}

std::unordered_set<ProcId>& CallsKB::getAllNodes(ProcId p, NodeType type)
{
	try {
		if (type == NodeType::SUCCESSOR) {
			return callsTable.at(p).allCallees;
		}
		else {
			return callsTable.at(p).allCallers;
		}
	}
	catch (const std::out_of_range&) {
		return callsTable[-1].allCallers;
	}
}

void CallsKB::addToAll(ProcId p, ProcId proc, NodeType type) {
	if (type == NodeType::SUCCESSOR) {
		callsTable.at(p).allCallees.insert(proc);
	}
	else {
		callsTable.at(p).allCallers.insert(proc);
	}
}

void CallsKB::addToAll(ProcId p, std::unordered_set<ProcId>& procs, NodeType type) {
	if (type == NodeType::SUCCESSOR) {
		callsTable.at(p).allCallees.insert(procs.begin(), procs.end());
	}
	else {
		callsTable.at(p).allCallers.insert(procs.begin(), procs.end());
	}
}
