#include "QueryParser.h"
#include "Parser.h"

using std::invalid_argument;
using std::pair;
using std::regex;
using std::smatch;
using std::string;
using std::tuple;
using std::unordered_map;
using std::vector;

namespace PQL {

    QueryParser::QueryParser() {
        this->queryCount = 0;
    }

    Query QueryParser::parseQuery(string queryString) {
        Query query;
        query.status = "ok";
        query.queryString = queryString;
        queryCount += 1;

        vector<string> statements = splitStatements(queryString);

        bool isValidQuery = validateQuerySyntax(query, statements);
        if (!isValidQuery) return query;

        // Extract mappings from synonyms to design entities
        bool hasValidEntities = parseDeclarations(query, statements);
        if (!hasValidEntities) return query;

        // Extract last statement (Select <var> (constraints)...)
        string queryBody = statements.back();
        
        bool hasValidTarget;
        string queryBodySuffix;
        tie(hasValidTarget, queryBodySuffix) = parseQueryTarget(query, queryBody);
        if (!hasValidTarget) return query;

        bool hasValidClauses;
        vector<string> relationClauses, patternClauses;
        tie(hasValidClauses, relationClauses, patternClauses) = splitConstraints(query, queryBodySuffix);
        if (!hasValidClauses) return query;

        bool hasValidRelations = parseRelationClauses(query, relationClauses);
        bool hasValidPatterns = parsePatternClauses(query, patternClauses);
        if (!(hasValidRelations && hasValidPatterns)) return query;

        // Ensure query is semantically valid (no ambiguities, valid synonyms, argument types)
        bool isSemanticallyValid = validateQuerySemantics(query);
        if (!isSemanticallyValid) return query;

        query.status = "success";
        return query;
    }

    // Performs basic validation of overall PQL query structure by validating individual
    // statements in the PQL query
    // Additional validation is performed during parsing of declarations, query body and clauses
    bool QueryParser::validateQuerySyntax(Query& query, vector<string> statements) {
        if (statements.size() == 0) {
            query.status = "syntax error: empty query";
            return false;
        }

        regex VALID_DECLARATION("^[A-Za-z]\\w* +[A-Za-z][A-Za-z0-9]*( *, *[A-Za-z][A-Za-z0-9]*)*$");
        smatch dmatch;

        // Validate each declaration (first N - 1 statements) satisfy
        // PQL declaration syntax of form <keyword> <synonym>(, <synonym>)*;
        int numDeclarations = statements.size() - 1;
        for (int i = 0; i < numDeclarations; i++) {
            if (!regex_search(statements.at(i), dmatch, VALID_DECLARATION)) {
                // SYNTAX ERROR: invalid declaration
                query.status = "syntax error: declaration statement has incorrect syntax";
                return false;
            }
        }

        // Validate structure of query body (last statement)
        regex VALID_QUERY_BODY("^Select [\\w<>\\(\\),\"\\+\\-\\*\\/\\% ]+(?!;)$");
        smatch qbmatch;

        if (!regex_search(statements.at(numDeclarations), qbmatch, VALID_QUERY_BODY)) {
            query.status = "syntax error: query missing 'Select' keyword or has unrecognised characters";
            return false;
        }

        // Check if syntax violations occur from use of clause keywords and connectives
        regex INVALID_REPEATED_RELATION("and +such that");
        regex INVALID_REPEATED_PATTERN("and +pattern");
        smatch rmatch;

        if (regex_search(statements.at(numDeclarations), rmatch, INVALID_REPEATED_RELATION)
            || regex_search(statements.at(numDeclarations), rmatch, INVALID_REPEATED_PATTERN)) {
            // SYNTAX ERROR: incorrect use of 'and' keyword to connect adjacent clauses
            query.status = "syntax error: query contains incorrect use of clause keyword and 'and'";
            return false;
        }

        return true;
    }

