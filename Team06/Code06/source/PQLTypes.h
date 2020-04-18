#pragma once

#include <cassert>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Types.h"

/**
 *  Enumeration for recognised program design entities in PQL queries.
 */
enum class DesignEntity {
    STATEMENT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE, PROG_LINE
};

/**
 *  Enumeration for recognised attribute types of synonyms appearing as a return
 *  type or an argument in a with clause.
 *
 *  Note that NONE is a sentinel value used to denote a synonym is used without an
 *  attribute type, for arguments that may accept attributes.
 *
 *  Note that for arguments that accept attributes, INVALID is a sentinel value
 *  used to denote an invalid attribute keyword was supplied with a synonym.
 *  Otherwise, INVALID denotes that argument does not accept an attribute.
 */
enum class AttrType {
    INVALID, NONE, PROC_NAME, VAR_NAME, VALUE, STMT_NUM
};

/**
 *  Enumeration for recognised clause types in PQL queries.
 *
 *  This enumeration is required for the Query Optimiser and Query Evaluator to
 *  distinguish and correctly cast Clause references to their child class types,
 *  since the RelationClause, PatternClause and WithClause are derived from the
 *  base Clause class.
 */
enum class ClauseType {
    RELATION, PATTERN, WITH
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
 *
 *  Note that CONTAINS is a procedure-statement relation, hence the first argument
 *  is restricted to the PROCEDURE design entity and the second argument is
 *  restricted to the STATEMENT design entity or its sub-types.
 */
enum class RelationType {
    FOLLOWS, FOLLOWST, PARENT, PARENTT, USESS, USESP, MODIFIESS, MODIFIESP,
    CALLS, CALLST, NEXT, NEXTT, AFFECTS, AFFECTST, CONTAINS
};

/**
 *  Enumeration for recognised pattern clause types in PQL queries, identified by
 *  design entity. Only valid for ASSIGN, WHILE and IF design entities.
 */
enum class PatternType {
    ASSIGN_PATTERN, IF_PATTERN, WHILE_PATTERN
};

/**
 *  Enumeration for recognised with clause types in PQL queries, identified by the
 *  type of value both arguments of the clause evaluate to.
 *
 *  Note that LITERAL_EQUAL is used to denote the special case where both arguments
 *  are literals (integers or identifiers) requiring no further evaluation.
 *
 *  Note that UNKNOWN_EQUAL is a sentinel used to denote the case where the types
 *  of both arguments have not been determined and compared yet.
 */
enum class WithType {
    UNKNOWN_EQUAL, LITERAL_EQUAL, INTEGER_EQUAL, IDENTIFIER_EQUAL
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
 *  - Ref args in the PQL grammar can be one of: INTEGER, IDENTIFIER, SYNONYM,
 *    ATTRIBUTE.
 */
enum class ArgType {
    INVALID, UNKNOWN, SYNONYM, INTEGER, IDENTIFIER, WILDCARD,
    INCLUSIVE_PATTERN, EXACT_PATTERN, ATTRIBUTE
};

namespace PQL {

    const int UNSET_SYNONYM_ID = -1;
    const int NON_SYNONYM_ID = -2;

