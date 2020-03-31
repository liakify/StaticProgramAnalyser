#pragma once

#include <string>
#include <utility>
#include <vector>

#include "PQLTypes.h"
#include "QueryUtils.h"
#include "QueryParser.h"

namespace UnitTesting {

    namespace StubQueries {
        const std::string EMPTY_QUERY = "";
        const std::string BOOLEAN_VALID_QUERY = "assign a; Select BOOLEAN pattern a(_, _\"x\"_)";
        const std::string BOOLEAN_INVALID_DECL_SYNTAX_QUERY = "procedure p & q; Select BOOLEAN";
        const std::string BOOLEAN_UNKNOWN_DECL_KEYWORD_QUERY = "statement s; Select BOOLEAN";
        const std::string BOOLEAN_REPEATED_DECLARATION_QUERY = "if ifs, ifs; Select BOOLEAN";
        const std::string BOOLEAN_CONFLICTING_DECLARATION_QUERY = "read r; print r; Select BOOLEAN";
        const std::string TUPLE_VALID_QUERY = "prog_line l1, l2; Select <l1, l2> such that Follows*(l1, l2)";
        const std::string TUPLE_INVALID_DECL_SYNTAX_QUERY = "variable v1 & v2; Select <v1, v2>";
        const std::string TUPLE_UNKNOWN_DECL_KEYWORD_QUERY = "while w; progline l; Select <w, l>";
        const std::string TUPLE_REPEATED_DECLARATION_QUERY = "procedure p, p; Select <p.procName>";
        const std::string TUPLE_CONFLICTING_DECLARATION_QUERY = "stmt s; constant c; call c; Select <s, c.value>";
        const std::string INVALID_RETURN_TYPE_QUERY = "call cl; Select [cl]";
    }

    /**
     *  The QueryParserStub class is a stub for the Query Parser sub-component used
     *  used by the Query Processor.
     */
    class QueryParserStub : public PQL::QueryParser {
     public:
        /**
         *  Empty constructor for the QueryParser stub.
         */
        QueryParserStub();

        /**
         *  Stubbed method for the Query Parser sub-component. Returns pre-constructed
         *  Query objects for a specific set of queries, or otherwise an empty Query
         *  struct with its default values.
         */
        PQL::Query parseQuery(std::string queryString);
     private:
        std::pair<int, std::vector<std::string>> queryLog;
    };

}
