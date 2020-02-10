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

	IfStmt::IfStmt(StatementList thenStmtLst, StatementList elseStmtLst) 
		: thenStmtLst(thenStmtLst), elseStmtLst(elseStmtLst) {
		this->stmtType = IF;
	}

	StatementList IfStmt::getThenStmtLst() {
		return this->thenStmtLst;
	}

	StatementList IfStmt::getElseStmtLst() {
		return this->elseStmtLst;
	}

	WhileStmt::WhileStmt(StatementList stmtLst)
		: stmtLst(stmtLst) {
		this->stmtType = WHILE;
	}

	StatementList WhileStmt::getStmtLst() {
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

	StatementList::StatementList(std::vector<Statement>& statements) 
		: statements(statements) {
	}

	bool StatementList::operator== (const StatementList& other) {
		return true;
	}

	Procedure::Procedure(ProcName procName, StatementList statements)
		: statements(statements), procName(procName) {
	}

	bool Procedure::operator== (const Procedure& p) {
		return procName == p.procName;
	}

	ProcName Procedure::getName() {
		return procName;
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