    // Validates entity arguments correspond to an existing design entity in the synonym table
    // Validates allowed entity types as arguments to clauses
    // Validate query targets correspond to existing design entities in synonym table
    bool QueryParser::validateQuerySemantics(Query& query) {
        unordered_map<string, DesignEntity> synonymTable = query.synonymTable;

        // If used as a single return type, the special type BOOLEAN is not stored in the
        // list of target synonyms, but instead indicated by the attribute returnsBool
        for (string target : query.targetEntities) {
            if (synonymTable.find(target) == synonymTable.end()) {
                // If used in a tuple, BOOLEAN is treated as a synonym and hence the query is
                // semantically invalid if it has not been previously declared
                query.status = "semantic error: undeclared synonym part of query return type";
                return false;
            }
        }

        for (RelationClause relation : query.relations) {
            RelationType relationClass = relation.type;

            if (relationClass == RelationType::USESS || relationClass == RelationType::MODIFIESS) {
                if (relation.firstStmt.first == ArgType::WILDCARD) {
                    // SEMANTIC ERROR: ill-defined wildcard argument
                    query.status = "semantic error: wildcard not accepted as first arg for Uses/Modifies";
                    return false;
                }
                else if (relation.firstStmt.first == ArgType::SYNONYM) {
                    // Determine if the synonym corresponds to a PROCEDURE - if yes the relation
                    // should be modified to the procedure-variable variant
                    auto synonymMapping = synonymTable.find(relation.firstStmt.second);
                    if (synonymMapping == synonymTable.end()) {
                        // SEMANTIC ERROR: synonym referenced in relation clause is undeclared
                        query.status = "semantic error: undeclared synonym as first arg in Uses/Modifies clause";
                        return false;
                    }
                    else if (synonymMapping->second == DesignEntity::PROCEDURE) {
                        // Modify RelationClause to <RELATION>P variant and update arguments
                        relation.type = relation.type == RelationType::USESS
                            ? RelationType::USESP
                            : RelationType::MODIFIESP;
                        relation.firstEnt = relation.firstStmt;
                        relation.firstStmt = INVALID_ARG;
                    }
                }

                if (relation.secondEnt.first == ArgType::SYNONYM) {
                    auto synonymMapping = synonymTable.find(relation.secondEnt.second);
                    if (synonymMapping == synonymTable.end()) {
                        // SEMANTIC ERROR: synonym referenced in relation clause is undeclared
                        query.status = "semantic error: undeclared synonym as second arg in Uses/Modifies clause";
                        return false;
                    }
                    else if (synonymMapping->second != DesignEntity::VARIABLE) {
                        // SEMANTIC ERROR: design entity type error (non-VARIABLE)
                        query.status = "semantic error: synonym as second arg in Uses/Modifies clause not a VARIABLE";
                        return false;
                    }
                }
            }
            else {
                // Relation is between statements: FOLLOWS, FOLLOWST, PARENT, PARENTT
                pair<ArgType, string> args[2] = { relation.firstStmt , relation.secondStmt };

                for (auto arg : args) {
                    if (arg.first == ArgType::INTEGER) {
                        int lineNo = stoi(arg.second);
                        if (lineNo <= 0) {
                            // SEMANTIC ERROR: stmt number must be positive
                            // Not possible to determine now if stmt number exceeds length of program
                            query.status = "semantic error: statement line number in relation clause must be postiive";
                            return false;
                        }
                    }
                    else if (arg.first == ArgType::SYNONYM) {
                        auto synonymMapping = synonymTable.find(arg.second);
                        if (synonymMapping == synonymTable.end()) {
                            // SEMANTIC ERROR: synonym referenced in relation clause is undeclared
                            query.status = "semantic error: undeclared synonym as first arg in Follows(*)/Parent(*) clause";
                            return false;
                        }
                        else if (synonymMapping->second == DesignEntity::VARIABLE
                            || synonymMapping->second == DesignEntity::CONSTANT
                            || synonymMapping->second == DesignEntity::PROCEDURE) {
                            // SEMANTIC ERROR: design entity type error (non-STATEMENT or subset)
                            query.status = "semantic error: synonym as second arg in relation clause not a STATEMENT or sub-type";
                            return false;
                        }
                    }
                }
            }
        }

        // Pattern clauses: only need to validate first argument (entity reference)
        // of ArgType SYNONYM corresponds to a valid declared synonym
        for (PatternClause pattern : query.patterns) {
            if (pattern.targetArg.first == ArgType::SYNONYM) {
                auto synonymMapping = synonymTable.find(pattern.targetArg.second);
                if (synonymMapping == synonymTable.end()) {
                    // SEMANTIC ERROR: synonym referenced in pattern clause is undeclared
                    query.status = "semantic error: undeclared synonym as first arg in pattern clause";
                    return false;
                }
                else if (synonymMapping->second != DesignEntity::VARIABLE) {
                    // SEMANTIC ERROR: design entity type error (non-VARIABLE)
                    query.status = "semantic error: synonym as first arg in pattern clause not a VARIABLE";
                    return false;
                }
            }
        }

        return true;
    }

