#include "Simple.h"
#include "LoggingUtils.h"

namespace SIMPLE {
	Procedure::Procedure(ProcName procName, StatementList statements) {
		this->procName = procName;
		this->statements = statements;
	}

	bool Procedure::operator== (const Procedure& p) {
		return procName == p.procName;
	}

	Operand::Operand(Operand left, Operand right, char op) {
		this->left = &left;
		this->right = &right;
		this->op = op;
		this->str = getStr();
	}

	Operand::Operand(std::string name) {
		this->left = nullptr;
		this->right = nullptr;
		this->op = '\0';
		this->str = name;
	}

	std::string Operand::getStr() {
		if (this->str != "") {
			return this->str;
		}
		else {
			return this->left->getStr() + std::string(1, this->op) + this->right->getStr();
		}
	}
}
