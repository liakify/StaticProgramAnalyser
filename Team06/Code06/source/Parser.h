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
	/**
	* The Parser class is used for parsing SIMPLE source code.
	*/
	class Parser {
	public:
		/**
		* Parser constructor.
		* @param	source	SIMPLE program string for parsing
		* @param	pkb		PKB reference for use when parsing
		*/
		Parser(string source, PKB::PKB& pkb);
		
		/**
		* Parses source and populates pkb accordingly.
		* @throws	std::invalid_argument if the SIMPLE source has syntax errors.
		*/
		void parse();
	private:
		string src;
		PKB::PKB pkb;
		size_t pos;

		string consume(regex rgx);

		string name();
		string integer();

		void program();
		ProcId procedure();
		StmtListId stmtLst();
		StmtId stmt();
		ReadStmt read_stmt();
		PrintStmt print_stmt();
		CallStmt call_stmt();
		WhileStmt while_stmt();
		IfStmt if_stmt();
		AssignStmt assign_stmt();

		CondExpr cond_expr();
		CondExpr rel_expr();
		Expression rel_factor();

		int get_op_rank(char op);
		int compare_op(char op1, char op2);
		Expression expr();
		Expression factor();

		VarId var_name();
		ProcName proc_name();
		ConstValue const_value();

		void populateParentKB(StmtId stmtId, StmtListId stmtLstId);
	};
	int analyse(string& src);
}
#endif