    vector<string> QueryParser::splitStatements(string queryString) {
        vector<string> statements;

        regex DECLARATION("[\\w,\\s]+;");
        smatch stmatch;

        while (regex_search(queryString, stmatch, DECLARATION)) {
            for (auto token : stmatch) {
                string stmt = token.str();
                stmt.pop_back();
                statements.push_back(QueryUtils::trimString(stmt));
            }
            queryString = stmatch.suffix().str();
        }

        // Last statement is the query body
        statements.push_back(QueryUtils::trimString(queryString));
        return statements;
    }

    // Assumes invalid connected clauses do not exist after syntax checking, e.g.
    // Select a such that Modifies (a, v) and such that Parent (a, w) and pattern (v, _)
    // C++ regex does not support negative lookbehind, (?<!(x))y - match y only if not preceded by x
    // Consumes compound clauses from start to end, terminating early if failing to do so before
    // reaching the end of the query string (i.e. there exists some incorrect syntax)
    // Returns an array of clauses
    tuple<bool, vector<string>, vector<string>> QueryParser::splitConstraints(Query& query, string queryBodySuffix) {
        string RELATION_CLAUSE = "[A-Za-z*]+ *\\([\\w,\"\\s]*\\)";
        string PATTERN_CLAUSE = "[A-Za-z][A-Za-z0-9]* *\\([\\w\"\\s]+(?:, *(?:_|(?:(_?) *\"[A-Za-z0-9\\(\\)\\+\\-\\*\\/\\%\\s]+\" *\\1)) *)+\\)";

        regex COMPOUND_RELATION_CLAUSE("^ *such that +" + RELATION_CLAUSE + "(?: +and +" + RELATION_CLAUSE + ")*");
        regex COMPOUND_PATTERN_PREFIX("^ *pattern +[A-Za-z][A-Za-z0-9]* *\\(");
        regex COMPOUND_PATTERN_CLAUSE("^ *pattern +" + PATTERN_CLAUSE + "(?: +and +" + PATTERN_CLAUSE + ")*");
        smatch ccmatch;

        vector<string> relationClauses;
        vector<string> patternClauses;

        // Continue consuming compound clauses until the end of the query
        while (queryBodySuffix.length() > 0) {

            // Attempt to consume either a relation or pattern compound clause
            // If this fails, then the query string is synctactically invalid
            if (regex_search(queryBodySuffix, ccmatch, COMPOUND_RELATION_CLAUSE)) {
                vector<string> relations = QueryUtils::matchAll(ccmatch.str(), RELATION_CLAUSE);
                relationClauses.insert(relationClauses.end(), relations.begin(), relations.end());
            }
            else if (regex_search(queryBodySuffix, ccmatch, COMPOUND_PATTERN_PREFIX)) {
                if (!regex_search(queryBodySuffix, ccmatch, COMPOUND_PATTERN_CLAUSE)) {
                    // SYNTAX ERROR: first arg has zero length or other args violate pattern string syntax
                    query.status = "syntax error: pattern clause has missing or malformed argument";
                    return { false, relationClauses, patternClauses };
                }

                vector<string> patterns = QueryUtils::matchAll(ccmatch.str(), PATTERN_CLAUSE);
                patternClauses.insert(patternClauses.end(), patterns.begin(), patterns.end());
            }
            else {
                // SYNTAX ERROR: compound clauses fail to obey query syntax somewhere in query body
                query.status = "syntax error: compound clauses in query body violate query body syntax";
                return { false, relationClauses, patternClauses };
            }

            queryBodySuffix = QueryUtils::leftTrim(ccmatch.suffix().str());
        }

        return { true, relationClauses, patternClauses };
    }

