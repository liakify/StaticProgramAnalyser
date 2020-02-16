#ifndef TYPES_H
#define TYPES_H

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

// Additional types specific to PQL
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

using QueryResultEntry = std::vector<std::string>;
using ClauseResultEntry = std::vector<std::unordered_map<std::string, std::string> >;
using ClauseResult = std::vector<ClauseResultEntry>;
using QueryResult = std::vector<QueryResultEntry>;

#endif
