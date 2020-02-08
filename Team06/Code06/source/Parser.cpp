//#include "PKB.h"
//#include "TNode.h"
#include "Parser.h"
#include "LoggingUtils.h"
#include <stack>

using std::invalid_argument;

namespace Parser{
	Parser::Parser(string source) {
		this->src = source;
		this->pos = 0;
	}

	string Parser::consume(regex rgx) {
		std::smatch match;
		string str = src.substr(this->pos);
		SPA::LoggingUtils::LogDebugMessage("%d", this->pos);
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

	void Parser::parse() {
		program();
	}

	void Parser::program() {
		while (this->pos < src.length()) {
			procedure();
		}
	}

	Procedure Parser::procedure() {
		consume(regex("[[:space:]]*procedure[[:space:]]+"));
		ProcName pn = proc_name();
		consume(regex("[[:space:]]*[{][[:space:]]*"));
		StatementList sl = stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*"));
		return Procedure(pn, sl);
	}

	StatementList Parser::stmtLst() {
		std::vector<Statement> statements;
		statements.push_back(stmt());
		while (true) {
			try {
				statements.push_back(stmt());
			} catch (invalid_argument& e) {
				break;
			}
		}
		return StatementList(statements);
	}

	Statement Parser::stmt() {
		int currentPos = this->pos;
		try {
			read_stmt();
			return Statement();
		} catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			print_stmt();
			return Statement();
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			call_stmt();
			return Statement();
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			while_stmt();
			return Statement();
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			if_stmt();
			return Statement();
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		assign_stmt();
		return Statement();
	}

	void Parser::read_stmt() {
		consume(regex("[[:space:]]*read[[:space:]]+"));
		var_name();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
	}

	void Parser::print_stmt() {
		consume(regex("[[:space:]]*print[[:space:]]+"));
		var_name();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
	}

	void Parser::call_stmt() {
		consume(regex("[[:space:]]*call[[:space:]]+"));
		proc_name();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
	}

	void Parser::while_stmt() {
		consume(regex("[[:space:]]*while[[:space:]]*[(][[:space:]]*"));
		cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*[{][[:space:]]*"));
		stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*"));
	}

	void Parser::if_stmt() {
		consume(regex("[[:space:]]*if[[:space:]]*[(][[:space:]]*"));
		cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*then[[:space:]]*[{][[:space:]]*"));
		stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*else[[:space:]]*[{][[:space:]]*"));
		stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*"));
	}

	void Parser::assign_stmt() {
		var_name();
		consume(regex("[[:space:]]*[=][[:space:]]*"));
		expr();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
	}

	void Parser::cond_expr() {
		int currentPos = this->pos;
		try {
			rel_expr();
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			consume(regex("[[:space:]]*[!][[:space:]]*[(][[:space:]]*"));
			cond_expr();
			consume(regex("[[:space:]]*[)][[:space:]]*"));
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			consume(regex("[[:space:]]*[(][[:space:]]*"));
			cond_expr();
			consume(regex("[[:space:]]*[)][[:space:]]*(&&)[[:space:]]*[(][[:space:]]*"));
			cond_expr();
			consume(regex("[[:space:]]*[)][[:space:]]*"));
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		consume(regex("[[:space:]]*[(][[:space:]]*"));
		cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*(||)[[:space:]]*[(][[:space:]]*"));
		cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*"));
	}

	void Parser::rel_expr() {
		int currentPos = this->pos;
		try {
			rel_factor();
			consume(regex("[[:space:]]*(>)[[:space:]]*"));
			rel_factor();
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			rel_factor();
			consume(regex("[[:space:]]*(>=)[[:space:]]*"));
			rel_factor();
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			rel_factor();
			consume(regex("[[:space:]]*(<)[[:space:]]*"));
			rel_factor();
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			rel_factor();
			consume(regex("[[:space:]]*(<=)[[:space:]]*"));
			rel_factor();
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			rel_factor();
			consume(regex("[[:space:]]*(==)[[:space:]]*"));
			rel_factor();
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		rel_factor();
		consume(regex("[[:space:]]*(!=)[[:space:]]*"));
		rel_factor();
	}

	void Parser::rel_factor() {
		int currentPos = this->pos;
		try {
			var_name();
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			const_value();
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		expr();
	}

	void Parser::expr() { // Need to redo, recursive descent infinite recursion
		/*int currentPos = this->pos;
		string tokenStr;
		Operand token;
		std::stack<Operand> operands;
		//std::stack<SIMPLE::Operator> operators;
		consume(regex("[[:space:]]*"));
		try {
			tokenStr = var_name();
			token = Variable(tokenStr);
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
			tokenStr = const_value();
			token = Constant(tokenStr);
		}
		consume(regex("[[:space:]]*"));
		
		operands.push(token);*/

		/*try {
			expr();
			consume(regex("[[:space:]]*[+][[:space:]]*"));
			term();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			expr();
			consume(regex("[[:space:]]*[-][[:space:]]*"));
			term();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		term();*/
	}

	void Parser::term() { // Need to redo, recursive descent infinite recursion
		/*int currentPos = pos;
		try {
			term();
			consume(regex("[[:space:]]*[*][[:space:]]*"));
			factor();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			term();
			consume(regex("[[:space:]]*[/][[:space:]]*"));
			factor();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			term();
			consume(regex("[[:space:]]*[%][[:space:]]*"));
			factor();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		factor();*/
	}

	void Parser::factor() {
		int currentPos = this->pos;
		try {
			var_name();
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		try {
			const_value();
			return;
		}
		catch (invalid_argument & e) {
			this->pos = currentPos;
		}
		consume(regex("[[:space:]]*[(][[:space:]]*"));
		expr();
		consume(regex("[[:space:]]*[)][[:space:]]*"));
	}

	VarName Parser::var_name() {
		return name();
	}

	ProcName Parser::proc_name() {
		return name();
	}

	ConstValue Parser::const_value() {
		return integer();
	}

	int analyse(string& src) {
		Parser p = Parser(src);
		p.parse();
		return 0;
	}
}