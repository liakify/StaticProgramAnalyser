#pragma once

#include <stdexcept>
#include <regex>
#include <stack>
#include "PKB.h"
#include "Simple.h"

using namespace SIMPLE;

namespace FrontEnd {
	/**
	* The Parser class is used for parsing SIMPLE source code.
	*/
	class Parser {
	public:
		/**
		* Parses full SIMPLE source and populates PKB accordingly.
		* 
		* @param	source					SIMPLE program string for parsing.
		* @return	PKB::PKB				A populated PKB instance
		* @throws	std::invalid_argument	if the SIMPLE source has syntax errors.
		*/
		PKB::PKB parseSimple(std::string source);

		/**
		* Parses standalone SIMPLE expressions into a consistent string format.
		* PKB remains unmodified.
		* 
		* @param	exp						SIMPLE expression to be parsed.
		* @return	std::string				A converted string representation
		* @throws	std::invalid_argument	if the SIMPLE source has syntax errors.
		*/
		std::string parseExpression(std::string exp);
	private:
		std::string src;
		PKB::PKB pkb;
		size_t pos;
		bool isExpression;

		std::string consume(std::regex rgx);

		std::string name();
		std::string integer();

		void program();
		ProcId procedure();
		StmtListId stmtLst();
		StmtId stmt();
		ReadStmt* read_stmt();
		PrintStmt* print_stmt();
		CallStmt* call_stmt();
		WhileStmt* while_stmt();
		IfStmt* if_stmt();
		AssignStmt* assign_stmt();

		CondExpr cond_expr();
		CondExpr rel_expr();

		int get_op_rank(char op);
		int compare_op(char op1, char op2); 
		void combine_op(std::stack<Expression>& operands, std::stack<char>& operators);
		Expression expr();
		Expression factor();

		VarId var_name();
		ProcName proc_name();
		ConstId const_value();

		std::unordered_set<VarId> getAllUses(StmtListId sid);
		std::unordered_set<VarId> getAllModifies(StmtListId sid);

		void populateParentKB(StmtId stmtId, StmtListId stmtLstId);
		void populateUsesKB(StmtId stmtId, std::unordered_set<VarId> varSet);
		void populateModifiesKB(StmtId stmtId, std::unordered_set<VarId> varSet);
		void populatePatternKB(StmtId stmtId, Expression exp);
	};
}
