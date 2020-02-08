#include "PKB.h"
#include "TNode.h"
#include "Parser.h"
#include <iostream>

using std::smatch;
using std::invalid_argument;
using std::cout;
using std::endl;

namespace Parser{
	Parser::Parser(string source) {
		Parser::src = source;
		Parser::pos = 0;
	}

	string Parser::consume(regex rgx) {
		smatch match;
		string str = src.substr(pos);
		cout << pos << endl;
		if (!regex_search(str, match, rgx, std::regex_constants::match_continuous)) {
			throw invalid_argument("Syntax error in SIMPLE source.\n");
		}
		pos += match.length();
		return match.str();
	}

	void Parser::name() {
		cout << consume(regex("[A-Z|a-z][A-Z|a-z|0-9]*"));
	}

	void Parser::integer() {
		cout << consume(regex("[0-9]+"));
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
		consume(regex("[[:space:]]*procedure[[:space:]]+"));
		proc_name();
		consume(regex("[[:space:]]*[{][[:space:]]*"));
		stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*"));
	}

	void Parser::stmtLst() {
		stmt();
		while (true) {
			try {
				stmt();
			} catch (invalid_argument& e) {
				break;
			}
		}
	}

	void Parser::stmt() {
		int currentPos = pos;
		try {
			read_stmt();
			return;
		} catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			print_stmt();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			call_stmt();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			while_stmt();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			if_stmt();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		assign_stmt();
	}

	void Parser::read_stmt() {
		consume(regex("[[:space:]]*read[[:space:]]+"));
		var_name();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
	}

	void Parser::print_stmt() {
		consume(regex("[[:space:]]*print[[:space:]]+"));
		var_name();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
	}

	void Parser::call_stmt() {
		consume(regex("[[:space:]]*call[[:space:]]+"));
		proc_name();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
	}

	void Parser::while_stmt() {
		consume(regex("[[:space:]]*while[[:space:]]*[(][[:space:]]*"));
		cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*[{][[:space:]]*"));
		stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*"));
	}

	void Parser::if_stmt() {
		consume(regex("[[:space:]]*if[[:space:]]*[(][[:space:]]*"));
		cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*then[[:space:]]*[{][[:space:]]*"));
		stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*else[[:space:]]*[{][[:space:]]*"));
		stmtLst();
		consume(regex("[[:space:]]*[}][[:space:]]*"));
	}

	void Parser::assign_stmt() {
		var_name();
		consume(regex("[[:space:]]*[=][[:space:]]*"));
		expr();
		consume(regex("[[:space:]]*[;][[:space:]]*"));
	}

	void Parser::cond_expr() {
		int currentPos = pos;
		try {
			rel_expr();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			consume(regex("[[:space:]]*[!][[:space:]]*[(][[:space:]]*"));
			cond_expr();
			consume(regex("[[:space:]]*[)][[:space:]]*"));
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			consume(regex("[[:space:]]*[(][[:space:]]*"));
			cond_expr();
			consume(regex("[[:space:]]*[)][[:space:]]*(&&)[[:space:]]*[(][[:space:]]*"));
			cond_expr();
			consume(regex("[[:space:]]*[)][[:space:]]*"));
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		consume(regex("[[:space:]]*[(][[:space:]]*"));
		cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*(||)[[:space:]]*[(][[:space:]]*"));
		cond_expr();
		consume(regex("[[:space:]]*[)][[:space:]]*"));
	}

	void Parser::rel_expr() {
		int currentPos = pos;
		try {
			rel_factor();
			consume(regex("[[:space:]]*(>)[[:space:]]*"));
			rel_factor();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			rel_factor();
			consume(regex("[[:space:]]*(>=)[[:space:]]*"));
			rel_factor();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			rel_factor();
			consume(regex("[[:space:]]*(<)[[:space:]]*"));
			rel_factor();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			rel_factor();
			consume(regex("[[:space:]]*(<=)[[:space:]]*"));
			rel_factor();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			rel_factor();
			consume(regex("[[:space:]]*(==)[[:space:]]*"));
			rel_factor();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		rel_factor();
		consume(regex("[[:space:]]*(!=)[[:space:]]*"));
		rel_factor();
	}

	void Parser::rel_factor() {
		int currentPos = pos;
		try {
			var_name();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			const_value();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		expr();
	}

	void Parser::expr() { // Need to redo, recursive descent infinite recursion
		/*int currentPos = pos;
		try {
			expr();
			consume(regex("[[:space:]]*[+][[:space:]]*"));
			term();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			expr();
			consume(regex("[[:space:]]*[-][[:space:]]*"));
			term();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		term();*/
	}

	void Parser::term() { // Need to redo, recursive descent infinite recursion
		/*int currentPos = pos;
		try {
			term();
			consume(regex("[[:space:]]*[*][[:space:]]*"));
			factor();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			term();
			consume(regex("[[:space:]]*[/][[:space:]]*"));
			factor();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			term();
			consume(regex("[[:space:]]*[%][[:space:]]*"));
			factor();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		factor();*/
	}

	void Parser::factor() {
		int currentPos = pos;
		try {
			var_name();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		try {
			const_value();
			return;
		}
		catch (invalid_argument & e) {
			pos = currentPos;
		}
		consume(regex("[[:space:]]*[(][[:space:]]*"));
		expr();
		consume(regex("[[:space:]]*[)][[:space:]]*"));
	}

	void Parser::var_name() {
		name();
		cout << " : VAR\n";
	}

	void Parser::proc_name() {
		name();
		cout << " : PROC\n";
	}

	void Parser::const_value() {
		integer();
		cout << " : CONST\n";
	}

	int analyse(string& src) {
		Parser p = Parser(src);
		p.parse();
		return 0;
	}
}