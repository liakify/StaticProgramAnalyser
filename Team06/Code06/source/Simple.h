#ifndef SIMPLE_H
#define SIMPLE_H

#include <string>
#include <vector>
#include "Types.h"

namespace SIMPLE {
	/**
	The Operand class is used to represent expressions in SIMPLE. 
	An Operand is either a literal, or combines 2 other Operands via an operator.
	*/
	class Operand {
	public:
		/**
		Constructor for a non-literal Operand.
		@param	left	Left operand
		@param	right	Right operand
		@param	op		Operator
		*/
		Operand(Operand left, Operand right, char op);

		/**
		Constructor for a literal Operand.
		Left, right and op will be initialised with null values.
		@param	string	String representation of the literal
		*/
		Operand(std::string name);

		/**
		Recursively constructs the string representation of an Operand.
		Parentheses are added to avoid ambiguity in reading the order of operations.
		@return	the string representation
		*/
		std::string getStr();
	private:
		Operand* left;
		Operand* right;
		char op;
		std::string str;
	};

	/**
	The CondExpr class is used to represent conditional expressions in SIMPLE.
	A CondExpr combines either 2 CondExprs or 2 Operands but not both.
	When combining 2 CondExprs, the Operands will be nullptr and vice versa.
	*/
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