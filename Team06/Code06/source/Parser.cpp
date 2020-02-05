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
		cout << str << endl;
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
		cout << consume(regex("procedure[[:space:]]+")) << endl;
		proc_name();
	}

	void Parser::proc_name() {
		name();
	}

	void Parser::name() {
		cout << consume(regex("[A-Z|a-z][A-Z|a-z|0-9]*")) << endl;
	}

	int analyse(string& src) {
		Parser p = Parser(src);
		p.parse();
		return 0;
	}
}