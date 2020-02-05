#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <stdexcept>
#include <regex>

using namespace std;

namespace Parser {

	class Parser {
	public:
		Parser(string source);
		void parse();
	private:
		string src;
		size_t pos;

		string consume(regex rgx);

		void program();
		void procedure();
		void proc_name();
		void name();
		void stmtLst();
		void stmt();
		void readStmt();
		void printStmt();
		void whileStmt();
	};
	int analyse(string& src);
}
#endif