    bool QueryParser::parseDeclarations(Query& query, vector<string> statements) {
        unordered_map<string, DesignEntity> synonymTable;

        // Parse first N - 1 statements, i.e. exclude query body
        for (unsigned int i = 0; i < statements.size() - 1; i++) {
            string stmt = statements.at(i);

            // Each declaration is a string of form "<entity-name> <synonym>(, <synonym>)*"
            pair<string, string> splitPair = QueryUtils::splitString(stmt, ' ');
            string entityName = splitPair.first;
            string synonymString = splitPair.second;

            vector<string> synonyms = QueryUtils::tokeniseString(synonymString, ',');

            for (unsigned int j = 0; j < synonyms.size(); j++) {
                string synonym = synonyms.at(j);

                auto entityMapping = ENTITY_MAP.find(entityName);
                if (entityMapping == ENTITY_MAP.end()) {
                    // SYNTAX ERROR: unknown design entity
                    query.status = "syntax error: unrecognised design entity keyword in declaration";
                    return false;
                }

                auto synonymMapping = synonymTable.find(synonym);
                if (synonymMapping != synonymTable.end()) {
                    // SEMANTIC ERROR: current synonym has already been declared
                    query.status = "semantic error: conflicting synonym declarations";
                    return false;
                }

                synonymTable[synonym] = entityMapping->second;
            }
        }

        // Each statement except the query body contributes at least 1 synonym mapping
        assert(synonymTable.size() >= statements.size() - 1);
        query.synonymTable = synonymTable;

        return true;
    }

    pair<bool, string> QueryParser::parseQueryTarget(Query& query, string queryBody) {
        vector<string> targets;

        regex SINGLE_TARGET("^Select +[A-Za-z][A-Za-z0-9]*(?= *$| (?! *,))");
        regex TUPLE_TARGET("^Select +< *[A-Za-z][A-Za-z0-9]*( *, *[A-Za-z][A-Za-z0-9]*)* *>(?= *$| (?! *,))");
        smatch tmatch;

        // Attempt to match a single return type, otherwise match a tuple return type
        if (regex_search(queryBody, tmatch, SINGLE_TARGET)) {
            // Strip leading "Select"
            string targetEntity = QueryUtils::leftTrim(tmatch.str().erase(0, 6));
            if (targetEntity == "BOOLEAN") {
                query.returnsBool = true;
            }
            else {
                query.returnsBool = false;
                targets.push_back(targetEntity);
            }
        }
        else if (regex_search(queryBody, tmatch, TUPLE_TARGET)) {
            // Retrieve first match - a string of form "Select <x1, x2, ...>"
            string tupleString = QueryUtils::splitString(tmatch.str(), '<').second;
            tupleString.pop_back();

            // Extract all target entities in the tuple string "x1, x2, ..."
            query.returnsBool = false;
            targets = QueryUtils::tokeniseString(tupleString, ',');
        }
        else {
            // SYNTAX ERROR: unable to parse select target to entities
            query.status = "syntax error: missing query target or target entities not correctly specified";
            return { false, "" };
        }

        // Extract the suffix of the query body (following the query return types)
        string queryBodySuffix = QueryUtils::leftTrim(tmatch.suffix().str());

        assert(query.returnsBool || targets.size() >= 1);
        query.targetEntities = targets;

        return { true, queryBodySuffix };
    }

