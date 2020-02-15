#pragma once

#include <string>

using VarName = std::string;
using VarId = int;

using ProcId = int;
using ProcName = std::string;

using StmtId = int;
using StmtType = std::string;
using StmtListId = int;

// Additional types specific to PQL
using Constant = int;
using StatementRef = std::string;
using EntityRef = std::string;

enum DesignEntity {
    STATEMENT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
};

enum RelationType {
    FOLLOWS, FOLLOWST, PARENT, PARENTT, USESS, USESP, MODIFIESS, MODIFIESP
};

enum PatternType {
    ASSIGN_PATTERN, IF_PATTERN, WHILE_PATTERN
};
