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

	Variable::Variable(VarName varName) {
		this->varName = varName;
	}

	Constant::Constant(ConstValue value) {
		this->value = value;
	}
}
