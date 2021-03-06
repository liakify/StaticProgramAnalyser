#pragma once

#include <string>
#include <unordered_map>
#include <vector>

enum class StmtType {
    PRINT, READ, IF, WHILE, CALL, ASSIGN, INVALID
};

enum class ExprType {
    VAR, CONST
};

enum class NodeType {
    PREDECESSOR, SUCCESSOR
};

using VarName = std::string;
using VarId = int;

using ProcId = int;
using ProcName = std::string;

using StmtId = int;
using StmtListId = int;

using ConstValue = std::string;
using ConstId = int;

using Pattern = std::string;

using Constant = int;
using StmtRef = std::string;
using EntityRef = std::string;

using Synonym = std::string;
