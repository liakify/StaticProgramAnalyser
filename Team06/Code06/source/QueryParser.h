#pragma once

#include <regex>
#include <sstream>

#include "PQLTypes.h"
#include "QueryUtils.h"

using std::istringstream;
using std::regex;
using std::smatch;

namespace PQL {

    class QueryParser {
    public:
        QueryParser();
        Query parseQuery(string queryString);
    private:
        int queryCount;
        bool validateQuerySyntax(Query& query, vector<string> stmts);
        bool validateQuerySemantics(Query& query);
        vector<string> splitStatements(string queryString);
        pair<vector<string>, vector<string>> splitConstraints(string queryBody);
        bool parseDeclarations(Query& query, vector<string> statements);
        bool parseQueryTarget(Query& query, string queryBody);
        bool parseRelationClauses(Query& query, vector<string> relationClauses);
        bool parsePatternClauses(Query& query, vector<string> patternClauses);
        pair<ArgType, StmtRef> parseStmtRef(string arg);
        pair<ArgType, EntityRef> parseEntityRef(string arg);
        pair<ArgType, Pattern> parsePattern(string arg);
    };
    
}
