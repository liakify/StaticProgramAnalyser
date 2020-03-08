#pragma once

#include <cassert>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "Types.h"

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
    FOLLOWS, FOLLOWST, PARENT, PARENTT, USESS, USESP, MODIFIESS, MODIFIESP,
    CALLS, CALLST, NEXT, NEXTT, AFFECTS, AFFECTST
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
     *  All status messages in use by the Query Parser during query parsing and
     *  validation of a PQL query. All but the OK and SUCCESS messages indicate
     *  either a syntax or semantic error with the query.
     */
    const std::string OK = "ok";
    const std::string SUCCESS = "success";
    const std::string SYNTAX_ERR_EMPTY_QUERY = "syntax error: empty query";
    const std::string SYNTAX_ERR_INVALID_DECLARATION = "syntax error: declaration statement has incorrect syntax";
    const std::string SYNTAX_ERR_MISSING_SELECT_OR_UNKNOWN_CHAR = "syntax error: query missing 'Select' keyword or has unrecognised characters";
    const std::string SYNTAX_ERR_INVALID_AND_CHAINED_CLAUSES = "syntax error: query contains incorrect use of clause keywords and 'and'";
    const std::string SYNTAX_ERR_UNKNOWN_DESIGN_ENTITY_KEYWORD = "syntax error: unrecognised design entity keyword in declaration";
    const std::string SEMANTIC_ERR_CONFLICTING_SYNONYM_DECLARATIONS = "semantic error: conflicting synonym declarations";
    const std::string SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET = "syntax error: missing query target or target entities not correctly specified";
    const std::string SYNTAX_ERR_MISSING_OR_MALFORMED_PATTERN_ARG = "syntax error: pattern clause has missing or malformed argument";
    const std::string SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY = "syntax error: compound clauses in query body violate query body syntax";
    const std::string SYNTAX_ERR_INVALID_RELATION_KEYWORD = "syntax error: invalid relation keyword";
    const std::string SYNTAX_ERR_RELATION_INVALID_NUM_ARGS = "syntax error: relations only accept 2 arguments";
    const std::string SYNTAX_ERR_FOLLOWS_PARENTS_INVALID_STMT_REF = "syntax error: invalid statement reference in Follows(*)/Parent(*) clause";
    const std::string SYNTAX_ERR_USES_MODIFIES_INVALID_SECOND_ENT_REF = "syntax error: invalid entity reference as second arg in Uses/Modifies clause";
    const std::string SYNTAX_ERR_USES_MODIFIES_INVALID_FIRST_ARG = "syntax error: invalid first arg in Uses/Modifies clause";
    const std::string SYNTAX_ERR_CALLS_INVALID_ENT_REF = "syntax error: invalid entity reference in Calls(*) clause";
    const std::string SYNTAX_ERR_NEXT_INVALID_LINE_REF = "syntax error: invalid line reference in Next(*) clause";
    const std::string SYNTAX_ERR_AFFECTS_INVALID_STMT_REF = "syntax error: invalid statement reference in Affects(*) clause";
    const std::string FATAL_MISSING_RELATION_HANDLER = "internal error: failed to match relation type";
    const std::string SEMANTIC_ERR_UNDECLARED_PATTERN_TYPE_SYNONYM = "semantic error: undeclared synonym for type of pattern clause";
    const std::string SYNTAX_ERR_PATTERN_INVALID_ENT_REF = "syntax error: invalid entity reference as first arg in pattern clause";
    const std::string SYNTAX_ERR_ASSIGN_PATTERN_INVALID_NUM_ARGS = "syntax error: assign pattern does not have two arguments";
    const std::string SYNTAX_ERR_ASSIGN_PATTERN_INVALID_PATTERN = "syntax error: assign pattern has invalid pattern string";
    const std::string SYNTAX_ERR_WHILE_PATTERN_INVALID_NUM_ARGS = "syntax error: while pattern does not have two arguments";
    const std::string SYNTAX_ERR_WHILE_PATTERN_INVALID_SECOND_ARG = "syntax error: while pattern only supports '_' as second argument";
    const std::string SYNTAX_ERR_IF_PATTERN_INVALID_NUM_ARGS = "syntax error: if pattern does not have three arguments";
    const std::string SYNTAX_ERR_IF_PATTERN_INVALID_SECOND_THIRD_ARG = "syntax error: if pattern only supports '_' for last two arguments";
    const std::string SYNTAX_ERR_INVALID_PATTERN_TYPE = "syntax error: pattern clauses only defined for assign, if, while";
    const std::string SEMANTIC_ERR_AMBIGUOUS_USE_OF_BOOLEAN = "semantic error: ambiguous use of BOOLEAN as both synonym and return type";
    const std::string SEMANTIC_ERR_UNDECLARED_SYNONYM_IN_RETURN_TYPE = "semantic error: undeclared synonym part of query return type";
    const std::string SEMANTIC_ERR_USES_MODIFIES_AMBIGUOUS_WILDCARD = "semantic error: wildcard not accepted as first arg for Uses/Modifies";
    const std::string SEMANTIC_ERR_USES_MODIFIES_UNDECLARED_FIRST_SYNONYM = "semantic error: undeclared synonym as first arg in Uses/Modifies clause";
    const std::string SEMANTIC_ERR_USES_INVALID_FIRST_SYNONYM = "semantic error: relation not defined for synonym as first arg in Uses clause";
    const std::string SEMANTIC_ERR_MODIFIES_INVALID_FIRST_SYNONYM = "semantic error: relation not defined for synonym as first arg in Modifies clause";
    const std::string SEMANTIC_ERR_USES_MODIFIES_UNDECLARED_SECOND_SYNONYM = "semantic error: undeclared synonym as second arg in Uses/Modifies clause";
    const std::string SEMANTIC_ERR_USES_MODIFIES_NON_VARIABLE_SECOND_SYNONYM = "semantic error: synonym as second arg in Uses/Modifies clause not a VARIABLE";
    const std::string SEMANTIC_ERR_CALLS_UNDECLARED_SYNONYM = "semantic error: undeclared synonym in Calls(*) clause";
    const std::string SEMANTIC_ERR_CALLS_NON_PROCEDURE_SYNONYM = "semantic error: synonym in Calls(*) clause not a PROCEDURE";
    const std::string SEMANTIC_ERR_FPNA_NON_POSITIVE_STMT_NUMBER = "semantic error: statement number in F(*)/P(*)/N(*)/A(*) clause must be positive";
    const std::string SEMANTIC_ERR_FPNA_UNDECLARED_SYNONYM = "semantic error: undeclared synonym in F(*)/P(*)/N(*)/A(*) clause";
    const std::string SEMANTIC_ERR_AFFECTS_NON_ASSIGN_SYNONYM = "semantic error: synonym in Affects(*) clause not an ASSIGN or its super-types";
    const std::string SEMANTIC_ERR_FPN_NON_STMT_SYNONYM = "semantic error: synonym in F(*)/P(*)/N(*) clause not a STATEMENT or its sub-types";
    const std::string SEMANTIC_ERR_PATTERN_UNDECLARED_FIRST_SYNONYM = "semantic error: undeclared synonym as first arg in pattern clause";
    const std::string SEMANTIC_ERR_PATTERN_NON_VARIABLE_FIRST_SYNONYM = "semantic error: synonym as first arg in pattern clause not a VARIABLE";
    
    /**
     *  Sentinel argument pair that should never be returned by getArgs() method
     *  of a RelationClause or PatternClause instance. Used to denote an argument
     *  that does not apply to that relation or pattern type.
     */
    const std::pair<ArgType, std::string> INVALID_ARG = { ArgType::INVALID, "" };

    /**
     *  Vector of invalid design entity types as arguments for the relations
     *  Follows(*), Parent(*), Next(*).
     */
    const std::vector<DesignEntity> NON_STMTS = {
        DesignEntity::CONSTANT, DesignEntity::PROCEDURE, DesignEntity::VARIABLE
    };

    /**
     *  Vector of invalid design entity types as the first argument for the
     *  statement variant of the Uses relation.
     */
    const std::vector<DesignEntity> NON_USES = {
        DesignEntity::CONSTANT, DesignEntity::PROCEDURE,
        DesignEntity::READ, DesignEntity::VARIABLE
    };

    /**
     *  Vector of invalid design entity types as the first argument for the
     *  statement variant of the Modifies relation.
     */
    const std::vector<DesignEntity> NON_MODIFIES = {
        DesignEntity::CONSTANT, DesignEntity::PRINT,
        DesignEntity::PROCEDURE, DesignEntity::VARIABLE
    };

    /**
     *  Vector of invalid design entity types as arguments for the Affects(*) relation.
     */
    const std::vector<DesignEntity> NON_AFFECTS = {
        DesignEntity::CALL, DesignEntity::CONSTANT, DesignEntity::IF,
        DesignEntity::PRINT, DesignEntity::PROCEDURE, DesignEntity::READ,
        DesignEntity::VARIABLE, DesignEntity::WHILE
    };

    /**
     *  Map from program entity keyword to program entity enum.
     */
    const std::unordered_map<std::string, DesignEntity> ENTITY_MAP {
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
    const std::unordered_map<std::string, RelationType> RELATION_MAP {
        {"Follows", RelationType::FOLLOWS},
        {"Follows*", RelationType::FOLLOWST},
        {"Parent", RelationType::PARENT},
        {"Parent*", RelationType::PARENTT},
        {"Uses", RelationType::USESS},
        {"Modifies", RelationType::MODIFIESS},
        {"Calls", RelationType::CALLS},
        {"Calls*", RelationType::CALLST},
        {"Next", RelationType::NEXT},
        {"Next*", RelationType::NEXTT},
        {"Affects", RelationType::AFFECTS},
        {"Affects*", RelationType::AFFECTST}
    };

    /**
     *  Struct representing a parsed relation clause in a PQL query. Contains the
     *  clause string, relation type and method to retrieve its arguments. Arguments
     *  are returned as pairs of ArgType and the argument string.
     */
    struct RelationClause {
        std::string clause;
        RelationType type;
        // StatementRef, EntityRef are just strings
        // Use ArgType to determine how to interpret them
        std::pair<ArgType, StmtRef> firstStmt;
        std::pair<ArgType, StmtRef> secondStmt;
        std::pair<ArgType, EntityRef> firstEnt;
        std::pair<ArgType, EntityRef> secondEnt;
        std::pair<std::pair<ArgType, std::string>, std::pair<ArgType, std::string>> getArgs() {
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
     *  clause string, pattern type, synonym and method to retrieve its arguments.
     *  Arguments are returned as pairs of ArgType and the argument string.
     */
    struct PatternClause {
        std::string clause;
        PatternType type;
        std::string synonym;
        std::pair<ArgType, EntityRef> targetArg;
        std::pair<ArgType, Pattern> patternArg;
        std::pair<std::pair<ArgType, std::string>, std::pair<ArgType, std::string>> getArgs() {
            return { targetArg, patternArg };
        };
    };

    /**
     *  Struct representing a parsed PQL query. Contains the following fields:

     *  - status:           status message from evaluation by the Query Parser.
     *  - queryString:      the full query string.
     *  - returnsBool:      boolean describing if this query returns a BOOLEAN.
     *  - targetEntities:   vector of all design entities (by synonym) to return for the query.
     *  - synonymTable:     a mapping of declared synonyms to design entities.
     *  - relations:        a vector of parsed relation clauses.
     *  - patterns:         a vector of parsed pattern clauses.
     *
     *  Note however that the BOOLEAN return type is not supported yet.
     */
    struct Query {
        std::string status;
        std::string queryString;
        bool returnsBool;
        std::vector<std::string> targetEntities;
        std::unordered_map<std::string, DesignEntity> synonymTable;
        std::vector<RelationClause> relations;
        std::vector<PatternClause> patterns;
    };
    
}
