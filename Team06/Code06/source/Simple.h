#ifndef SIMPLE_H
#define SIMPLE_H

#include <string>
#include <vector>
#include "Types.h"

namespace SIMPLE {

	class Statement {

	};

	using StatementList = std::vector<Statement>;

	class Procedure {
	public:
		Procedure(ProcName procName, StatementList statements);
		bool operator== (const Procedure& p);
	private:
		ProcName procName;
		StatementList statements;
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

	};

	class PrintStmt : public Statement {
	private:
		Variable var;
	};

	class ReadStmt : public Statement {
	private:
		Variable var;
	};

	class IfStmt : public Statement {
	private:
		CondExpr cond;
		StmtList thenStmtList;
		StmtList ElseStmtList;
	};

	class WhileStmt : public Statement {
	public:
		CondExpr cond;
		StmtList stmtList;
	};

	class CallStmt : public Statement {
	public:
		Procedure procedure;
	};

	class AssignStmt : public Statement {
	public:
		Expression expr;
		Variable var;
	};*/
}

#endif