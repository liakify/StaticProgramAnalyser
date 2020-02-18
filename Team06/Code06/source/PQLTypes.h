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

/**
 *  Enumeration for recognised program design entities in PQL queries.
 */
enum class DesignEntity {
    STATEMENT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE, PROG_LINE
};

/**
 *  Enumeration for recognised relation types in PQL queries.
 *
 *  Note that FOLLOWS, FOLLOWST, PARENT, PARENTT are statement-statement relations,
 *  and thus restricted to STATEMENT, READ, PRINT, CALL, WHILE, IF, ASSIGN design
 *  entities as arguments.
 *
 *  For all of USESS, MODIFIES, USESP, MODIFIESP, the relation involves a variable,
 *  and thus the second argument is restricted to only the VARIABLE design entity.
 *
 *  Note that USESS, MODIFIESS are statement-variable relations, and thus the first
 *  argument is restricted to the STATEMENT design entity or its sub-types.
 *
 *  Note that USESP, MODIFIESP are procedure-variable relations, and thus the first
 *  argument is restricted to the PROCEDURE design entity.
 */
enum class RelationType {
    FOLLOWS, FOLLOWST, PARENT, PARENTT, USESS, USESP, MODIFIESS, MODIFIESP
};

/**
 *  Enumeration for recognised pattern clause types in PQL queries, identified by
 *  design entity. Only valid for ASSIGN, WHILE and IF design entities.
 */
enum class PatternType {
    ASSIGN_PATTERN, IF_PATTERN, WHILE_PATTERN
};

/**
 *  Enumeration to convey argument types of clauses in PQL queries, identified by
 *  parsing and validation.
 *
 *  Note the following usage conditions:
 *  - INVALID is a sentinel value used to denote that argument does not apply for
 *    a given relation.
 *  - UNKNOWN is a sentinel value used to denote an argument that requires further
 *    validation.
 *  - StmtRef args in the PQL grammar can be one of: SYNONYM, INTEGER, WILDCARD.
 *  - EntityRef args in the PQL grammar can be one of: SYNONYM, IDENTIFIER, WILDCARD.
 *  - Pattern args in the PQL grammar can be one of: WILDCARD, INCLUSIVE_PATTERN,
 *    EXACT_PATTERN.
 */
enum class ArgType {
    INVALID, UNKNOWN, SYNONYM, INTEGER, IDENTIFIER, WILDCARD, INCLUSIVE_PATTERN, EXACT_PATTERN
};

namespace PQL {
    
    /**
     *  Sentinel argument pair that should never be returned by getArgs() method
     *  of a RelationClause or PatternClause instance. Used to denote an argument
     *  that does not apply to that relation or pattern type.
     */
    const pair<ArgType, string> INVALID_ARG = { ArgType::INVALID, "" };

    /**
     *  Map from program entity keyword to program entity enum.
     */
    const unordered_map<string, DesignEntity> ENTITY_MAP {
        {"stmt", DesignEntity::STATEMENT},
        {"read", DesignEntity::READ},
        {"print", DesignEntity::PRINT},
        {"call", DesignEntity::CALL},
        {"while", DesignEntity::WHILE},
        {"if", DesignEntity::IF},
        {"assign", DesignEntity::ASSIGN},
        {"variable", DesignEntity::VARIABLE},
        {"constant", DesignEntity::CONSTANT},
        {"procedure", DesignEntity::PROCEDURE},
        {"prog_line", DesignEntity::PROG_LINE}
    };

    /**
     *  Map from relation keyword to relation type enum.
     *
     *  Note that there are no unique keywords for the procedure-variable variants
     *  of the Uses and Modifies relations, since the variant is only determined
     *  at time of semantic validation by examining the program entity types of
     *  the supplied arguments.
     */
    const unordered_map<string, RelationType> RELATION_MAP {
        {"Follows", RelationType::FOLLOWS},
        {"Follows*", RelationType::FOLLOWST},
        {"Parent", RelationType::PARENT},
        {"Parent*", RelationType::PARENTT},
        {"Uses", RelationType::USESS},
        {"Modifies", RelationType::MODIFIESS}
    };

    /**
     *  Struct representing a parsed relation clause in a PQL query. Contains the
     *  clause string, relation type and method to retrieve its arguments. Arguments
     *  are returned as pairs of ArgType and the argument string.
     */
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

    /**
     *  Struct representing a parsed pattern clause in a PQL query. Contains the
     *  clause string, pattern type and method to retrieve its arguments. Arguments
     *  are returned as pairs of ArgType and the argument string.
     */
    struct PatternClause {
        string clause;
        PatternType type;
        pair<ArgType, EntityRef> targetArg;
        pair<ArgType, Pattern> patternArg;
        pair<pair<ArgType, string>, pair<ArgType, string>> getArgs() {
            return { targetArg, patternArg };
        };
    };

    /**
     *  Struct representing a parsed PQL query. Contains the following fields:

     *  - status:           status message from evaluation by the Query Parser.
     *  - queryString:      the full query string.
     *  - targetEntities:   a vector of all design entities to return for the query.
     *  - synonymTable:     a mapping of declared synonyms to design entities.
     *  - relations:        a vector of parsed relation clauses.
     *  - patterns:         a vector of parsed pattern clauses.
     *
     *  Note however that the BOOLEAN return type is not supported yet.
     */
    struct Query {
        string status;
        string queryString;
        vector<string> targetEntities;
        unordered_map<string, DesignEntity> synonymTable;
        vector<RelationClause> relations;
        vector<PatternClause> patterns;
    };
    
}
