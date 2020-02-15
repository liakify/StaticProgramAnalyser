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
using StmtRef = std::string;
using EntityRef = std::string;

enum class DesignEntity {
    STATEMENT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
};

enum class RelationType {
    FOLLOWS, FOLLOWST, PARENT, PARENTT, USESS, USESP, MODIFIESS, MODIFIESP
};

enum class PatternType {
    ASSIGN_PATTERN, IF_PATTERN, WHILE_PATTERN
};

enum class ArgType {
    // INVALID used when argument does not apply for that relation
    // UNKNOWN used for arguments that require further validation
    // StmtRef arguments can be SYNONYM, INTEGER or WILDCARD
    // EntityRef arguments can be SYNONYM, IDENTIFIER or WILDCARD
    INVALID, UNKNOWN, SYNONYM, INTEGER, IDENTIFIER, WILDCARD
};
