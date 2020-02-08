#include "Simple.h"
#include "LoggingUtils.h"

namespace SIMPLE {
	StatementList::StatementList(std::vector<Statement>& statements) 
		: statements(statements) {
	}

	Procedure::Procedure(ProcName procName, StatementList statements)
		: statements(statements), procName(procName) {
	}

	bool Procedure::operator== (const Procedure& p) {
		return procName == p.procName;
	}

	Operand::Operand(Operand left, Operand right, char op) 
		: left(&left), right(&right), op(op) {
		this->str = getStr();
	}

	Operand::Operand(std::string name)
		: left(nullptr), right(nullptr), op('\0') {
		this->str = name;
	}

	std::string Operand::getStr() {
		if (this->str != "") {
			return this->str;
		}
		else {
			return "(" + this->left->getStr() + std::string(1, this->op) + this->right->getStr() + ")";
		}
	}
}
