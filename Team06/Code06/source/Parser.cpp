#include "Parser.h"

using std::invalid_argument;
using std::regex;
using std::string;

namespace FrontEnd {
	PKB::PKB Parser::parseSimple(string src) {
		this->pkb = PKB::PKB();
		this->isExpression = false;
		this->src = src;
		this->pos = 0;
		program();
		return this->pkb;
	}

	string Parser::parseExpression(string exp) {
		this->pkb = PKB::PKB();
		this->isExpression = true;
		this->src = exp;
		this->pos = 0;
		return expr().getStr();
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
		procedure();
		while (this->pos < src.length()) {
			procedure();
		}
	}

	ProcId Parser::procedure() {
		consume(regex("[\\s]*procedure[\\s]+"));
		ProcName pn = proc_name();
		consume(regex("[\\s]*[{][\\s]*"));
		StmtListId sl = stmtLst();
		consume(regex("[\\s]*[}][\\s]*"));
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
			StmtId stmtId = pkb.stmtTable.reserveId();
			pkb.stmtTable.insertStmtAtId(while_stmt(), stmtId);
			return stmtId;
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
			pkb.stmtTable.unreserveId();
		}
		try {
			StmtId stmtId = pkb.stmtTable.reserveId();
			pkb.stmtTable.insertStmtAtId(if_stmt(), stmtId);
			return stmtId;
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
			pkb.stmtTable.unreserveId();
		}
		return pkb.stmtTable.insertStmt(assign_stmt());
	}

	ReadStmt* Parser::read_stmt() {
		consume(regex("[\\s]*read[\\s]+"));
		VarId v = var_name();
		consume(regex("[\\s]*[;][\\s]*"));
		ReadStmt* rs = new ReadStmt(v);
		return rs;
	}

	PrintStmt* Parser::print_stmt() {
		consume(regex("[\\s]*print[\\s]+"));
		VarId v = var_name();
		consume(regex("[\\s]*[;][\\s]*"));
		PrintStmt* ps = new PrintStmt(v);
		return ps;
	}

	CallStmt* Parser::call_stmt() {
		consume(regex("[\\s]*call[\\s]+"));
		ProcName p = proc_name();
		consume(regex("[\\s]*[;][\\s]*"));
		CallStmt* cs = new CallStmt(p);
		return cs;
	}

	WhileStmt* Parser::while_stmt() {
		consume(regex("[\\s]*while[\\s]*[(][\\s]*"));
		CondExpr cond = cond_expr();
		consume(regex("[\\s]*[)][\\s]*[{][\\s]*"));
		StmtListId sl = stmtLst();
		consume(regex("[\\s]*[}][\\s]*"));
		WhileStmt* ws = new WhileStmt(cond, sl);
		return ws;
	}

	IfStmt* Parser::if_stmt() {
		consume(regex("[\\s]*if[\\s]*[(][\\s]*"));
		CondExpr cond = cond_expr();
		consume(regex("[\\s]*[)][\\s]*then[\\s]*[{][\\s]*"));
		StmtListId thenStmtLst = stmtLst();
		consume(regex("[\\s]*[}][\\s]*else[\\s]*[{][\\s]*"));
		StmtListId elseStmtLst = stmtLst();
		consume(regex("[\\s]*[}][\\s]*"));
		IfStmt* ifs = new IfStmt(cond, thenStmtLst, elseStmtLst);
		return ifs;
	}

	AssignStmt* Parser::assign_stmt() {
		VarId v = var_name();
		consume(regex("[\\s]*[=][\\s]*"));
		Expression exp = expr();
		consume(regex("[\\s]*[;][\\s]*"));
		AssignStmt* as = new AssignStmt(v, exp);
		return as;
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
			consume(regex("[\\s]*[!][\\s]*[(][\\s]*"));
			CondExpr negated = cond_expr();
			consume(regex("[\\s]*[)][\\s]*"));
			return negated;
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			consume(regex("[\\s]*[(][\\s]*"));
			CondExpr left = cond_expr();
			consume(regex("[\\s]*[)][\\s]*(&&)[\\s]*[(][\\s]*"));
			CondExpr right = cond_expr();
			consume(regex("[\\s]*[)][\\s]*"));
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		consume(regex("[\\s]*[(][\\s]*"));
		CondExpr left = cond_expr();
		consume(regex("[\\s]*[)][\\s]*(\\|\\|)[\\s]*[(][\\s]*"));
		CondExpr right = cond_expr();
		consume(regex("[\\s]*[)][\\s]*"));
		return CondExpr(left, right);
	}

	CondExpr Parser::rel_expr() {
		int currentPos = this->pos;
		try {
			Expression left = expr();
			consume(regex("[\\s]*(>)[\\s]*"));
			Expression right = expr();
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			Expression left = expr();
			consume(regex("[\\s]*(>=)[\\s]*"));
			Expression right = expr();
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			Expression left = expr();
			consume(regex("[\\s]*(<)[\\s]*"));
			Expression right = expr();
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			Expression left = expr();
			consume(regex("[\\s]*(<=)[\\s]*"));
			Expression right = expr();
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			Expression left = expr();
			consume(regex("[\\s]*(==)[\\s]*"));
			Expression right = expr();
			return CondExpr(left, right);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		Expression left = expr();
		consume(regex("[\\s]*(!=)[\\s]*"));
		Expression right = expr();
		return CondExpr(left, right);
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

	void Parser::combine_op(std::stack<Expression>& operands, std::stack<char>& operators) {
		Expression right = operands.top();
		operands.pop();
		Expression left = operands.top();
		operands.pop();
		operands.push(Expression(left, right, operators.top()));
		operators.pop();
	}

	Expression Parser::expr() {
		int currentPos = this->pos;
		char op;
		std::stack<Expression> operands;
		std::stack<char> operators;
		consume(regex("[\\s]*"));
		Expression token = factor();
		operands.push(token);
		while (true) {
			try {
				consume(regex("[\\s]*"));
				op = consume(regex("[\\+\\%\\*\\-\\/][\\s]*"))[0];
				if (!operators.empty() && compare_op(operators.top(), op) != -1) {
					combine_op(operands, operators);
				}
				operators.push(op);
				operands.push(factor());
			}
			catch (const invalid_argument & e) {
				if (operands.size() - operators.size() != 1) {
					throw e;
				}
				break;
			}
		}
		while (!operators.empty() && operands.size() > 1) {
			combine_op(operands, operators);
		}
		consume(regex("[\\s]*"));
		return operands.top();
	}

	Expression Parser::factor() {
		int currentPos = this->pos;
		try {
			if (isExpression) {
				// Sentinel id value
				return Expression(name(), -1, ExprType::VAR);
			}
			else {
				VarId id = var_name();
				VarName name = pkb.varTable.get(id);
				return Expression(name, id, ExprType::VAR);
			}
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			ConstId id = const_value();
			ConstValue value = pkb.constTable.get(id);
			return Expression(value, id, ExprType::CONST);
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		consume(regex("[\\s]*[(][\\s]*"));
		Expression opr = expr();
		consume(regex("[\\s]*[)][\\s]*"));
		return opr;
	}

	VarId Parser::var_name() {
		return pkb.varTable.insertVar(name());
	}

	ProcName Parser::proc_name() {
		return name();
	}

	ConstId Parser::const_value() {
		return pkb.constTable.insertConst(integer());
	}
}