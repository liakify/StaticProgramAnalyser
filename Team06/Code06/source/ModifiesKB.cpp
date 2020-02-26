#include "ModifiesKB.h"
#include <stdexcept>

void ModifiesKB::addStmtModifies(StmtId stmtId, VarId var)
{
	stmtVarTable[stmtId].insert(var);
	varStmtTable[var].insert(stmtId);
}

bool ModifiesKB::stmtModifies(StmtId stmtId, VarId var)
{
	try {
		std::unordered_set<VarId> varSet = stmtVarTable.at(stmtId);
		return varSet.find(var) != varSet.end();
	}
	catch (const std::out_of_range &) {
		return false;
	}
}

std::unordered_set<VarId> ModifiesKB::getAllVarsModifiedByStmt(StmtId stmtId)
{
	try {
		return stmtVarTable.at(stmtId);
	}
	catch (const std::out_of_range &) {
		return {};
	}
}

std::unordered_set<StmtId> ModifiesKB::getAllStmtsModifyVar(VarId var)
{
	try {
		return varStmtTable.at(var);
	}
	catch (const std::out_of_range &) {
		return {};
	}
}
