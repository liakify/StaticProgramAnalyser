#pragma once

#include <cassert>
#include <unordered_map>
#include <vector>

#include "Types.h"

using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

// Additional types specific to PQL (Query Processor)
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
    // Pattern arguments can be WILDCARD, INCLUSIVE_PATTERN, EXACT_PATTERN
    INVALID, UNKNOWN, SYNONYM, INTEGER, IDENTIFIER, WILDCARD, INCLUSIVE_PATTERN, EXACT_PATTERN
};

namespace PQL {
    
    const pair<ArgType, string> INVALID_ARG = { ArgType::INVALID, "" };

    const unordered_map<string, DesignEntity> ENTITY_MAP {
        {"statement", DesignEntity::STATEMENT},
        {"read", DesignEntity::READ},
        {"print", DesignEntity::PRINT},
        {"call", DesignEntity::CALL},
        {"while", DesignEntity::WHILE},
        {"if", DesignEntity::IF},
        {"assign", DesignEntity::ASSIGN},
        {"variable", DesignEntity::VARIABLE},
        {"constant", DesignEntity::CONSTANT},
        {"procedure", DesignEntity::PROCEDURE}
    };

    const unordered_map<string, RelationType> RELATION_MAP {
        {"Follows", RelationType::FOLLOWS},
        {"Follows*", RelationType::FOLLOWST},
        {"Parent", RelationType::PARENT},
        {"Parent*", RelationType::PARENTT},
        {"Uses", RelationType::USESS},
        {"Modifies", RelationType::MODIFIESS}
    };

    struct RelationClause {
        string clause;
        RelationType type;
        // StatementRef, EntityRef are just strings
        // Use ArgType to determine how to interpret them
        pair<ArgType, StmtRef> firstStmt;
        pair<ArgType, StmtRef> secondStmt;
        pair<ArgType, EntityRef> firstEnt;
        pair<ArgType, EntityRef> secondEnt;
        pair<pair<ArgType, string>, pair<ArgType, string>> getArgs() {
            switch (type) {
            case RelationType::FOLLOWS:
            case RelationType::FOLLOWST:
            case RelationType::PARENT:
            case RelationType::PARENTT:
                return { firstStmt, secondStmt };
                break;
            case RelationType::USESS:
            case RelationType::MODIFIESS:
                return { firstStmt, secondEnt };
                break;
            case RelationType::USESP:
            case RelationType::MODIFIESP:
                return { firstEnt, secondEnt };
            default:
                return { INVALID_ARG, INVALID_ARG };
            };
        };
    };

    struct PatternClause {
        string clause;
        PatternType type;
        pair<ArgType, EntityRef> targetArg;
        pair<ArgType, Pattern> patternArg;
        pair<pair<ArgType, string>, pair<ArgType, string>> getArgs() {
            return { targetArg, patternArg };
        };
    };

    struct Query {
        string status;
        string queryString;
        // Entities to return as part of query (BOOLEAN not included yet)
        vector<string> targetEntities;
        // Maps declared synonyms to design entities
        unordered_map<string, DesignEntity> synonymTable;
        vector<RelationClause> relations;
        vector<PatternClause> patterns;
    };
    
}
