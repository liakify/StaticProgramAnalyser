#ifndef TYPES_H
#define TYPES_H

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

using VarName = std::string;
using ConstValue = std::string;

#endif