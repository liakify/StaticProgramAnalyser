#ifndef SIMPLE_H
#define SIMPLE_H

#include <string>
#include <vector>
#include "Types.h"

namespace SIMPLE {
	class Operand {
	public:
		Operand(Operand left, Operand right, char op);
		Operand(std::string name);
		std::string getStr();
	private:
		Operand* left;
		Operand* right;
		char op;
		std::string str;
	};

	class CondExpr {
	public:
		CondExpr(CondExpr left, CondExpr right);
		CondExpr(Operand left, Operand right);
	private:
		CondExpr* leftCond;
		CondExpr* rightCond;
		Operand* leftFactor;
		Operand* rightFactor;
	};

	class Statement {
	public:
		StmtType getType();
	protected:
		Statement();
		StmtType stmtType;
	};

	class PrintStmt : public Statement {
	public:
		PrintStmt(VarName var);
		VarName getVar();
	private:
		VarName var;
	};

	class ReadStmt : public Statement {
	public:
		ReadStmt(VarName var);
		VarName getVar();
	private:
		VarName var;
	};

	class IfStmt : public Statement {
	public:
		IfStmt(CondExpr cond, StmtListId thenStmtLst, StmtListId elseStmtLst);
		CondExpr getCondExpr();
		StmtListId getThenStmtLstId();
		StmtListId getElseStmtLstId();
	private:
		CondExpr condExpr;
		StmtListId thenStmtLst;
		StmtListId elseStmtLst;
	};

	class WhileStmt : public Statement {
	public:
		WhileStmt(CondExpr cond, StmtListId stmtLst);
		CondExpr getCondExpr();
		StmtListId getStmtLstId();
	public:
		CondExpr condExpr;
		StmtListId stmtLst;
	};

	class CallStmt : public Statement {
	public:
		CallStmt(ProcName procName);
		ProcName getProc();
	public:
		ProcName procName;
	};

	class AssignStmt : public Statement {
	public:
		AssignStmt(VarName var, Operand expr);
		VarName getVar();
		Operand getExpr();
	public:
		Operand expr;
		VarName var;
	};

	class StatementList {
	public:
		StatementList(std::vector<StmtId>& statements);
		bool operator== (const StatementList& other);
		std::vector<StmtId> getStmtIds();
	private:
		std::vector<StmtId> statements;
	};

	class Procedure {
	public:
		Procedure(ProcName procName, StmtListId stmtLstId);
		bool operator== (const Procedure& p);
		ProcName getName();
		StmtListId getStmtLstId();
	private:
		ProcName procName;
		StmtListId stmtLstId;
	};
}

#endif