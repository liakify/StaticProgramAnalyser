#pragma once

#include <string>
#include <vector>
#include <unordered_map>

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

using Constant = int;
using StmtRef = std::string;
using EntityRef = std::string;

using Synonym = std::string;

using QueryResultEntry = std::vector<std::string>;
using ClauseResultEntry = std::unordered_map<std::string, std::string>;
using ClauseResult = std::vector<ClauseResultEntry>;
using QueryResult = std::vector<QueryResultEntry>;