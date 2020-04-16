#pragma once

#include <algorithm>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "PQLTypes.h"
#include "QueryUtils.h"

/**
 *  Bitmask used to ensure C++ regex_search searches for the first match from the
 *  beginning of the input string and not mid-way through the string.
 */
const auto MODE_CONSUME = std::regex_constants::match_continuous;

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
        Query parseQuery(std::string queryString);
     private:
        int queryCount;
        bool validateQuerySyntax(Query& query, std::vector<std::string> stmts);
        bool validateQuerySemantics(Query& query);
        std::vector<std::string> splitStatements(std::string queryString);
        bool splitClauses(Query& query, std::string queryBodySuffix,
            std::vector<std::string>& relationClauses, std::vector<std::string>& patternClauses, std::vector<std::string>& withClauses);
        bool parseDeclarations(Query& query, std::vector<std::string> statements);
        std::pair<bool, std::string> parseQueryTarget(Query& query, std::string queryBody);
        bool parseRelationClauses(Query& query, std::vector<std::string>& relationClauses);
        bool parsePatternClauses(Query& query, std::vector<std::string>& patternClauses);
        bool parseWithClauses(Query& query, std::vector<std::string>& withClauses);
        std::pair<bool, ReturnType> parseReturnType(std::string arg);
        Argument parseStmtRef(std::string arg);
        Argument parseEntityRef(std::string arg);
        Argument parsePattern(std::string arg);
        std::pair<bool, Argument> parseRef(std::string arg);
        std::pair<bool, std::pair<std::string, AttrType>> parseAttribute(std::string arg);
    };

}
