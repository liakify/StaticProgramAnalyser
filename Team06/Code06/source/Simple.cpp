#include "Simple.h"

namespace SIMPLE {
	Expression::Expression(Expression left, Expression right, char op)
		: left(&left), right(&right), op(op) {
		this->str = getStr();
		this->varSet = std::set<VarId>(left.varSet);
		this->varSet.insert(right.varSet.begin(), right.varSet.end());
		this->constSet = std::set<ConstValue>(left.constSet);
		this->constSet.insert(right.constSet.begin(), right.constSet.end());
	}

	Expression::Expression(VarName name, VarId id)
		: left(nullptr), right(nullptr), op('\0'), str(name) {
		varSet.insert(id);
	}

	Expression::Expression(ConstValue name)
		: left(nullptr), right(nullptr), op('\0'), str(name) {
		constSet.insert(name);
	}

	std::set<VarId> Expression::getVarIds() {
		return this->varSet;
	}
	
	std::set<ConstValue> Expression::getConstValues() {
		return this->constSet;
	}

	std::string Expression::getStr() {
		if (this->str != "") {
			return this->str;
		}
		else {
			return "(" + this->left->getStr() + this->op + this->right->getStr() + ")";
		}
	}

	CondExpr::CondExpr(CondExpr left, CondExpr right)
		: leftCond(&left), rightCond(&right), leftFactor(nullptr), rightFactor(nullptr) {
	}

	CondExpr::CondExpr(Expression left, Expression right)
		: leftCond(nullptr), rightCond(nullptr), leftFactor(&left), rightFactor(&right) {
	}

	Statement::Statement() {}

	StmtType Statement::getType() {
		return this->stmtType;
	}

	PrintStmt::PrintStmt(VarId var)
		: var(var) {
		this->stmtType = PRINT;
	}

	VarId PrintStmt::getVar() {
		return this->var;
	}

	ReadStmt::ReadStmt(VarId var)
		: var(var) {
		this->stmtType = READ;
	}
	
	VarId ReadStmt::getVar() {
		return this->var;
	}

	IfStmt::IfStmt(CondExpr cond, StmtListId thenStmtLst, StmtListId elseStmtLst)
		: condExpr(cond), thenStmtLst(thenStmtLst), elseStmtLst(elseStmtLst) {
		this->stmtType = IF;
	}

	CondExpr IfStmt::getCondExpr() {
		return this->condExpr;
	}

	StmtListId IfStmt::getThenStmtLstId() {
		return this->thenStmtLst;
	}

	StmtListId IfStmt::getElseStmtLstId() {
		return this->elseStmtLst;
	}

	WhileStmt::WhileStmt(CondExpr cond, StmtListId stmtLst)
		: condExpr(cond), stmtLst(stmtLst) {
		this->stmtType = WHILE;
	}

	CondExpr WhileStmt::getCondExpr() {
		return this->condExpr;
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

	AssignStmt::AssignStmt(VarId var, Expression expr)
		: var(var), expr(expr) {
		this->stmtType = ASSIGN;
	}

	VarId AssignStmt::getVar() {
		return this->var;
	}

	Expression AssignStmt::getExpr() {
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
}
