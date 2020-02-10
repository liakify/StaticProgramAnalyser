#pragma once

#include "Simple.h"

using namespace SIMPLE;

namespace SimpleEntities {
	ProcName PROC_NAME = "abc";
	VarName VAR_NAME = "x";
	ConstValue CONST_VALUE = "123";
	
	Operand OPERAND = Operand(CONST_VALUE);

	PrintStmt PRINT_STMT = PrintStmt(VAR_NAME);
	ReadStmt READ_STMT = ReadStmt(VAR_NAME);
	AssignStmt ASSIGN_STMT = AssignStmt(VAR_NAME, OPERAND);
	CallStmt CALL_STMT = CallStmt(PROC_NAME);

	std::vector<Statement> stmt_vector = std::vector<Statement>(1, PRINT_STMT);
	StatementList STMTLST = std::vector<Statement>(stmt_vector);

	IfStmt IF_STMT = IfStmt(STMTLST, STMTLST);
	WhileStmt WHILE_STMT = WhileStmt(STMTLST);
}