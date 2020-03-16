#include "NextKB.h"
#include <stdexcept>

void NextKB::addNext(StmtId s1, StmtId s2)
{
	nextRS& nRS1 = nextTable[s1];
	nextRS& nRS2 = nextTable[s2];

	nRS1.directNext.insert(s2);
	nRS2.directPrev.insert(s1);
}

bool NextKB::next(StmtId s1, StmtId s2)
{
	try {
		std::unordered_set<ProcId> dirNext = nextTable.at(s1).directNext;
		return dirNext.find(s2) != dirNext.end();
	}
	catch (const std::out_of_range&) {
		return false;
	}
}

const std::unordered_set<StmtId>& NextKB::getDirectNodes(StmtId s, NodeType type)
{
	try {
		if (type == NodeType::SUCCESSOR) {
			return nextTable.at(s).directNext;
		}
		else {
			return nextTable.at(s).directPrev;
		}
	}
	catch (const std::out_of_range&) {
		return EMPTY_RESULT;
	}
}

bool NextKB::hasNext(StmtId s)
{
	try {
		return nextTable.at(s).directNext.size() != 0;
	}
	catch (const std::out_of_range&) {
		return false;
	}
}

bool NextKB::hasPrev(StmtId s)
{
	try {
		return nextTable.at(s).directNext.size() != 0;
	}
	catch (const std::out_of_range&) {
		return false;
	}
}

bool NextKB::hasNextRelation()
{
	return nextTable.size() != 0;
}

void NextKB::clear()
{
	nextStarTable.clear();
}
