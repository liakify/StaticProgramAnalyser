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
		void read_stmt();
		void print_stmt();
		void call_stmt();
		void while_stmt();
		void if_stmt();
		void assign_stmt();
		void var_name();
	};
	int analyse(string& src);
}
#endif
