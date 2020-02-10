#include "Simple.h"
#include "LoggingUtils.h"

namespace SIMPLE {
	Statement::Statement() {}

	StmtType Statement::getType() {
		return this->stmtType;
	}

	PrintStmt::PrintStmt(VarName var)
		: var(var) {
		this->stmtType = PRINT;
	}

	VarName PrintStmt::getVar() {
		return this->var;
	}

	ReadStmt::ReadStmt(VarName var)
		: var(var) {
		this->stmtType = READ;
	}
	
	VarName ReadStmt::getVar() {
		return this->var;
	}

	IfStmt::IfStmt(StmtListId thenStmtLst, StmtListId elseStmtLst)
		: thenStmtLst(thenStmtLst), elseStmtLst(elseStmtLst) {
		this->stmtType = IF;
	}

	StmtListId IfStmt::getThenStmtLstId() {
		return this->thenStmtLst;
	}

	StmtListId IfStmt::getElseStmtLstId() {
		return this->elseStmtLst;
	}

	WhileStmt::WhileStmt(StmtListId stmtLst)
		: stmtLst(stmtLst) {
		this->stmtType = WHILE;
	}

	StmtListId WhileStmt::getStmtLstId() {
		return this->stmtLst;
	}

	CallStmt::CallStmt(ProcName procName)
		: procName(procName) {
		this->stmtType = CALL;
	}

	ProcName CallStmt::getProc() {
		return this->procName;
	}

	AssignStmt::AssignStmt(VarName var, Operand expr)
		: var(var), expr(expr) {
		this->stmtType = ASSIGN;
	}

	VarName AssignStmt::getVar() {
		return this->var;
	}

	Operand AssignStmt::getExpr() {
		return this->expr;
	}

	StatementList::StatementList(std::vector<StmtId>& statements) 
		: statements(statements) {
	}

	bool StatementList::operator== (const StatementList& other) {
		return true;
	}

	std::vector<StmtId> StatementList::getStmtIds() {
		return this->statements;
	}

	Procedure::Procedure(ProcName procName, StmtListId stmtLstId)
		: procName(procName), stmtLstId(stmtLstId) {
	}

	bool Procedure::operator== (const Procedure& p) {
		return procName == p.procName;
	}

	ProcName Procedure::getName() {
		return this->procName;
	}

	StmtListId Procedure::getStmtLstId() {
		return this->stmtLstId;
	}

	Operand::Operand(Operand left, Operand right, char op) 
		: left(&left), right(&right), op(op) {
		this->str = getStr();
	}

	Operand::Operand(std::string name)
		: left(nullptr), right(nullptr), op('\0'), str(name) {
	}

	std::string Operand::getStr() {
		if (this->str != "") {
			return this->str;
		}
		else {
			return "(" + this->left->getStr() + std::string(1, this->op) + this->right->getStr() + ")";
		}
	}
}
