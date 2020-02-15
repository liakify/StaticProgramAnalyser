#pragma once

#include <string>

enum StmtType
{
	PRINT, READ, IF, WHILE, CALL, ASSIGN
};

using VarName = std::string;
using VarId = int;

using ProcId = int;
using ProcName = std::string;

using StmtId = int;
using StmtListId = int;

using ConstValue = std::string;
using Pattern = std::string;
