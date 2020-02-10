#ifndef SIMPLE_H
#define SIMPLE_H

#include <string>
#include <vector>
#include "Types.h"

namespace SIMPLE {

	class Statement {
	public:
		StmtType getType();
	protected:
		Statement();
		StmtType stmtType;
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

	/*class RelFactor {
	private:
		Variable var;
	};

	class RelExpr {
	private:
		RelFactor leftFactor;
		RelFactor rightFactor;
	};

	class CondExpr {

	};*/

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
		IfStmt(StmtListId thenStmtLst, StmtListId elseStmtLst);
		StmtListId getThenStmtLstId();
		StmtListId getElseStmtLstId();
	private:
		//CondExpr cond;
		StmtListId thenStmtLst;
		StmtListId elseStmtLst;
	};

	class WhileStmt : public Statement {
	public:
		WhileStmt(StmtListId stmtLst);
		StmtListId getStmtLstId();
	public:
		//CondExpr cond;
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
}

#endif