    bool QueryParser::parseRelationClauses(Query& query, vector<string> relationClauses) {
        vector<RelationClause> relations;

        for (auto clause : relationClauses) {
            // Each candidate relation clause is of form <relation> (arg1, arg2)

            pair<string, string> splitPair = QueryUtils::splitString(clause, '(');
            string relationKeyword = splitPair.first;
            string argString = splitPair.second;
            argString.pop_back();
            vector<string> args = QueryUtils::tokeniseString(argString, ',');

            // Find the relation corresponding to the provided relation keyword
            // Uses and Modifies for procedures are treated as that for statements first
            // until subsequent validation changes the relation type
            auto relationMapping = RELATION_MAP.find(relationKeyword);
            if (relationMapping == RELATION_MAP.end()) {
                // SYNTAX ERROR: unknown relation
                query.status = "syntax error: invalid relation keyword";
                return false;
            }
            else if (args.size() != 2) {
                // SYNTAX ERROR: too few or too many arguments
                query.status = "syntax error: relations only accept 2 arguments";
                return false;
            }

            RelationType relationClass = relationMapping->second;
            string arg1 = args.at(0);
            string arg2 = args.at(1);

            RelationClause relation;

            switch (relationClass) {
            case RelationType::FOLLOWS:
                // Fallthrough
            case RelationType::FOLLOWST:
                // Fallthrough
            case RelationType::PARENT:
                // Fallthrough
            case RelationType::PARENTT:
                // Interpret and validate both arguments as statement references
                if (!(QueryUtils::isValidStmtRef(arg1) && QueryUtils::isValidStmtRef(arg2))) {
                    // SYNTAX ERROR: at least one argument is not a valid statement reference
                    query.status = "syntax error: invalid statement reference in relation clause";
                }
                else {
                    relation = { clause, relationClass, parseStmtRef(arg1), parseStmtRef(arg2), INVALID_ARG, INVALID_ARG };
                }
                break;
            case RelationType::USESS:
                // Fallthrough
            case RelationType::MODIFIESS:
                // Two cases for first argument: either statement ref or entity ref
                // Not possible to distinguish here if a SYNONYM corresponds to stmt or entity ref
                // Semantic validation will check against the synonym table and determine if
                // the relation type needs to be modified to USESP or MODIFIESP for procedures

                // Validate second argument as an entity reference
                if (!(QueryUtils::isValidEntityRef(arg2))) {
                    // SYNTAX ERROR: second argument is not a valid entity reference
                    query.status = "syntax error: invalid entity reference as second arg in Uses/Modifies clause";
                } else if (QueryUtils::isValidStmtRef(arg1)) {
                    relation = { clause, relationClass, parseStmtRef(arg1), INVALID_ARG, INVALID_ARG, parseEntityRef(arg2) };
                }
                else if (QueryUtils::isValidEntityRef(arg1)) {
                    relation = {
                        clause, relationClass == RelationType::USESS ? RelationType::USESP : RelationType::MODIFIESP,
                        INVALID_ARG, INVALID_ARG, parseEntityRef(arg1), parseEntityRef(arg2)
                    };
                }
                else {
                    // SYNTAX ERROR: cannot be interpreted either as statement or entity ref
                    query.status = "syntax error: invalid first arg in Uses/Modifies clause";
                }
                break;
            default:
                // SYNTAX ERROR: unknown relation type
                query.status = "internal error: failed to match relation type";
            }

            if (query.status != "ok") {
                return false;
            }
            else {
                // Relation parsed successfully - add new relation clause
                relations.push_back(relation);
            }
        }

        query.relations = relations;
        return true;
    }

