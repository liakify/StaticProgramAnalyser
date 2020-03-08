#include "ModifiesKB.h"
#include <stdexcept>

void ModifiesKB::addStmtModifies(StmtId stmtId, VarId varId)
{
	stmtVarTable[stmtId].insert(varId);
	varStmtTable[varId].insert(stmtId);
}

bool ModifiesKB::stmtModifies(StmtId stmtId, VarId varId)
{
	try {
		std::unordered_set<VarId> varSet = stmtVarTable.at(stmtId);
		return varSet.find(varId) != varSet.end();
	}
	catch (const std::out_of_range &) {
		return false;
	}
}

void ModifiesKB::addProcModifies(ProcId procId, VarId varId)
{
	procVarTable[procId].insert(varId);
	varProcTable[varId].insert(procId);
}

bool ModifiesKB::procModifies(ProcId procId, VarId varId)
{
	try {
		std::unordered_set<VarId> varSet = procVarTable.at(procId);
		return varSet.find(varId) != varSet.end();
	}
	catch (const std::out_of_range&) {
		return false;
	}
}

const std::unordered_set<VarId>& ModifiesKB::getAllVarsModifiedByStmt(StmtId stmtId)
{
	try {
		return stmtVarTable.at(stmtId);
	}
	catch (const std::out_of_range &) {
		return EMPTY_RESULT;
	}
}

const std::unordered_set<StmtId>& ModifiesKB::getAllStmtsModifyVar(VarId varId)
{
	try {
		return varStmtTable.at(varId);
	}
	catch (const std::out_of_range &) {
		return EMPTY_RESULT;
	}
}

const std::unordered_set<VarId>& ModifiesKB::getAllVarsModifiedByProc(ProcId procId)
{
	try {
		return procVarTable.at(procId);
	}
	catch (const std::out_of_range&) {
		return EMPTY_RESULT;
	}
}

const std::unordered_set<ProcId>& ModifiesKB::getAllProcModifyVar(VarId varId)
{
	try {
		return varProcTable.at(varId);
	}
	catch (const std::out_of_range&) {
		return EMPTY_RESULT;
	}
}
