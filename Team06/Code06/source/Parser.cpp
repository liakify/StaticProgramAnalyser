#include "Parser.h"
#include <stack>

using std::invalid_argument;

namespace Parser{
	Parser::Parser(string source, PKB::PKB& pkb)
		: src(source), pkb(pkb) {
		this->pos = 0;
	}

	void Parser::parse() {
		program();
	}

	int analyse(string& src) {
		Parser p = Parser(src, PKB::PKB());
		p.parse();
		return 0;
	}

	string Parser::consume(regex rgx) {
		std::smatch match;
		string str = src.substr(this->pos);
		if (!regex_search(str, match, rgx, std::regex_constants::match_continuous)) {
			throw invalid_argument("Syntax error in SIMPLE source.\n");
		}
		this->pos += match.length();
		return match.str();
	}

	string Parser::name() {
		return consume(regex("[A-Z|a-z][A-Z|a-z|0-9]*"));
	}

	string Parser::integer() {
		return consume(regex("[0-9]+"));
	}

	void Parser::program() {
		while (this->pos < src.length()) {
			procedure();
		}
	}

	ProcId Parser::procedure() {
		consume(regex("[[:space:]]*procedure[[:space:]]+"));
		ProcName pn = proc_name();
		consume(regex("[[:space:]]*[{][[:space:]]*"));
		StmtListId sl = stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*"));
		Procedure p = Procedure(pn, sl);
		return pkb.procTable.insertProc(p);
	}

	StmtListId Parser::stmtLst() {
		std::vector<StmtId> statements;
		statements.push_back(stmt());
		while (true) {
			try {
				statements.push_back(stmt());
			} catch (const invalid_argument&) {
				break;
			}
		}
		StatementList sl = StatementList(statements);
		return pkb.stmtListTable.insertStmtLst(sl);
	}

	StmtId Parser::stmt() {
		int currentPos = this->pos;
		try {
			return pkb.stmtTable.insertStmt(read_stmt());
		} catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			return pkb.stmtTable.insertStmt(print_stmt());
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			return pkb.stmtTable.insertStmt(call_stmt());
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			return pkb.stmtTable.insertStmt(while_stmt());
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			return pkb.stmtTable.insertStmt(if_stmt());
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		return pkb.stmtTable.insertStmt(assign_stmt());
	}

	ReadStmt Parser::read_stmt() {
		consume(regex("[[:space:]]*read[[:space:]]+"));
		VarName v = var_name();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
		return ReadStmt(v);
	}

	PrintStmt Parser::print_stmt() {
		consume(regex("[[:space:]]*print[[:space:]]+"));
		VarName v = var_name();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
		return PrintStmt(v);
	}

	CallStmt Parser::call_stmt() {
		consume(regex("[[:space:]]*call[[:space:]]+"));
		ProcName p = proc_name();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
		return CallStmt(p);
	}