    /**
     *  All status messages in use by the Query Parser during query parsing and
     *  validation of a PQL query. All but the OK and SUCCESS messages indicate
     *  either a syntax or semantic error with the query.
     */
    const std::string STATUS_OK = "ok";
    const std::string STATUS_SUCCESS = "success";
    const std::string SYNTAX_ERR_EMPTY_QUERY = "syntax error: empty query";
    const std::string SYNTAX_ERR_INVALID_DECLARATION = "syntax error: declaration statement has incorrect syntax";
    const std::string SYNTAX_ERR_MISSING_SELECT_OR_UNKNOWN_CHAR = "syntax error: query missing 'Select' keyword or has unrecognised characters";
    const std::string SYNTAX_ERR_INVALID_AND_CHAINED_CLAUSES = "syntax error: query contains incorrect use of clause keywords and 'and'";
    const std::string SYNTAX_ERR_UNKNOWN_DESIGN_ENTITY_KEYWORD = "syntax error: unrecognised design entity keyword in declaration";
    const std::string SEMANTIC_ERR_CONFLICTING_SYNONYM_DECLARATIONS = "semantic error: conflicting synonym declarations";
    const std::string SYNTAX_ERR_INVALID_ATTRIBUTE_KEYWORD_IN_RETURN_TYPE = "syntax error: invalid design entity attribute keyword in return type";
    const std::string SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET = "syntax error: missing query target or target entities not correctly specified";
    const std::string SYNTAX_ERR_MISSING_OR_MALFORMED_PATTERN_ARG = "syntax error: pattern clause has missing or malformed argument";
    const std::string SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY = "syntax error: compound clauses in query body violate query body syntax";
    const std::string SYNTAX_ERR_INVALID_RELATION_KEYWORD = "syntax error: invalid relation keyword";
    const std::string SYNTAX_ERR_RELATION_INVALID_NUM_ARGS = "syntax error: relation clause does not have two arguments";
    const std::string SYNTAX_ERR_FOLLOWS_PARENTS_INVALID_STMT_REF = "syntax error: invalid statement reference in Follows(*)/Parent(*) clause";
    const std::string SYNTAX_ERR_USES_MODIFIES_INVALID_SECOND_ENT_REF = "syntax error: invalid entity reference as second arg in Uses/Modifies clause";
    const std::string SYNTAX_ERR_USES_MODIFIES_INVALID_FIRST_ARG = "syntax error: invalid first arg in Uses/Modifies clause";
    const std::string SYNTAX_ERR_CALLS_INVALID_ENT_REF = "syntax error: invalid entity reference in Calls(*) clause";
    const std::string SYNTAX_ERR_NEXT_INVALID_LINE_REF = "syntax error: invalid line reference in Next(*) clause";
    const std::string SYNTAX_ERR_AFFECTS_INVALID_STMT_REF = "syntax error: invalid statement reference in Affects(*) clause";
    const std::string SYNTAX_ERR_CONTAINS_INVALID_ENT_REF = "syntax error: invalid entity reference in Contains clause";
    const std::string SYNTAX_ERR_CONTAINS_INVALID_STMT_REF = "syntax error: invalid statement reference in Contains clause";
    const std::string FATAL_MISSING_RELATION_HANDLER = "internal error: failed to match relation type";
    const std::string SEMANTIC_ERR_UNDECLARED_PATTERN_TYPE_SYNONYM = "semantic error: undeclared synonym for type of pattern clause";
    const std::string SYNTAX_ERR_PATTERN_INVALID_ENT_REF = "syntax error: invalid entity reference as first arg in pattern clause";
    const std::string SYNTAX_ERR_ASSIGN_PATTERN_INVALID_NUM_ARGS = "syntax error: assign pattern does not have two arguments";
    const std::string SYNTAX_ERR_ASSIGN_PATTERN_INVALID_PATTERN = "syntax error: assign pattern has invalid pattern string";
    const std::string SYNTAX_ERR_WHILE_PATTERN_INVALID_NUM_ARGS = "syntax error: while pattern does not have two arguments";
    const std::string SYNTAX_ERR_WHILE_PATTERN_INVALID_SECOND_ARG = "syntax error: while pattern only supports '_' as second arg";
    const std::string SYNTAX_ERR_IF_PATTERN_INVALID_NUM_ARGS = "syntax error: if pattern does not have three arguments";
    const std::string SYNTAX_ERR_IF_PATTERN_INVALID_SECOND_THIRD_ARG = "syntax error: if pattern only supports '_' for last two args";
    const std::string SYNTAX_ERR_INVALID_PATTERN_TYPE = "syntax error: pattern clauses only defined for assign, if, while";
    const std::string SYNTAX_ERR_WITH_CLAUSE_INVALID_REF_ARG = "syntax error: invalid reference arg in with (equality) clause";
    const std::string SYNTAX_ERR_WITH_CLAUSE_INVALID_ATTRIBUTE_KEYWORD = "syntax error: invalid attribute keyword in attrRef arg in with (equality) clause";
    const std::string SEMANTIC_ERR_AMBIGUOUS_USE_OF_BOOLEAN = "semantic error: ambiguous use of BOOLEAN as both synonym and return type";
    const std::string SEMANTIC_ERR_UNDECLARED_SYNONYM_IN_RETURN_TYPE = "semantic error: undeclared synonym part of query return type";
    const std::string SEMANTIC_ERR_INVALID_SYNONYM_ATTRIBUTE_IN_RETURN_TYPE = "semantic error: attribute not defined for synonym in query return type";
    const std::string SEMANTIC_ERR_USES_MODIFIES_AMBIGUOUS_WILDCARD = "semantic error: wildcard not accepted as first arg for Uses/Modifies";
    const std::string SEMANTIC_ERR_USES_MODIFIES_UNDECLARED_FIRST_SYNONYM = "semantic error: undeclared synonym as first arg in Uses/Modifies clause";
    const std::string SEMANTIC_ERR_USES_INVALID_FIRST_SYNONYM = "semantic error: relation not defined for synonym as first arg in Uses clause";
    const std::string SEMANTIC_ERR_MODIFIES_INVALID_FIRST_SYNONYM = "semantic error: relation not defined for synonym as first arg in Modifies clause";
    const std::string SEMANTIC_ERR_USES_MODIFIES_UNDECLARED_SECOND_SYNONYM = "semantic error: undeclared synonym as second arg in Uses/Modifies clause";
    const std::string SEMANTIC_ERR_USES_MODIFIES_NON_VARIABLE_SECOND_SYNONYM = "semantic error: synonym as second arg in Uses/Modifies clause not a VARIABLE";
    const std::string SEMANTIC_ERR_CALLS_UNDECLARED_SYNONYM = "semantic error: undeclared synonym as arg in Calls(*) clause";
    const std::string SEMANTIC_ERR_CALLS_NON_PROCEDURE_SYNONYM = "semantic error: synonym as arg in Calls(*) clause not a PROCEDURE";
    const std::string SEMANTIC_ERR_CONTAINS_UNDECLARED_FIRST_SYNONYM = "semantic error: undeclared synonym as first arg in Contains clause";
    const std::string SEMANTIC_ERR_CONTAINS_NON_PROCEDURE_FIRST_SYNONYM = "semantic error: synonym as first arg in Contains clause not a PROCEDURE";
    const std::string SEMANTIC_ERR_CONTAINS_STMT_NUMBER_OVERFLOW = "semantic error: statement number in Contains clause exceeds 32-bit signed integer limit";
    const std::string SEMANTIC_ERR_CONTAINS_NON_POSITIVE_STMT_NUMBER = "semantic error: statement number in Contains clause must be positive";
    const std::string SEMANTIC_ERR_CONTAINS_UNDECLARED_SECOND_SYNONYM = "semantic error: undeclared synonym as second arg in Contains clause";
    const std::string SEMANTIC_ERR_CONTAINS_NON_STATEMENT_SECOND_SYNONYM = "semantic error: synonym as second arg in Contains clause not a STATEMENT or its sub-types";
    const std::string SEMANTIC_ERR_FPNA_STMT_NUMBER_OVERFLOW = "semantic error: statement number in F(*)/P(*)/N(*)/A(*) clause exceeds 32-bit signed integer limit";
    const std::string SEMANTIC_ERR_FPNA_NON_POSITIVE_STMT_NUMBER = "semantic error: statement number in F(*)/P(*)/N(*)/A(*) clause must be positive";
    const std::string SEMANTIC_ERR_FPNA_UNDECLARED_SYNONYM = "semantic error: undeclared synonym as arg in F(*)/P(*)/N(*)/A(*) clause";
    const std::string SEMANTIC_ERR_AFFECTS_NON_ASSIGN_SYNONYM = "semantic error: synonym as arg in Affects(*) clause not an ASSIGN or its super-types";
    const std::string SEMANTIC_ERR_FPN_NON_STATEMENT_SYNONYM = "semantic error: synonym as arg in F(*)/P(*)/N(*) clause not a STATEMENT or its sub-types";
    const std::string SEMANTIC_ERR_PATTERN_UNDECLARED_FIRST_SYNONYM = "semantic error: undeclared synonym as first arg in pattern clause";
    const std::string SEMANTIC_ERR_PATTERN_NON_VARIABLE_FIRST_SYNONYM = "semantic error: synonym as first arg in pattern clause not a VARIABLE";
    const std::string SEMANTIC_ERR_WITH_CLAUSE_DIFF_LITERAL_TYPE = "semantic error: literal values on both sides of with (equality) clause have different type";
    const std::string SEMANTIC_ERR_WITH_CLAUSE_UNDECLARED_SYNONYM_ARG = "semantic error: undeclared synonym as arg in with (equality) clause";
    const std::string SEMANTIC_ERR_WITH_CLAUSE_NON_PROG_LINE_SYNONYM_ARG = "semantic error: synonym arg in with (equality) clause not a PROG_LINE";
    const std::string SEMANTIC_ERR_WITH_CLAUSE_UNDECLARED_SYNONYM_IN_ATTRIBUTE_ARG = "semantic error: undeclared synonym part of attribute arg in with (equality) clause";
    const std::string SEMANTIC_ERR_WITH_CLAUSE_INVALID_SYNONYM_ATTRIBUTE_PAIR_ARG = "semantic error: attribute not defined for synonym part of attribute arg in with (equality) clause";
    const std::string SEMANTIC_ERR_WITH_CLAUSE_DIFF_RETURN_TYPE_OF_ARGS = "semantic error: expressions on both sides of with (equality) clause evaluate to different type";

