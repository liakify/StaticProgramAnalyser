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
		consume(regex("[[:space:]]*[{]"));
		stmtLst();
		consume(regex("[[:space:]]*[}]"));
	}

	void Parser::proc_name() {
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
			readStmt();
			return;
		} catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			printStmt();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		whileStmt();
	}

	void Parser::readStmt() {
		consume(regex("[[:space:]]*read[[:space:]]+"));
	}

	void Parser::printStmt() {
		consume(regex("[[:space:]]*print[[:space:]]+"));
	}

	void Parser::whileStmt() {
		consume(regex("[[:space:]]*while[[:space:]]+"));
	}

	int analyse(string& src) {
		Parser p = Parser(src);
		p.parse();
		return 0;
	}
}