	WhileStmt Parser::while_stmt() {
		consume(regex("[[:space:]]*while[[:space:]]*[(][[:space:]]*"));
		CondExpr cond = cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*[{][[:space:]]*"));
		StmtListId sl = stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*"));
		return WhileStmt(cond, sl);
	}

	IfStmt Parser::if_stmt() {
		consume(regex("[[:space:]]*if[[:space:]]*[(][[:space:]]*"));
		CondExpr cond = cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*then[[:space:]]*[{][[:space:]]*"));
		StmtListId thenStmtLst = stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*else[[:space:]]*[{][[:space:]]*"));
		StmtListId elseStmtLst = stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*"));
		return IfStmt(cond, thenStmtLst, elseStmtLst);
	}

	AssignStmt Parser::assign_stmt() {
		VarName v = var_name();
		consume(regex("[[:space:]]*[=][[:space:]]*"));
		Operand exp = expr();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
		return AssignStmt(v, exp);
	}

	CondExpr Parser::cond_expr() {
		int currentPos = this->pos;
		try {
			return rel_expr();
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			consume(regex("[[:space:]]*[!][[:space:]]*[(][[:space:]]*"));
			CondExpr negated = cond_expr();
			consume(regex("[[:space:]]*[)][[:space:]]*"));
			return negated;
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			consume(regex("[[:space:]]*[(][[:space:]]*"));
			CondExpr left = cond_expr();
			consume(regex("[[:space:]]*[)][[:space:]]*(&&)[[:space:]]*[(][[:space:]]*"));
			CondExpr right = cond_expr();
			consume(regex("[[:space:]]*[)][[:space:]]*"));
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		consume(regex("[[:space:]]*[(][[:space:]]*"));
		CondExpr left = cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*(||)[[:space:]]*[(][[:space:]]*"));
		CondExpr right = cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*"));
		return CondExpr(left, right);
	}

	CondExpr Parser::rel_expr() {
		int currentPos = this->pos;
		try {
			Operand left = rel_factor();
			consume(regex("[[:space:]]*(>)[[:space:]]*"));
			Operand right = rel_factor();
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			Operand left = rel_factor();
			consume(regex("[[:space:]]*(>=)[[:space:]]*"));
			Operand right = rel_factor();
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			Operand left = rel_factor();
			consume(regex("[[:space:]]*(<)[[:space:]]*"));
			Operand right = rel_factor();
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			Operand left = rel_factor();
			consume(regex("[[:space:]]*(<=)[[:space:]]*"));
			Operand right = rel_factor();
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			Operand left = rel_factor();
			consume(regex("[[:space:]]*(==)[[:space:]]*"));
			Operand right = rel_factor();
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		Operand left = rel_factor();
		consume(regex("[[:space:]]*(!=)[[:space:]]*"));
		Operand right = rel_factor();
		return CondExpr(left, right);
	}

	Operand Parser::rel_factor() {
		int currentPos = this->pos;
		try {
			return Operand(var_name());
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			return Operand(const_value());
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		return expr();
	}

	int Parser::get_op_rank(char op) {
		if (op == '*' || op == '/' || op == '%') {
			return 1;
		}
		else if (op == '+' || op == '-') {
			return 0;
		}
		else {
			throw invalid_argument("Unknown operator while parsing expression in SIMPLE.");
		}
	}

	//-1 if op1 < op2, 0 if op1 == op2, 1 if op1 > op2
	int Parser::compare_op(char op1, char op2) {
		return get_op_rank(op1) - get_op_rank(op2);
	}

	Operand Parser::expr() {
		int currentPos = this->pos;
		char op;
		std::stack<Operand> operands;
		std::stack<char> operators;
		consume(regex("[[:space:]]*"));
		Operand token = factor();
		operands.push(token);
		while (true) {
			try {
				consume(regex("[[:space:]]*"));
				op = consume(regex("[+%\*\-\/][[:space:]]*"))[0];
				if (!operators.empty() && compare_op(operators.top(), op) != -1) {
					Operand right = operands.top();
					operands.pop();
					Operand left = operands.top();
					operands.pop();
					operands.push(Operand(left, right, operators.top()));
					operators.pop();
				}
				operators.push(op);
				token = factor();
				operands.push(token);
			}
			catch (const invalid_argument & e) {
				if (operands.size() - operators.size() != 1) {
					throw e;
				}
				break;
			}
		}
		while (!operators.empty() && operands.size() > 1) {
			Operand right = operands.top();
			operands.pop();
			Operand left = operands.top();
			operands.pop();
			operands.push(Operand(left, right, operators.top()));
			operators.pop();
		}
		consume(regex("[[:space:]]*"));
		return operands.top();
	}

	Operand Parser::factor() {
		int currentPos = this->pos;
		try {
			return Operand(var_name());
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			return Operand(const_value());
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		consume(regex("[[:space:]]*[(][[:space:]]*"));
		Operand opr = expr();
		consume(regex("[[:space:]]*[)][[:space:]]*"));
		return opr;
	}

	VarName Parser::var_name() {
		VarName vn = name();
		pkb.varTable.insertVar(vn);
		return vn;
	}

	ProcName Parser::proc_name() {
		return name();
	}

	ConstValue Parser::const_value() {
		return integer();
	}
}