    /**
     *  Vector of invalid design entity types to reject as statement arguments for
     *  the relations Follows(*), Parent(*), Next(*) and Contains.
     */
    const std::vector<DesignEntity> NON_STMTS = {
        DesignEntity::CONSTANT, DesignEntity::PROCEDURE, DesignEntity::VARIABLE
    };

    /**
     *  Vector of invalid design entity types to reject as the first argument for
     *  the statement variant of the Uses relation.
     */
    const std::vector<DesignEntity> NON_USES = {
        DesignEntity::CONSTANT, DesignEntity::PROCEDURE,
        DesignEntity::READ, DesignEntity::VARIABLE
    };

    /**
     *  Vector of invalid design entity types to reject as the first argument for
     *  the statement variant of the Modifies relation.
     */
    const std::vector<DesignEntity> NON_MODIFIES = {
        DesignEntity::CONSTANT, DesignEntity::PRINT,
        DesignEntity::PROCEDURE, DesignEntity::VARIABLE
    };

    /**
     *  Vector of invalid design entity types to reject as arguments for the
     *  Affects(*) relation.
     */
    const std::vector<DesignEntity> NON_AFFECTS = {
        DesignEntity::CALL, DesignEntity::CONSTANT, DesignEntity::IF,
        DesignEntity::PRINT, DesignEntity::PROCEDURE, DesignEntity::READ,
        DesignEntity::VARIABLE, DesignEntity::WHILE
    };

