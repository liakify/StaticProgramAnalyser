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

	pRS1.child = stmtId2;
	pRS2.parent = stmtId1;

	for (auto parent : pRS1.allParents)
	{
		parentTable[parent].allChildren.insert(stmtId2);
	}

	pRS2.allParents = pRS1.allParents;
	pRS2.allParents.insert(stmtId1);
}

bool ParentKB::parent(StmtId stmtId1, StmtId stmtId2)
{
	return parentTable[stmtId1].child == stmtId2;
}

bool ParentKB::parentStar(StmtId stmtId1, StmtId stmtId2)
{
	parentRS pRS1 = parentTable[stmtId1];
	std::unordered_set<StmtId> parent1Children = pRS1.allChildren;
	return parent1Children.find(stmtId2) != parent1Children.end();
}

StmtId ParentKB::getParent(StmtId stmtId)
{
	return parentTable[stmtId].parent;
}

StmtId ParentKB::getChild(StmtId stmtId)
{
	return parentTable[stmtId].child;
}

std::unordered_set<StmtId> ParentKB::getAllParents(StmtId stmtId)
{
	return parentTable[stmtId].allParents;
}

std::unordered_set<StmtId> ParentKB::getAllChildren(StmtId stmtId)
{
	return parentTable[stmtId].allChildren;
}
