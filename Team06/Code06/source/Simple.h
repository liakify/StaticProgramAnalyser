#ifndef SIMPLE_H
#define SIMPLE_H

#include <string>
#include <vector>
#include <set>
#include "Types.h"

namespace SIMPLE {
	/**
	* The Expression class is used to represent expressions in SIMPLE. 
	* An Expression is either a literal, or combines 2 other Expressions via an operator.
	* Internally, it maintains one VarId set and one ConstValue set representing
	* the variables and constants present in the expression.
	*/
	class Expression {
	public:
		/**
		* Constructor for a non-literal Expression. The sets of the sub-expressions
		* will be unioned to form the VarId and ConstValue sets of the expression.
		* 
		* @param	left	Left operand
		* @param	right	Right operand
		* @param	op		Operator
		*/
		Expression(Expression left, Expression right, char op);

		/**
		* Constructor for a variable Expression.
		* Left, right and op will be initialised with null values.
		* 
		* @param	name	Name of the variable
		* @param	id		ID of the variable as stored in the PKB
		*/
		Expression(VarName name, VarId id);

		/**
		* Constructor for a constant value Expression.
		* Left, right and op will be initialised with null values.
		* 
		* @param	ConstValue	String representation of the constant
		*/
		Expression(ConstValue name);

		/**
		* Recursively constructs the string representation of an Expression.
		* Parentheses are added to avoid ambiguity in reading the order of operations.
		* 
		* @return	the string representation
		*/
		std::string getStr();
		std::set<VarId> getVarIds();
		std::set<ConstValue> getConstValues();
	private:
		Expression* left;
		Expression* right;
		char op;
		std::string str;
		std::set<VarId> varSet;
		std::set<ConstValue> constSet;
	};

	/**
	* The CondExpr class is used to represent conditional expressions in SIMPLE.
	* A CondExpr combines either 2 CondExprs or 2 Expressions but not both.
	* When combining 2 CondExprs, the Expressions will be nullptr and vice versa.
	*/
	class CondExpr {
	public:
		CondExpr(CondExpr left, CondExpr right);
		CondExpr(Expression left, Expression right);
	private:
		CondExpr* leftCond;
		CondExpr* rightCond;
		Expression* leftFactor;
		Expression* rightFactor;
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
		PrintStmt(VarId var);
		VarId getVar();
	private:
		VarId var;
	};

	class ReadStmt : public Statement {
	public:
		ReadStmt(VarId var);
		VarId getVar();
	private:
		VarId var;
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
		AssignStmt(VarId var, Expression expr);
		VarId getVar();
		Expression getExpr();
	public:
		Expression expr;
		VarId var;
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