    /**
     *  Map from attribute type to a vector of valid associated design entities.
     */
    const std::unordered_map<AttrType, std::vector<DesignEntity>> ATTRIBUTE_ENTITY_MAP{
        { AttrType::PROC_NAME, { DesignEntity::PROCEDURE, DesignEntity::CALL } },
        { AttrType::VAR_NAME, { DesignEntity::VARIABLE, DesignEntity::READ, DesignEntity::PRINT } },
        { AttrType::VALUE, { DesignEntity::CONSTANT } },
        { AttrType::STMT_NUM, {
            DesignEntity::STATEMENT,
            DesignEntity::READ,
            DesignEntity::PRINT,
            DesignEntity::CALL,
            DesignEntity::WHILE,
            DesignEntity::IF,
            DesignEntity::ASSIGN
        } }
    };

    /**
     *  Map from program entity keyword to program entity enum.
     */
    const std::unordered_map<std::string, DesignEntity> ENTITY_MAP{
        { "stmt", DesignEntity::STATEMENT },
        { "read", DesignEntity::READ },
        { "print", DesignEntity::PRINT },
        { "call", DesignEntity::CALL },
        { "while", DesignEntity::WHILE },
        { "if", DesignEntity::IF },
        { "assign", DesignEntity::ASSIGN },
        { "variable", DesignEntity::VARIABLE },
        { "constant", DesignEntity::CONSTANT },
        { "procedure", DesignEntity::PROCEDURE },
        { "prog_line", DesignEntity::PROG_LINE }
    };

