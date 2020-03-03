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

std::unordered_set<ProcId>& CallsKB::getRoots()
{
	std::unordered_set<ProcId> roots;

	for (auto it = callsTable.begin(); it != callsTable.end(); ++it) {
		callsRS callsStruct = it->second;
		if (callsStruct.directCallers.size() == 0) {
			roots.insert(it->first);
		}
	}

	return roots;
}

std::unordered_set<ProcId>& CallsKB::getLeaves()
{
	std::unordered_set<ProcId> leaves;

	for (auto it = callsTable.begin(); it != callsTable.end(); ++it) {
		callsRS callsStruct = it->second;
		if (callsStruct.directCallees.size() == 0) {
			leaves.insert(it->first);
		}
	}

	return leaves;
}

std::unordered_set<ProcId>& CallsKB::getDirectCallees(ProcId p)
{
	try {
		return callsTable.at(p).directCallees;
	}
	catch (const std::out_of_range&) {
		return callsTable[-1].directCallees;
	}
}

std::unordered_set<ProcId>& CallsKB::getDirectCallers(ProcId p)
{
	try {
		return callsTable.at(p).directCallers;
	}
	catch (const std::out_of_range&) {
		return callsTable[-1].directCallers;
	}
}

std::unordered_set<ProcId>& CallsKB::getAllCallees(ProcId p)
{
	try {
		return callsTable.at(p).allCallees;
	}
	catch (const std::out_of_range&) {
		return callsTable[-1].allCallees;
	}
}

std::unordered_set<ProcId>& CallsKB::getAllCallers(ProcId p)
{
	try {
		return callsTable.at(p).allCallers;
	}
	catch (const std::out_of_range&) {
		return callsTable[-1].allCallers;
	}
}

void CallsKB::setAllCallees(ProcId p, std::unordered_set<ProcId>& callees) {
	for (const auto& callee : callees) {
		callsTable.at(p).allCallees.insert(callee);
	}
}

void CallsKB::setAllCallers(ProcId p, std::unordered_set<ProcId>& callers) {
	for (const auto& caller : callers) {
		callsTable.at(p).allCallers.insert(caller);
	}
}
