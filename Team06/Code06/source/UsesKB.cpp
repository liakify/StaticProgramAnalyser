#include "UsesKB.h"
#include <stdexcept>

void UsesKB::addStmtUses(StmtId stmtId, VarId var)
{
	stmtVarTable[stmtId].insert(var);
	varStmtTable[var].insert(stmtId);
}

bool UsesKB::stmtUses(StmtId stmtId, VarId var)
{
	try {
		std::unordered_set<VarId> varSet = stmtVarTable.at(stmtId);
		return varSet.find(var) != varSet.end();
	}
	catch (const std::out_of_range & oor) {
		return false;
	}
}

std::unordered_set<VarId> UsesKB::getAllVarsUsedByStmt(StmtId stmtId)
{
	try {
		return stmtVarTable.at(stmtId);
	}
	catch (const std::out_of_range & oor) {
		return {};
	}
}

std::unordered_set<StmtId> UsesKB::getAllStmtsUsingVar(VarId var)
{
	try {
		return varStmtTable.at(var);
	}
	catch (const std::out_of_range & oor) {
		return {};
	}
}
