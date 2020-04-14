#include "stdafx.h"

#include "PQLTypes.h"
#include "QueryParserStub.h"

using namespace PQL;

using std::string;
using std::vector;

namespace UnitTesting {

    QueryParserStub::QueryParserStub() : QueryParser() {
        this->queryLog = { 0, { } };
    }

    Query QueryParserStub::parseQuery(string queryString) {
        this->queryLog.first += 1;
        this->queryLog.second.push_back(queryString);

        if (queryString == StubQueries::EMPTY_QUERY) {
            // Fails immediately during syntax validation - no fields populated
            return {
                SYNTAX_ERR_EMPTY_QUERY, queryString, false,
                { },
                { },
                { },
                { },
                { }
            };
        } else if (queryString == StubQueries::BOOLEAN_VALID_QUERY) {
            // Passes all validation
            return {
                STATUS_SUCCESS, queryString, true,
                { },
                { { "a", DesignEntity::ASSIGN } },
                { },
                {
                    {
                        "pattern a(_, _\"x\"_)", false, PatternType::ASSIGN_PATTERN, "a",
                        { ArgType::WILDCARD, "_" },
                        { ArgType::INCLUSIVE_PATTERN, "_x_" }
                    }
                },
                { }
            };
        } else if (queryString == StubQueries::BOOLEAN_INVALID_DECL_SYNTAX_QUERY) {
            // Fails immediately during syntax validation of declarations - no fields populated
            return {
                SYNTAX_ERR_INVALID_DECLARATION, queryString, false,
                { },
                { },
                { },
                { },
                { }
            };
        } else if (queryString == StubQueries::BOOLEAN_UNKNOWN_DECL_KEYWORD_QUERY) {
            // Fails during parsing of declarations - only terminates after parsing of return type
            // No other fields populated except for the returnsBool boolean flag
            return {
                SYNTAX_ERR_UNKNOWN_DESIGN_ENTITY_KEYWORD, queryString, true,
                { },
                { },
                { },
                { },
                { }
            };
        } else if (queryString == StubQueries::BOOLEAN_REPEATED_DECLARATION_QUERY) {
            // Fails during insertion of repeated synonym mapping during parsing of declarations
            // Only terminates after parsing of return type - returnsBool flag populated
            return {
                SEMANTIC_ERR_CONFLICTING_SYNONYM_DECLARATIONS, queryString, true,
                { },
                { { "ifs", DesignEntity::IF } },
                { },
                { },
                { }
            };
        } else if (queryString == StubQueries::BOOLEAN_CONFLICTING_DECLARATION_QUERY) {
            // Fails during insertion of second synonym mapping during parsing of declarations
            // Only terminates after parsing of return type - returnsBool flag populated
            return {
                SEMANTIC_ERR_CONFLICTING_SYNONYM_DECLARATIONS, queryString, true,
                { },
                { { "r", DesignEntity::READ } },
                { },
                { },
                { }
            };
        } else if (queryString == StubQueries::TUPLE_VALID_QUERY) {
            // Passes all validation
            return {
                STATUS_SUCCESS, queryString, false,
                { { "l1", AttrType::NONE }, { "l2", AttrType::NONE } },
                { { "l1", DesignEntity::PROG_LINE }, { "l2", DesignEntity::PROG_LINE } },
                {
                    {
                        "Next*(l1, l2)", false, RelationType::NEXTT,
                        { ArgType::SYNONYM, "l1" },
                        { ArgType::SYNONYM, "l2" }
                    }
                },
                { },
                { }
            };
        } else if (queryString == StubQueries::TUPLE_INVALID_DECL_SYNTAX_QUERY) {
            // Fails immediately during syntax validation of declarations - no fields populated
            return {
                SYNTAX_ERR_INVALID_DECLARATION, queryString, false,
                { },
                { },
                { },
                { },
                { }
            };
        } else if (queryString == StubQueries::TUPLE_UNKNOWN_DECL_KEYWORD_QUERY) {
            // Fails during parsing of declarations - only terminates after parsing of return type
            // targetEntities vector populated with tuple elements
            return {
                SYNTAX_ERR_UNKNOWN_DESIGN_ENTITY_KEYWORD, queryString, false,
                { { "v1", AttrType::NONE }, { "v2", AttrType::NONE } },
                { },
                { },
                { },
                { }
            };
        } else if (queryString == StubQueries::TUPLE_REPEATED_DECLARATION_QUERY) {
            // Fails during insertion of repeated synonym mapping during parsing of declarations
            // Only terminates after parsing of return type - targetEntities populated
            return {
                SEMANTIC_ERR_CONFLICTING_SYNONYM_DECLARATIONS, queryString, false,
                { { "p", AttrType::PROC_NAME } },
                { { "P", DesignEntity::PROCEDURE } },
                { },
                { },
                { }
            };
        } else if (queryString == StubQueries::TUPLE_CONFLICTING_DECLARATION_QUERY) {
            // Fails during insertion of second synonym mapping during parsing of declarations
            // Only terminates after parsing of return type - targetEntities populated
            return {
                SEMANTIC_ERR_CONFLICTING_SYNONYM_DECLARATIONS, queryString, false,
                { { "s", AttrType::NONE }, { "c", AttrType::VALUE } },
                { { "s", DesignEntity::STATEMENT }, { "c", DesignEntity::CONSTANT } },
                { },
                { },
                { }
            };
        } else if (queryString == StubQueries::INVALID_RETURN_TYPE_QUERY) {
            // Fails during parsing of return type - declarations populated
            return {
                STATUS_SUCCESS, queryString, false,
                { },
                { { "cl", DesignEntity::CALL } },
                { },
                { },
                { }
            };
        } else {
            // Query string is not in the list of predefined queries for this stubbed method
            // Return an empty (dummy) Query object
            Query query;
            query.status = STATUS_SUCCESS;
            query.queryString = queryString;
            query.returnsBool = false;
            return query;
        }
    }
}
