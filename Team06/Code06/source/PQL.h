#pragma once

#include <stdio.h>
#include <cassert>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "PKB.h"
#include "Types.h"

using namespace std;

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
        static string rightTrim(string input);
        static string trimString(string input);
        static pair<string, string> splitString(string input, char delim);
        static vector<string> tokeniseString(string input, char delim);
        static vector<string> dualMatch(string input, string first, string second);
    };
}
