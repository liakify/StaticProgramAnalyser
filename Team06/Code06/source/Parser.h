#ifndef PARSER_H
#define PARSER_H

#include <stdexcept>
#include <regex>
#include "Simple.h"

using std::regex;
using std::string;
using namespace SIMPLE;

namespace Parser {

	class Parser {
	public:
		Parser(string source);
		void parse();
	private:
		string src;
		size_t pos;

		string consume(regex rgx);

		string name();
		string integer();

		void program();
		Procedure procedure();
		StatementList stmtLst();
		Statement stmt();
		void read_stmt();
		void print_stmt();
		void call_stmt();
		void while_stmt();
		void if_stmt();
		void assign_stmt();

		void cond_expr();
		void rel_expr();
		void rel_factor();

		int get_op_rank(char op);
		int compare_op(char op1, char op2);
		Operand expr();
		Operand factor();

		VarName var_name();
		ProcName proc_name();
		ConstValue const_value();
	};
	int analyse(string& src);
}
#endif
