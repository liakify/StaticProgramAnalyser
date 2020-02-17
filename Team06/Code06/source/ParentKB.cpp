#include "ParentKB.h"

/*
	TODO: Exception handling for invalid inputs
*/

/*
	TODO: Move add Follows* logic out to Design Extractor
*/
void ParentKB::addParent(StmtId stmtId1, StmtId stmtId2)
{
	if (parentTable.find(stmtId1) == parentTable.end())
	{
		parentTable.insert(std::make_pair(stmtId1, parentRS()));
	}

	if (parentTable.find(stmtId2) == parentTable.end())
	{
		parentTable.insert(std::make_pair(stmtId2, parentRS()));
	}

	parentRS pRS1 = parentTable[stmtId1];
	parentRS pRS2 = parentTable[stmtId2];

	pRS1.directChildren.insert(stmtId2);
	pRS2.parent = stmtId1;
}

bool ParentKB::parent(StmtId stmtId1, StmtId stmtId2)
{
	std::unordered_set<StmtId> stmtId1Children = parentTable[stmtId1].directChildren;
	return stmtId1Children.find(stmtId2) != stmtId1Children.end();
}

bool ParentKB::parentStar(StmtId stmtId1, StmtId stmtId2)
{
	std::unordered_set<StmtId> stmtId1AllChildren = parentTable[stmtId1].allChildren;
	return stmtId1AllChildren.find(stmtId2) != stmtId1AllChildren.end();
}

StmtId ParentKB::getParent(StmtId stmtId)
{
	return parentTable[stmtId].parent;
}

std::unordered_set<StmtId> ParentKB::getDirectChildren(StmtId stmtId)
{
	return parentTable[stmtId].directChildren;
}

bool ParentKB::hasParent(StmtId stmtId)
{
	return parentTable[stmtId].parent != 0;
}

bool ParentKB::hasDirectChildren(StmtId stmtId)
{
	return parentTable[stmtId].directChildren.size() != 0;
}

std::unordered_set<StmtId> ParentKB::getAllParents(StmtId stmtId)
{
	return parentTable[stmtId].allParents;
}

std::unordered_set<StmtId> ParentKB::getAllChildren(StmtId stmtId)
{
	return parentTable[stmtId].allChildren;
}

void ParentKB::setAllChildren(StmtId stmtId, std::unordered_set<StmtId> children)
{
	parentTable[stmtId].allChildren = children;
}

void ParentKB::setAllParents(StmtId stmtId, std::unordered_set<StmtId> parents)
{
	parentTable[stmtId].allParents = parents;
}
