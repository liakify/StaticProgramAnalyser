#include <string>

class Statement {
	
};

class Expression {

};

class Variable {
	string var_name;

};

class RelFactor {
	Variable var;
};

class RelExpr {

};

class CondExpr {

};

class PrintStmt : public Statement {
	Variable var;
};

class ReadStmt : public Statement {
	Variable var;
};

class IfStmt : public Statement {
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
};

class StmtList {
	Statement statement[];
public:
	StmtList(int noOfStmts) {
		statement[] = new Statement[noOfStmts]
	}
};

class Procedure {
	StmtList stmtlist;
};

class Program {
	Procedure main;
};