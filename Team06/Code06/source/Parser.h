#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <stdexcept>
#include <regex>

using std::regex;
using std::string;

namespace Parser {

	class Parser {
	public:
		Parser(string source);
		void parse();
	private:
		string src;
		size_t pos;

		string consume(regex rgx);

		void name();
		void integer();

		void program();
		void procedure();
		void stmtLst();
		void stmt();
		void read_stmt();
		void print_stmt();
		void call_stmt();
		void while_stmt();
		void if_stmt();
		void assign_stmt();

		void cond_expr();
		void rel_expr();
		void rel_factor();

		void expr();
		void term();
		void factor();

		void var_name();
		void proc_name();
		void const_value();
	};
	int analyse(string& src);
}
#endif
