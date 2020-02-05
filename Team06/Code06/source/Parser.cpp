#include "PKB.h"
#include "TNode.h"
#include "Parser.h"

namespace Parser{
	Parser::Parser(string source) {
		Parser::src = source;
		Parser::pos = 0;
	}

	void Parser::expect(string s) {
		size_t len = s.length();
		if (src.substr(pos, len).compare(s) != 0) {
			throw invalid_argument("Syntax error in SIMPLE source.\n");
		}
		pos += len;
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
		expect("procedure");
	}

	int analyse(string& src) {
		Parser p = Parser(src);
		p.parse();
		return 0;
	}
}