#ifndef PARSER_H
#define PARSER_H

#include <stdexcept>
#include <regex>
#include <stack>
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
		* Parses full SIMPLE source and populates PKB accordingly.
		* 
		* @param	source					SIMPLE program string for parsing.
		* @param	pkb					PKB reference for use when parsing.
		* @throws	std::invalid_argument	if the SIMPLE source has syntax errors.
		*/
		void parseSimple(string source, PKB::PKB& pkb);

		/**
		* Parses standalone SIMPLE expressions. PKB remains unmodified.
		* 
		* @throws	std::invalid_argument	if the SIMPLE source has syntax errors.
		*/
		Expression parseExpression(string exp);
	private:
		string src;
		PKB::PKB pkb;
		size_t pos;
		bool isExpression;

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

		int get_op_rank(char op);
		int compare_op(char op1, char op2); 
		void combine_op(std::stack<Expression>& operands, std::stack<char>& operators);
		Expression expr();
		Expression factor();

		VarId var_name();
		ProcName proc_name();
		ConstValue const_value();

		std::unordered_set<VarId> getAllUses(StmtListId sid);
		std::unordered_set<VarId> getAllModifies(StmtListId sid);

		void populateParentKB(StmtId stmtId, StmtListId stmtLstId);
		void populateUsesKB(StmtId stmtId, std::unordered_set<VarId> varSet);
		void populateModifiesKB(StmtId stmtId, std::unordered_set<VarId> varSet);
		void populatePatternKB(StmtId stmtId, Expression exp);
	};
}
#endif
