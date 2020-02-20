#include "ParentKB.h"
#include <stdexcept>

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

	parentRS& pRS1 = parentTable.at(stmtId1);
	parentRS& pRS2 = parentTable.at(stmtId2);

	pRS1.directChildren.insert(stmtId2);
	pRS2.parent = stmtId1;
}

bool ParentKB::parent(StmtId stmtId1, StmtId stmtId2)
{
	try {
		std::unordered_set<StmtId> stmtId1Children = parentTable.at(stmtId1).directChildren;
		return stmtId1Children.find(stmtId2) != stmtId1Children.end();
	}
	catch (const std::out_of_range & oor) {
		return false;
	}
}

bool ParentKB::parentStar(StmtId stmtId1, StmtId stmtId2)
{
	try {
		std::unordered_set<StmtId> stmtId1AllChildren = parentTable.at(stmtId1).allChildren;
		return stmtId1AllChildren.find(stmtId2) != stmtId1AllChildren.end();
	}
	catch (const std::out_of_range & oor) {
		return false;
	}
}

StmtId ParentKB::getParent(StmtId stmtId)
{
	try {
		return parentTable.at(stmtId).parent;
	}
	catch (const std::out_of_range & oor) {
		return INVALID_STMT_ID;
	}
}

std::unordered_set<StmtId> ParentKB::getDirectChildren(StmtId stmtId)
{
	try {
		return parentTable.at(stmtId).directChildren;
	}
	catch (const std::out_of_range & oor) {
		return {};
	}
}

bool ParentKB::hasParent(StmtId stmtId)
{
	try {
		return parentTable.at(stmtId).parent != 0;
	}
	catch (const std::out_of_range & oor) {
		return false;
	}
}

bool ParentKB::hasDirectChildren(StmtId stmtId)
{
	try {
		return parentTable.at(stmtId).directChildren.size() != 0;
	}
	catch (const std::out_of_range & oor) {
		return false;
	}
}

std::unordered_set<StmtId> ParentKB::getAllParents(StmtId stmtId)
{
	try {
		return parentTable.at(stmtId).allParents;
	}
	catch (const std::out_of_range & oor) {
		return {};
	}
}

std::unordered_set<StmtId> ParentKB::getAllChildren(StmtId stmtId)
{
	try {
		return parentTable.at(stmtId).allChildren;
	}
	catch (const std::out_of_range & oor) {
		return {};
	}
}

void ParentKB::setAllChildren(StmtId stmtId, std::unordered_set<StmtId> children)
{
	try {
		parentTable.at(stmtId).allChildren = children;
	}
	catch (const std::out_of_range & oor) {
		return;
	}
}

void ParentKB::setAllParents(StmtId stmtId, std::unordered_set<StmtId> parents)
{
	try {
		parentTable.at(stmtId).allParents = parents;
	}
	catch (const std::out_of_range & oor) {
		return;
	}
}
