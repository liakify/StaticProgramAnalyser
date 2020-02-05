#include "PKB.h"
#include "TNode.h"
#include "Parser.h"

namespace Parser{
	Parser::Parser(string source) {
		Parser::src = source;
		Parser::pos = 0;
	}

	string Parser::consume(regex rgx) {
		smatch match;
		string str = src.substr(pos);
		if (!regex_search(str, match, rgx, regex_constants::match_continuous)) {
			throw invalid_argument("Syntax error in SIMPLE source.\n");
		}
		pos += match.length();
		return match.str();
	}

	void Parser::parse() {
		program();
	}

	void Parser::program() {
		while (pos < src.length()) {
			procedure();
		}
	}

	void Parser::procedure() {
		consume(regex("[[:space:]]*procedure[[:space:]]+"));
		proc_name();
		consume(regex("[[:space:]]*[{][[:space:]]*"));
		stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*"));
	}

	void Parser::proc_name() {
		cout << "PROC: ";
		name();
	}

	void Parser::name() {
		cout << consume(regex("[A-Z|a-z][A-Z|a-z|0-9]*")) << endl;
	}

	void Parser::stmtLst() {
		stmt();
		while (true) {
			try {
				stmt();
			} catch (invalid_argument& e) {
				break;
			}
		}
	}

	void Parser::stmt() {
		int currentPos = pos;
		try {
			read_stmt();
			return;
		} catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			print_stmt();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			call_stmt();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			while_stmt();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			if_stmt();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		assign_stmt();
	}

	void Parser::read_stmt() {
		consume(regex("[[:space:]]*read[[:space:]]+"));
		var_name();
		consume(regex("[[:space:]]*;[[:space:]]*"));
	}

	void Parser::print_stmt() {
		consume(regex("[[:space:]]*print[[:space:]]+"));
		var_name();
		consume(regex("[[:space:]]*;[[:space:]]*"));
	}

	void Parser::call_stmt() {
		consume(regex("[[:space:]]*call[[:space:]]+"));
		proc_name();
		consume(regex("[[:space:]]*;[[:space:]]*"));
	}

	void Parser::while_stmt() {
		consume(regex("[[:space:]]*while[[:space:]]+"));
	}

	void Parser::if_stmt() {
		consume(regex("[[:space:]]*if[[:space:]]+"));
	}

	void Parser::assign_stmt() {
		consume(regex("[[:space:]]*assign[[:space:]]+"));
	}

	void Parser::var_name() {
		cout << "VAR: ";
		name();
	}

	int analyse(string& src) {
		Parser p = Parser(src);
		p.parse();
		return 0;
	}
}