    /**
     *  Map from design entity attribute keyword to attribute type enum,
     */
    const std::unordered_map<std::string, AttrType> ATTRIBUTE_MAP{
        { "procName", AttrType::PROC_NAME },
        { "varName", AttrType::VAR_NAME },
        { "value", AttrType::VALUE },
        { "stmt#", AttrType::STMT_NUM }
    };

    /**
     *  Map from relation keyword to relation type enum.
     *
     *  Note that there are no unique keywords for the procedure-variable variants
     *  of the Uses and Modifies relations, since the variant is only determined
     *  at time of semantic validation by examining the program entity types of
     *  the supplied arguments.
     */
    const std::unordered_map<std::string, RelationType> RELATION_MAP{
        { "Follows", RelationType::FOLLOWS },
        { "Follows*", RelationType::FOLLOWST },
        { "Parent", RelationType::PARENT },
        { "Parent*", RelationType::PARENTT },
        { "Uses", RelationType::USESS },
        { "Modifies", RelationType::MODIFIESS },
        { "Calls", RelationType::CALLS },
        { "Calls*", RelationType::CALLST },
        { "Next", RelationType::NEXT },
        { "Next*", RelationType::NEXTT },
        { "Affects", RelationType::AFFECTS },
        { "Affects*", RelationType::AFFECTST },
        { "Contains", RelationType::CONTAINS }
    };

    /**
     *  Struct representing a return type of a query. Contains the synonym to be returned,
     *  an integer string to identify the synonym, and the parsed attribute type if any
     *  (otherwise set to AttrType::NONE).
     */
    struct ReturnType {
        std::string synonym;
        int synonymId;
        AttrType attrType;
        bool operator==(const ReturnType& other) const;
        bool operator<(const ReturnType& other) const;
    };

    /**
     *  Struct representing an argument to a clause. Contains the parsed argument type,
     *  argument value (excluding attribute, if any), an integer string to identify synonym
     *  arguments (otherwise set to NON_SYNONYM_ID), and the parsed attribute type for
     *  attribute arguments (set to AttrType::INVALID for arguments not accepting attributes).
     */
    struct Argument {
        ArgType type;
        std::string value;
        int synonymId;
        AttrType attrType;
        bool operator==(const Argument& other) const;
    };

    /**
     *  Class representing a parsed clause of any type in a PQL query. Contains the
     *  clause string and the clause type.
     */
    class Clause {
     public:
        /**
         *  Compares this Clause instance to another Clause instance and returns true if
         *  the values of every attribute is equal in both instances.
         *
         *  @return     true if both Clause instances are equal.
         */
        bool operator==(const Clause& other) const;

        /**
         *  Returns the input string corresponding to this Clause instance.
         *
         *  @return     string representation of this clause.
         */
        std::string asString();

        /**
         *  Returns the boolean describing if this clause has been negated.
         *
         *  @return     true if this clause has the unary negation operator applied to it.
         */
        bool isNegatedClause();

        /**
         *  Returns the clause type of this Clause instance.
         *
         *  @return     ClauseType enum value describing the type of this clause.
         */
        ClauseType getClauseType();

     protected:
        std::string clause;
        bool isNegated;
        ClauseType clauseType;

        /**
         *  Constructs a new Clause instance with its input string, clause type and a boolean
         *  flag describing if this clause is negated.
         */
        Clause(std::string clause, bool isNegated, ClauseType clauseType);
    };

    /**
     *  Class representing a parsed relation clause in a PQL query. Contains the
     *  clause string, relation type and a method to retrieve its arguments.
     */
    class RelationClause : public Clause {
     public:
        /**
         *  Constructs a new RelationClause instance with its input string, relation type, two
         *  positional arguments, and a boolean flag describing if this clause is negated.
         */
        RelationClause(std::string clause, bool isNegated, RelationType type,
            Argument firstArg, Argument secondArg);

