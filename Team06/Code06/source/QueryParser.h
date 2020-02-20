#pragma once

#include <regex>
#include <sstream>

#include "PQLTypes.h"
#include "QueryUtils.h"

using std::regex;
using std::smatch;

namespace PQL {

    /**
     *  The QueryParser class is the implementation of the Query Parser sub-component
     *  of the Query Processor. It accepts input PQL query strings and validates its
     *  syntax and semantics, parsing it into a complete query struct for use by the
     *  Query Evaluator sub-component if successful.
     */
    class QueryParser {
    public:
        /**
         *  Empty constructor for the QueryParser class.
         */
        QueryParser();

        /**
         *  Accepts an input PQL query string for parsing and validation, and returns
         *  a well-formed Query struct if successful, or terminates early if an error
         *  is encountered.
         *
         *  The 'status' attribute of the returned Query is populated with "success" if
         *  parsing and validation were both successful, or otherwise an error message
         *  describing the syntax or semantic error encountered.
         *
         *  @param      queryString     input PQL query string.
         *  @return     populated Query struct with status message.
         */
        Query parseQuery(string queryString);
    private:
        int queryCount;
        bool validateQuerySyntax(Query& query, vector<string> stmts);
        bool validateQuerySemantics(Query& query);
        vector<string> splitStatements(string queryString);
        tuple<bool, vector<string>, vector<string>> splitConstraints(Query& query, string queryBodySuffix);
        bool parseDeclarations(Query& query, vector<string> statements);
        pair<bool, string> parseQueryTarget(Query& query, string queryBody);
        bool parseRelationClauses(Query& query, vector<string> relationClauses);
        bool parsePatternClauses(Query& query, vector<string> patternClauses);
        pair<ArgType, StmtRef> parseStmtRef(string arg);
        pair<ArgType, EntityRef> parseEntityRef(string arg);
        pair<ArgType, Pattern> parsePattern(string arg);
    };
    
}
