#pragma once

#include <stdio.h>
#include <cassert>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "PKB.h"
#include "Types.h"

using namespace std;

namespace PQL {

    const unordered_map<string, DesignEntity> ENTITY_MAP {
        {"statement", STATEMENT},
        {"read", READ},
        {"print", PRINT},
        {"call", CALL},
        {"while", WHILE},
        {"if", IF},
        {"assign", ASSIGN},
        {"variable", VARIABLE},
        {"constant", CONSTANT},
        {"procedure", PROCEDURE}
    };

    const unordered_map<string, RelationType> RELATION_MAP{
        {"Follows", FOLLOWS},
        {"Follows*", FOLLOWST},
        {"Parent", PARENT},
        {"Parent*", PARENTT},
        {"Uses", USESS},
        {"Modifies", MODIFIESS}
    };

    struct RelationClause {
        string clause;
        RelationType type;
        StatementRef firstStmt;
        StatementRef secondStmt;
        EntityRef firstEnt;
        EntityRef secondEnt;
    };

    struct PatternClause {
        string clause;
        PatternType type;
        EntityRef targetArg;
        string patternArg;
    };

    struct Query {
        string status;
        string queryString;
        vector<string> targetEntities;
        unordered_map<string, DesignEntity> synonymTable;
        vector<RelationClause> relations;
        vector<PatternClause> patterns;
    };

    class QueryParser {
    public:
        QueryParser();
        Query parseQuery(string queryString);
    private:
        int queryCount;
        bool validateQuerySyntax(string queryString);
        bool validateQuerySemantics(Query &query);
        vector<string> splitStatements(string queryString);
        pair<vector<string>, vector<string>> splitConstraints(string queryBody);
        bool parseDeclarations(Query &query, vector<string> statements);
        bool parseQueryTarget(Query &query, string queryBody);
        bool parseRelationClauses(Query &query, vector<string> relationClauses);
        bool parsePatternCluases(Query &query, vector<string> patternClauses);
    };

    class QueryEvaluator {
    public:
        QueryEvaluator(PKB::PKB database);
    private:
        static PKB::PKB database;
    };

    class QueryResultProjector {
    public:
    };

    class PQLManager {
    public:
        PQLManager(PKB::PKB database);
        string evaluateQuery(string queryString);
    private:
        static QueryParser parser;
        static QueryEvaluator evaluator;
        static QueryResultProjector formatter;
    };
    
    class ParserUtils {
    public:
        static bool isInteger(string input);
        static bool isValidIdentifier(string input);
        static bool isValidStmtRef(string input);
        static bool isValidEntityRef(string input);
        static bool isValidPattern(string input);
        static string leftTrim(string input);
        static pair<string, string> splitString(string input, char delim);
        static vector<string> tokeniseString(string input, char delim);
        static vector<string> dualMatch(string input, string first, string second);
    };
}
