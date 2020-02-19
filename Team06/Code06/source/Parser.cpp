#include "Parser.h"

using std::invalid_argument;

namespace Parser{
	void Parser::parseSimple(string src, PKB::PKB& pkb) {
		this->isExpression = false;
		this->pkb = pkb;
		this->src = src;
		this->pos = 0;
		program();
	}

	Expression Parser::parseExpression(string exp) {
		this->pkb = PKB::PKB();
		this->isExpression = true;
		this->src = exp;
		this->pos = 0;
		return expr();
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
		for (size_t i = 0; i < statements.size() - 1; i++) {
			pkb.followsKB.addFollows(statements[i], statements[i + 1]);
		}
		StatementList sl = StatementList(statements);
		return pkb.stmtListTable.insertStmtLst(sl);
	}

	StmtId Parser::stmt() {
		int currentPos = this->pos;
		try {
			ReadStmt readStmt = read_stmt();
			StmtId stmtId = pkb.stmtTable.insertStmt(readStmt);
			pkb.modifiesKB.addStmtModifies(stmtId, readStmt.getVar());
			return stmtId;
		} catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			PrintStmt printStmt = print_stmt();
			StmtId stmtId = pkb.stmtTable.insertStmt(printStmt);
			pkb.usesKB.addStmtUses(stmtId, printStmt.getVar());
			return stmtId;
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
			WhileStmt whileStmt = while_stmt();
			StmtId stmtId = pkb.stmtTable.insertStmt(whileStmt);
			StmtListId stmtLstId = whileStmt.getStmtLstId();
			populateParentKB(stmtId, stmtLstId);
			populateUsesKB(stmtId, whileStmt.getCondExpr().getVarIds());
			populateUsesKB(stmtId, getAllUses(stmtLstId));
			populateModifiesKB(stmtId, getAllModifies(stmtLstId));
			return stmtId;
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			IfStmt ifStmt = if_stmt();
			StmtId stmtId = pkb.stmtTable.insertStmt(ifStmt);
			StmtListId stmtLstId1 = ifStmt.getThenStmtLstId();
			StmtListId stmtLstId2 = ifStmt.getElseStmtLstId();
			populateParentKB(stmtId, stmtLstId1);
			populateParentKB(stmtId, stmtLstId2);
			populateUsesKB(stmtId, ifStmt.getCondExpr().getVarIds());
			populateUsesKB(stmtId, getAllUses(stmtLstId1));
			populateUsesKB(stmtId, getAllUses(stmtLstId2));
			populateModifiesKB(stmtId, getAllModifies(stmtLstId1));
			populateModifiesKB(stmtId, getAllModifies(stmtLstId2));
			return stmtId;
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		AssignStmt assignStmt = assign_stmt();
		Expression exp = assignStmt.getExpr();
		StmtId stmtId = pkb.stmtTable.insertStmt(assignStmt);
		pkb.modifiesKB.addStmtModifies(stmtId, assignStmt.getVar());
		populateUsesKB(stmtId, exp.getVarIds());
		populatePatternKB(stmtId, exp);
		VarName vn = pkb.varTable.get(assignStmt.getVar());
		pkb.patternKB.addLHSPattern(vn, stmtId);
		return stmtId;
	}

	ReadStmt Parser::read_stmt() {
		consume(regex("[\\s]*read[\\s]+"));
		VarId v = var_name();
		consume(regex("[\\s]*[;][\\s]*"));
		return ReadStmt(v);
	}

	PrintStmt Parser::print_stmt() {
		consume(regex("[\\s]*print[\\s]+"));
		VarId v = var_name();
		consume(regex("[\\s]*[;][\\s]*"));
		return PrintStmt(v);
	}

	CallStmt Parser::call_stmt() {
		consume(regex("[\\s]*call[\\s]+"));
		ProcName p = proc_name();
		consume(regex("[\\s]*[;][\\s]*"));
		return CallStmt(p);
	}

	WhileStmt Parser::while_stmt() {
		consume(regex("[\\s]*while[\\s]*[(][\\s]*"));
		CondExpr cond = cond_expr();
		consume(regex("[\\s]*[)][\\s]*[{][\\s]*"));
		StmtListId sl = stmtLst();
		consume(regex("[\\s]*[}][\\s]*"));
		return WhileStmt(cond, sl);
	}

	IfStmt Parser::if_stmt() {
		consume(regex("[\\s]*if[\\s]*[(][\\s]*"));
		CondExpr cond = cond_expr();
		consume(regex("[\\s]*[)][\\s]*then[\\s]*[{][\\s]*"));
		StmtListId thenStmtLst = stmtLst();
		consume(regex("[\\s]*[}][\\s]*else[\\s]*[{][\\s]*"));
		StmtListId elseStmtLst = stmtLst();
		consume(regex("[\\s]*[}][\\s]*"));
		return IfStmt(cond, thenStmtLst, elseStmtLst);
	}

	AssignStmt Parser::assign_stmt() {
		VarId v = var_name();
		consume(regex("[\\s]*[=][\\s]*"));
		Expression exp = expr();
		consume(regex("[\\s]*[;][\\s]*"));
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
		consume(regex("[\\s]*[)][\\s]*(||)[\\s]*[(][\\s]*"));
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
				// Sentinel VarId value
				return Expression(name(), -1);
			}
			else {
				VarId id = var_name();
				VarName name = pkb.varTable.get(id);
				return Expression(name, id);
			}
		}
		catch (const invalid_argument&) {
			this->pos = currentPos;
		}
		try {
			return Expression(const_value());
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

	ConstValue Parser::const_value() {
		return integer();
	}

	std::unordered_set<VarId> Parser::getAllUses(StmtListId sid) {
		std::unordered_set<VarId> result;
		StatementList sl = pkb.stmtListTable.get(sid);
		std::vector<StmtId> idList = sl.getStmtIds();
		for (StmtId id : idList) {
			std::unordered_set<VarId> set = pkb.usesKB.getAllVarsUsedByStmt(id);
			result.insert(set.begin(), set.end());
		}
		return result;
	}

	std::unordered_set<VarId> Parser::getAllModifies(StmtListId sid) {
		std::unordered_set<VarId> result;
		StatementList sl = pkb.stmtListTable.get(sid);
		std::vector<StmtId> idList = sl.getStmtIds();
		for (StmtId id : idList) {
			std::unordered_set<VarId> set = pkb.modifiesKB.getAllVarsModifiedByStmt(id);
			result.insert(set.begin(), set.end());
		}
		return result;
	}

	void Parser::populateParentKB(StmtId stmtId, StmtListId stmtLstId) {
		StatementList sl = pkb.stmtListTable.get(stmtLstId);
		std::vector<StmtId> idList = sl.getStmtIds();
		for (StmtId id : idList) {
			pkb.parentKB.addParent(stmtId, id);
		}
	}

	void Parser::populateUsesKB(StmtId stmtId, std::unordered_set<VarId> varSet) {
		for (VarId id : varSet) {
			pkb.usesKB.addStmtUses(stmtId, id);
		}
	}

	void Parser::populateModifiesKB(StmtId stmtId, std::unordered_set<VarId> varSet) {
		for (VarId id : varSet) {
			pkb.modifiesKB.addStmtModifies(stmtId, id);
		}
	}

	void Parser::populatePatternKB(StmtId stmtId, Expression exp) {
		pkb.patternKB.addRHSPattern(exp.getStr(), stmtId);
		std::unordered_set<Pattern> patterns = exp.getPatterns();
		for (Pattern p : patterns) {
			pkb.patternKB.addRHSPattern(p, stmtId);
		}
	}
}