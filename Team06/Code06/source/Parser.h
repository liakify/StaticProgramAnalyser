#ifndef PARSER_H
#define PARSER_H

#include <stdexcept>
#include <regex>
#include "PKB.h"
#include "Simple.h"

using std::regex;
using std::string;
using namespace SIMPLE;

namespace Parser {

	class Parser {
	public:
		Parser(string source, PKB::PKB& pkb);
		void parse();
	private:
		string src;
		PKB::PKB pkb;
		size_t pos;

		string consume(regex rgx);

		string name();
		string integer();

		void program();
		Procedure procedure();
		StatementList stmtLst();
		Statement stmt();
		ReadStmt read_stmt();
		PrintStmt print_stmt();
		CallStmt call_stmt();
		WhileStmt while_stmt();
		IfStmt if_stmt();
		AssignStmt assign_stmt();

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