        /**
         *  Compares this RelationClause instance to another RelationClause instance and
         *  returns true if the values of every attribute is equal in both instances.
         *
         *  @return     true if both RelationClause instances are equal.
         */
        bool operator==(const RelationClause& other) const;

        /**
         *  Modifies the relation type of this RelationClause instance to the procedure
         *  variant, if its relation type possesses multiple variants, i.e. Uses and Modifies
         *  relations. Additionally updates the positional argument instances to satisfy this
         *  new relation type. Returns true if the modification was successful, otherwise
         *  returns false without modifying the relation type.
         *
         *  @return     true if the relation type was modified to its procedure variant.
         */
        bool setProcedureVariant();

        /**
         *  Sets the synonym ID of the first argument of this RelationClause instance to a
         *  non-negative integer, if the first argument is a synonym and its ID has not been
         *  previously set. Returns true if the update was successful, otherwise returns false
         *  without modifying the synonym ID.
         *
         *  @return     true if the synonym ID of the first argument was updated.
         */
        bool setFirstSynonymId(int synonymId);

        /**
         *  Sets the synonym ID of the second argument of this RelationClause instance to a
         *  non-negative integer, if the second argument is a synonym and its ID has not been
         *  previously set. Returns true if the update was successful, otherwise returns false
         *  without modifying the synonym ID.
         *
         *  @return     true if the synonym ID of the second argument was updated.
         */
        bool setSecondSynonymId(int synonymId);

        /**
         *  Returns the relation type of this RelationClause instance.
         *
         *  @return     RelationType enum value describing the relation type of this clause.
         */
        RelationType getRelationType();

        /**
         *  Returns a pair containing the two positional arguments of this RelationClause
         *  instance.
         *
         *  @return     pair of the first and second arguments of this clause.
         */
        std::pair<Argument, Argument> getArgs();
     private:
        RelationType type;
        Argument firstArg;
        Argument secondArg;
    };

    /**
     *  Class representing a parsed pattern clause in a PQL query. Contains the
     *  clause string, pattern type, pattern synonym argument and a method to
     *  retrieve its arguments.
     */
    class PatternClause : public Clause {
     public:
        /**
         *  Constructs a new PatternClause instance with its input string, pattern type,
         *  pattern synonym argument, entity argument, pattern argument and a boolean flag
         *  describing if this clause is negated.
         */
        PatternClause(std::string clause, bool isNegated, PatternType type, Argument synonymArg,
            Argument targetArg, Argument patternArg);

        /**
         *  Compares this PatternClause instance to another PatternClause instance and
         *  returns true if the values of every attribute is equal in both instances.
         *
         *  @return     true if both PatternClause instances are equal.
         */
        bool operator==(const PatternClause& other) const;

        /**
         *  Sets the synonym ID of the pattern synonym argument of this PatternClause instance
         *  to a non-negative integer, if it has not been previously set. Returns true if the
         *  update was successful, otherwise returns false without modifying the synonym ID.
         *
         *  @return     true if the synonym ID of the pattern synonym argument was updated.
         */
        bool setPatternSynonymId(int synonymId);

        /**
         *  Sets the synonym ID of the first argument of this PatternClause instance to a
         *  non-negative integer, if the first argument is a synonym and its ID has not been
         *  previously set. Returns true if the update was successful, otherwise returns false
         *  without modifying the synonym ID.
         *
         *  @return     true if the synonym ID of the first argument was updated.
         */
        bool setFirstSynonymId(int synonymId);

        /**
         *  Returns the pattern type of this PatternClause instance.
         *
         *  @return     PatternType enum value describing the relation type of this clause.
         */
        PatternType getPatternType();

        /**
         *  Returns the pattern synonym argument of this RelationClause instance.
         *
         *  @return     pattern synonym argument of this clause.
         */
        Argument getSynonym();

        /**
         *  Returns a pair containing the entity argument and the pattern argument of this
         *  PatternClause instance.
         *
         *  @return     pair of the entity and pattern arguments of this clause.
         */
        std::pair<Argument, Argument> getArgs();
     private:
        PatternType type;
        Argument synonymArg;
        Argument targetArg;
        Argument patternArg;
    };

