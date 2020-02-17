#include "QueryParser.h"

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

        parseQueryTarget(query, queryBody);

        vector<string> relationClauses, patternClauses;
        tie(relationClauses, patternClauses) = splitConstraints(queryBody);

        parseRelationClauses(query, relationClauses);
        parsePatternClauses(query, patternClauses);

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

        regex VALID_DECLARATION("^[A-Za-z][A-Za-z0-9]* +[A-Za-z][A-Za-z0-9]*( *, *[A-Za-z][A-Za-z0-9]*)*$");
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

        for (string target : query.targetEntities) {
            if (synonymTable.find(target) == synonymTable.end()) {
                // Case 1: single return type (accepts special return type BOOLEAN)
                if (query.targetEntities.size() == 1 && target == "BOOLEAN") continue;
                
                // Case 2: tuple return type (treat BOOLEAN as an identifier)
                // SEMANTIC ERROR: synonym referenced as query target is undeclared
                query.status = "semantic error: undeclared synonym for query return type";
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
                        query.status = "semantic error: undeclared synonym arg in Uses/Modifies clause";
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
                        query.status = "semantic error: undeclared synonym arg in Uses/Modifies clause";
                        return false;
                    }
                    else if (synonymMapping->second != DesignEntity::VARIABLE) {
                        // SEMANTIC ERROR: design entity type error (non-VARIABLE)
                        query.status = "semantic error: synonym arg in Uses/Modifies clause not a VARIABLE";
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
                            query.status = "semantic error: line number in relation clause must be postiive";
                            return false;
                        }
                    }
                    else if (arg.first == ArgType::SYNONYM) {
                        auto synonymMapping = synonymTable.find(arg.second);
                        if (synonymMapping == synonymTable.end()) {
                            // SEMANTIC ERROR: synonym referenced in relation clause is undeclared
                            query.status = "semantic error: undeclared synonym arg in Follows(*)/Parent(*) clause";
                            return false;
                        }
                        else if (synonymMapping->second == DesignEntity::VARIABLE
                            || synonymMapping->second == DesignEntity::CONSTANT
                            || synonymMapping->second == DesignEntity::PROCEDURE) {
                            // SEMANTIC ERROR: design entity type error (non-STATEMENT or subset)
                            query.status = "semantic error: synonym arg in relation clause not a STATEMENT or sub-type";
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
                    query.status = "semantic error: undeclared synonym arg used in pattern clause";
                    return false;
                }
                else if (synonymMapping->second != DesignEntity::VARIABLE) {
                    // SEMANTIC ERROR: design entity type error (non-VARIABLE)
                    query.status = "semantic error: synonym arg in pattern clause not a VARIABLE";
                    return false;
                }
            }
        }

        return true;
    }

    vector<string> QueryParser::splitStatements(string queryString) {
        vector<string> statements;

        regex DECLARATION("[A-Za-z0-9,\\s]+;");
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

    // Assumes syntactically invalid compound clauses do not exist after syntax checking, e.g.
    // Select a such that Modifies (a, v) and such that Parent (a, w) and pattern (v, _)
    // C++ regex does not support negative lookbehind, (?<!(x))y - match y only if not preceded by x
    // Returns an array of clauses
    pair<vector<string>, vector<string>> QueryParser::splitConstraints(string queryBody) {
        string RELATION_COMPOUND_CLAUSE = "such that +[A-Za-z*]+ *\\([\\w,\\s]*\\)(?: and +[A-Za-z*]+ *\\([\\w,\\s]*\\))*";
        string PATTERN_COMPOUND_CLAUSE = "pattern +[A-Za-z][A-Za-z0-9]* *\\([\\w,\"\\+\\-\\*\\/\\%\\s]*\\)(?: and +[A-Za-z][A-Za-z0-9]* *\\([\\w,\"\\+\\-\\*\\/\\%\\s]*\\))*";
        string RELATION_CLAUSE = "[A-Za-z*]+ *\\([\\w,\\s]*\\)";
        string PATTERN_CLAUSE = "[A-Za-z][A-Za-z0-9]* *\\([\\w,\"\\+\\-\\*\\/\\%\\s]*\\)";

        vector<string> relationClauses = QueryUtils::dualMatch(queryBody, RELATION_COMPOUND_CLAUSE, RELATION_CLAUSE);
        vector<string> patternClauses = QueryUtils::dualMatch(queryBody, PATTERN_COMPOUND_CLAUSE, PATTERN_CLAUSE);

        return { relationClauses, patternClauses };
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
                    query.status = "syntax error: unrecognised design entity in declaration";
                    return false;
                }

                auto synonymMapping = synonymTable.find(synonym);
                if (synonymMapping != synonymTable.end()) {
                    // SEMANTIC ERROR: current synonym has already been declared
                    query.status = "semantic error: conflicting synonym declaration";
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

    bool QueryParser::parseQueryTarget(Query& query, string queryBody) {
        vector<string> targets;

        regex SINGLE_TARGET("Select +[A-Za-z][A-Za-z0-9]*(?! *,)");
        regex TUPLE_TARGET("Select +< *[A-Za-z][A-Za-z0-9]*( *, *[A-Za-z][A-Za-z0-9]*)+ *>(?! *,)");
        smatch tmatch;

        // Attempt to match a single return type, otherwise match a tuple return type
        if (regex_search(queryBody, tmatch, SINGLE_TARGET)) {
            // Strip leading "Select "
            string targetEntity = QueryUtils::leftTrim(tmatch.str().erase(0, 6));
            targets.push_back(targetEntity);
        }
        else if (regex_search(queryBody, tmatch, TUPLE_TARGET)) {
            // Retrieve first match - a string of form "Select <x1, x2, ...>"
            string tupleString = QueryUtils::splitString(tmatch.str(), '<').second;
            tupleString.pop_back();

            // Extract all target entities in the tuple string "x1, x2, ..."
            targets = QueryUtils::tokeniseString(tupleString, ',');
        }
        else {
            // SYNTAX ERROR: unable to parse select target to entities
            query.status = "syntax error: target entity not correctly specified";
            return false;
        }

        if (targets.size() < 0) {
            // SYNTAX ERROR: Query must have at least one return target
            query.status = "syntax error: missing query target";
            return false;
        }
        else {
            query.targetEntities = targets;
        }

        return true;
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
                    query.status = "syntax error: invalid entity reference in Uses/Modifies clause";
                } else if (QueryUtils::isValidStmtRef(arg1)) {
                    relation = { clause, relationClass, parseStmtRef(arg1), INVALID_ARG, INVALID_ARG, parseEntityRef(arg2) };
                }
                else if (QueryUtils::isValidEntityRef(arg1)) {
                    relation = { clause, relationClass, INVALID_ARG, INVALID_ARG, parseEntityRef(arg1), parseEntityRef(arg2) };
                }
                else {
                    // SYNTAX ERROR: cannot be interpreted either as statement or entity ref
                    query.status = "syntax error: invalid first argument in Uses/Modifies clause";
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
                query.status = "semantic error: undeclared synonym in pattern clause";
                return false;
            }

            DesignEntity entityType = synonymMapping->second;
            string referenceString = args.at(0);

            // First argument is always an entity reference, validate it
            if (!QueryUtils::isValidEntityRef(referenceString)) {
                // SYNTAX ERROR: invalid entity reference
                query.status = "syntax error: pattern has invalid entity reference";
                return false;
            }

            PatternClause pattern;

            switch (entityType) {
            case DesignEntity::ASSIGN:
                if (args.size() != 2) {
                    // SYNTAX ERROR: incorrect number of arguments
                    query.status = "syntax error: assign pattern does not have two arguments";
                }
                else if (!QueryUtils::isValidPattern(args.at(1))) {
                    // SYNTAX ERROR: invalid pattern string
                    query.status = "syntax error: assign pattern has invalid pattern string";
                }
                else {
                    pattern = { clause, PatternType::ASSIGN_PATTERN, parseEntityRef(referenceString), parsePattern(args.at(1)) };
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
                    pattern = { clause, PatternType::WHILE_PATTERN, parseEntityRef(referenceString), parsePattern(args.at(1)) };
                }
                break;
            case DesignEntity::IF:
                if (args.size() != 3) {
                    // SYNTAX ERROR: incorrect number of arguments
                    query.status = "syntax error: if pattern does not have two arguments";
                }
                else if (args.at(1) != "_" || args.at(2) != "_") {
                    // SYNTAX ERROR: unallowed argument for while pattern clause
                    query.status = "syntax error: if pattern only supports '_' for last two arguments";
                }
                else {
                    // Pattern struct only stores first two args since third arg is fixed as '_' anyway
                    pattern = { clause, PatternType::IF_PATTERN, parseEntityRef(referenceString), parsePattern(args.at(1)) };
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
            return { ArgType::IDENTIFIER, arg.erase(0, 1) };
        }
        else {
            return { ArgType::SYNONYM, arg };
        }
    }

    pair<ArgType, Pattern> QueryParser::parsePattern(string arg) {
        if (arg == "_") {
            return { ArgType::WILDCARD, arg };
        }
        else if (arg.at(0) == '_') {
            // Inclusive pattern string
            return { ArgType::INCLUSIVE_PATTERN, arg };
        }
        else {
            return { ArgType::EXACT_PATTERN, arg };
        }
    }

}