    // Receives a vector of pattern clause strings and constructs pattern clause objects
    bool QueryParser::parsePatternClauses(Query& query, vector<string> patternClauses) {
        vector<PatternClause> patterns;

        for (auto clause : patternClauses) {
            // Each candidate pattern clause is of form <synonym> (arg1, arg2, [arg3 if while])

            pair<string, string> splitPair = QueryUtils::splitString(clause, '(');
            string synonym = splitPair.first;
            string argString = splitPair.second;
            argString.pop_back();
            vector<string> args = QueryUtils::tokeniseString(argString, ',');

            auto synonymMapping = query.synonymTable.find(synonym);
            if (synonymMapping == query.synonymTable.end()) {
                // SEMANTIC ERROR: unknown synonym
                query.status = "semantic error: undeclared synonym for type of pattern clause";
                return false;
            }

            DesignEntity entityType = synonymMapping->second;
            string referenceString = args.at(0);

            // First argument is always an entity reference, validate it
            if (!QueryUtils::isValidEntityRef(referenceString)) {
                // SYNTAX ERROR: invalid entity reference
                query.status = "syntax error: invalid entity reference as first arg in pattern clause";
                return false;
            }

            PatternClause pattern;

            switch (entityType) {
            case DesignEntity::ASSIGN:
                if (args.size() != 2) {
                    // SYNTAX ERROR: incorrect number of arguments
                    query.status = "syntax error: assign pattern does not have two arguments";
                }

                // Attempt to parse the pattern clause with the SPA FE Parser during construction of pattern clause
                // If it fails, then the pattern string is not a valid infix arithmetic exprresion
                try {
                    pattern = { clause, PatternType::ASSIGN_PATTERN, synonym, parseEntityRef(referenceString), parsePattern(args.at(1)) };
                }
                catch (const invalid_argument& e) {
                    // SYNTAX ERROR: pattern string is not a valid infix arithmetic expression
                    query.status = "syntax error: assign pattern has invalid pattern string";
                }
                break;
            case DesignEntity::WHILE:
                if (args.size() != 2) {
                    // SYNTAX ERROR: incorrect number of arguments
                    query.status = "syntax error: while pattern does not have two arguments";
                }
                else if (args.at(1) != "_") {
                    // SYNTAX ERROR: unallowed argument for while pattern clause
                    query.status = "syntax error: while pattern only supports '_' as second argument";
                }
                else {
                    pattern = { clause, PatternType::WHILE_PATTERN, synonym, parseEntityRef(referenceString), parsePattern(args.at(1)) };
                }
                break;
            case DesignEntity::IF:
                if (args.size() != 3) {
                    // SYNTAX ERROR: incorrect number of arguments
                    query.status = "syntax error: if pattern does not have three arguments";
                }
                else if (args.at(1) != "_" || args.at(2) != "_") {
                    // SYNTAX ERROR: unallowed argument for while pattern clause
                    query.status = "syntax error: if pattern only supports '_' for last two arguments";
                }
                else {
                    // Pattern struct only stores first two args since third arg is fixed as '_' anyway
                    pattern = { clause, PatternType::IF_PATTERN, synonym, parseEntityRef(referenceString), parsePattern(args.at(1)) };
                }
                break;
            default:
                // SYNTAX ERROR: unallowed design entity in pattern clauses
                query.status = "syntax error: pattern clauses only defined for assign, if, while";
            }

            if (query.status != "ok") {
                return false;
            }
            else {
                // Pattern parsed successfully - add new pattern clause
                patterns.push_back(pattern);
            }
        }

        query.patterns = patterns;
        return true;
    }

    pair<ArgType, StmtRef> QueryParser::parseStmtRef(string arg) {
        if (arg == "_") {
            return { ArgType::WILDCARD, arg };
        }
        else if (QueryUtils::isInteger(arg)) {
            return { ArgType::INTEGER, arg };
        }
        else {
            return { ArgType::SYNONYM, arg };
        }
    }

    pair<ArgType, EntityRef> QueryParser::parseEntityRef(string arg) {
        if (arg == "_") {
            return { ArgType::WILDCARD, arg };
        }
        else if (arg.find('\"') != string::npos) {
            // An identifier - strip leading and trailing "
            arg.pop_back();
            return { ArgType::IDENTIFIER, QueryUtils::trimString(arg.erase(0, 1)) };
        }
        else {
            return { ArgType::SYNONYM, arg };
        }
    }

    pair<ArgType, Pattern> QueryParser::parsePattern(string arg) {
        if (arg == "_") {
            return { ArgType::WILDCARD, arg };
        }

        string strippedPattern = QueryUtils::stripPattern(arg);
        if (arg.at(0) == '_') {
            // Inclusive pattern string - remove leading and trailing underscore
            strippedPattern = strippedPattern.substr(1, strippedPattern.length() - 2);
            return { ArgType::INCLUSIVE_PATTERN, FrontEnd::Parser().parseExpression(strippedPattern) };
        }
        else {
            // Exact pattern string
            return { ArgType::EXACT_PATTERN, FrontEnd::Parser().parseExpression(strippedPattern) };
        }
    }

}