    /**
     *  Class representing a parsed with clause in a PQL query. Contains the clause
     *  string, equality type and a method to retrieve its arguments.
     *
     *  Note that for non-ATTRIBUTE arguments, the optional attribute type is set to
     *  the default value NONE.
     */
    class WithClause : public Clause {
     public:
        /**
         *  Constructs a new WithClause instance with its input string, equality type, two
         *  positional arguments (one for each of the left and right arguments), and a
         *  boolean flag describing if this clause is negated.
         */
        WithClause(std::string clause, bool isNegated, WithType type,
            Argument leftArg, Argument rightArg);

        /**
         *  Compares this WithClause instance to another WithClause instance and returns
         *  true if the values of every attribute is equal in both instances.
         *
         *  @return     true if both WithClause instances are equal.
         */
        bool operator==(const WithClause& other) const;

        /**
         *  Modifies the equality type of this WithClause instance to the given WithType enum
         *  value, if the equality type is currently unknown. Returns true if the modification
         *  was successful, otherwise returns false without modifying the equality type.
         *
         *  @return     true if the equality type was successfully updated.
         */
        bool setWithType(WithType type);

        /**
         *  Sets the synonym ID of the left argument of this WithClause instance to a
         *  non-negative integer, if the left argument is a synonym or attribute and its ID
         *  has not been previously set. Returns true if the update was successful, otherwise
         *  returns false without modifying the synonym ID.
         *
         *  @return     true if the synonym ID of the left argument was updated.
         */
        bool setLeftSynonymId(int synonymId);

        /**
         *  Sets the synonym ID of the right argument of this WithClause instance to a
         *  non-negative integer, if the right argument is a synonym or attribute and its ID
         *  has not been previously set. Returns true if the update was successful, otherwise
         *  returns false without modifying the synonym ID.
         *
         *  @return     true if the synonym ID of the right argument was updated.
         */
        bool setRightSynonymId(int synonymId);

        /**
         *  Returns the equality type of this WithClause instance.
         *
         *  @return     WithType enum value describing the equality type of this clause.
         */
        WithType getWithType();

        /**
         *  Returns a pair containing the left and right arguments of this WithClause
         *  instance.
         *
         *  @return     pair of the left and right arguments of this clause.
         */
        std::pair<Argument, Argument> getArgs();
     private:
        WithType type;
        Argument leftArg;
        Argument rightArg;
    };

    /**
     *  Struct representing a parsed PQL query. Contains the following fields:
     *
     *  - status:           status message from evaluation by the Query Parser.
     *  - queryString:      the full query string.
     *  - returnsBool:      boolean describing if this query returns a BOOLEAN.
     *  - targetEntities:   vector of parsed return types for this query.
     *  - synonymTable:     a mapping of declared synonyms to design entities.
     *  - relations:        a vector of parsed relation clauses.
     *  - patterns:         a vector of parsed pattern clauses.
     *  - equalities:       a vector of parsed with (equality) clauses.
     */
    struct Query {
        std::string status;
        std::string queryString;
        bool returnsBool;
        std::vector<ReturnType> targetEntities;
        std::unordered_map<std::string, DesignEntity> synonymTable;
        std::vector<RelationClause> relations;
        std::vector<PatternClause> patterns;
        std::vector<WithClause> equalities;
        bool operator==(const Query& other) const;
    };

    /**
     *  Struct representing an optimised PQL query.
     *
     *  - clauses:          the clauses in the order in which they should be evaluated.
     *  - group:            the group that each clause belongs to (numbered starting from 0).
     *  - last:             the index of the last clause in each group.
     */
    struct OptimisedQuery {
        std::vector<Clause*> clauses;
        std::vector<int> groups;
        std::vector<int> last;
    };

    using ClauseResultEntry = std::vector<std::string>;

    struct ClauseResult {
        bool trueResult = false;
        std::vector<Synonym> syns;
        std::vector<ClauseResultEntry> rows;
    };

}
