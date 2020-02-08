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
	}

	Operand::Operand() {
		this->left = nullptr;
		this->right = nullptr;
		this->op = '\0';
	}

	Variable::Variable(VarName varName) {
		this->varName = varName;
	}

	Constant::Constant(ConstValue value) {
		this->value = value;
	}
}
