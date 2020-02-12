#include "PQL.h"

namespace PQL {

    PQLManager::PQLManager(PKB::PKB database) {
        this->parser = QueryParser();
        this->evaluator = QueryEvaluator(database);
        this->formatter = QueryResultProjector();
    }

    string PQLManager::evaluateQuery(string queryString) {

        // Validate then parse PQL query string into a query struct representation
        Query query = this->parser.parseQuery(queryString);
        if (query.status != "success") {
            return query.status;
        }

        // Evaluate the given query struct against the PKB
        // vector<string> rawResults = this->evaluator.parseQuery(query);

        // Process the list of results into a single output string
        // return this->formatter.format(rawResults);

        return "None";
    }

    QueryParser::QueryParser() {
        this->queryCount = 0;
    }

    Query QueryParser::parseQuery(string queryString) {
        Query query;
        query.queryString = queryString;
        queryCount += 1;

        vector<string> statements = splitStatements(queryString);
        assert(statements.size() > 0);

        bool isValidQuery = validateQuerySyntax(queryString);
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
        parsePatternCluases(query, patternClauses);

        // Ensure query is semantically valid (no ambiguities, valid synonyms, argument types)
        bool isSemanticallyCorrect = validateQuerySemantics(query);
        if (!isSemanticallyCorrect) return query;

        query.status = "success";
        return query;
    }

    // Perform basic validation of overall PQL query syntax
    // Additional validation is performed during parsing of declarations, statements and clauses
    bool QueryParser::validateQuerySyntax(string queryString) {
        // Validate each declaration is of form <design-entity> <synonym>(, <synonym>)*;
        return true;
    }

    bool QueryParser::validateQuerySemantics(Query& query) {
        return true;
    }

    vector<string> QueryParser::splitStatements(string queryString) {
        vector<string> statements;

        regex DECLARATION("[A-Za-z0-9,\\s]+;");
        smatch stmatch;

        while (regex_search(queryString, stmatch, DECLARATION)) {
            for (auto token : stmatch) {
                string stmt = token.str();
                statements.push_back(ParserUtils::leftTrim(stmt));
            }
            queryString = stmatch.suffix().str();
        }

        // Last statement is the query body
        statements.push_back(ParserUtils::leftTrim(queryString));
        return statements;
    }

    // Assumes syntactically invalid compound clauses do not exist after syntax checking, e.g.
    // Select a such that Modifies (a, v) and such that Parent (a, w) and pattern (v, _)
    // C++ regex does not support negative lookbehind, (?<!(x))y - match y only if not preceded by x
    // Returns an array of clauses
    pair<vector<string>, vector<string>> QueryParser::splitConstraints(string queryBody) {
        string RELATION_COMPOUND_CLAUSE = "such that [A-Za-z*]+ \\([A-Za-z_,\\s]*\\)(?: and [A-Za-z*]+ \\([A-Za-z_,\\s]*\\))*";
        string PATTERN_COMPOUND_CLAUSE = "pattern [A-Za-z*]+ \\([A-Za-z_\",\\s]*\\)(?: and [A-Za-z*]+ \\([A-Za-z_\",\\s]*\\))*";
        string RELATION_CLAUSE = "[A-Za-z*]+ \\([A-Za-z_,\\s]*\\)";
        string PATTERN_CLAUSE = "[A-Za-z*]+ \\([A-Za-z_\",\\s]*\\)";

        vector<string> relationClauses = ParserUtils::dualMatch(queryBody, RELATION_COMPOUND_CLAUSE, RELATION_CLAUSE);
        vector<string> patternClauses = ParserUtils::dualMatch(queryBody, PATTERN_COMPOUND_CLAUSE, PATTERN_CLAUSE);

        return make_pair(relationClauses, patternClauses);
    }

    bool QueryParser::parseDeclarations(Query& query, vector<string> statements) {
        unordered_map<string, DesignEntity> synonymTable;

        // Parse first N - 1 statements, i.e. exclude query body
        for (unsigned int i = 0; i < statements.size() - 1; i++) {
            string stmt = statements.at(i);

            // Each declaration is a string of form "<entity-name> <synonym>(, <synonym>)*"
            pair<string, string> splitPair = ParserUtils::splitString(stmt, ' ');
            string entityName = splitPair.first;
            string synonymString = splitPair.second;

            vector<string> synonyms = ParserUtils::tokeniseString(synonymString, ',');

            for (unsigned int j = 0; j < synonyms.size(); j++) {
                string synonym = synonyms.at(j);

                auto entityMapping = ENTITY_MAP.find(entityName);
                if (entityMapping == ENTITY_MAP.end()) {
                    // SYNTAX ERROR: unknown design entity
                    query.status = "syntax error: unrecognised design entity in declaration";
                    return false;
                }

                auto synonymMapping = synonymTable.find(synonym);
                if (synonymMapping != ENTITY_MAP.end()) {
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

        regex SINGLE_TARGET("Select [A-Za-z][A-Za-z0-9]*");
        regex TUPLE_TARGET("Select <[A-Za-z][A-Za-z0-9]*(,\\s[A-Za-z][A-Za-z0-9]*)+>");
        smatch tmatch;

        // Attempt to match a single return type, otherwise match a tuple return type
        if (regex_search(queryBody, tmatch, SINGLE_TARGET)) {
            string targetEntity = tmatch.str().erase(0, 7);
            targets.push_back(targetEntity);
        }
        else if (regex_search(queryBody, tmatch, TUPLE_TARGET)) {
            // Retrieve first match - a string of form "Select <x1, x2, ...>"
            string tupleString = tmatch.str().erase(0, 8);
            tupleString.pop_back();

            // Extract all target entities in the tuple string "x1, x2, ..."
            targets = ParserUtils::tokeniseString(tupleString, ',');
        }
        else {
            query.status = "syntax error: target entity not correctly specified";
            return false;
        }

        // Query must have at least one return target
        if (targets.size() < 0) {
            query.status = "syntax error: missing query target";
        }
        else {
            query.targetEntities = targets;
        }

        return true;
    }

    bool QueryParser::parseRelationClauses(Query& query, vector<string> relationClauses) {
        return true;
    }

    bool QueryParser::parsePatternCluases(Query& query, vector<string> patternClauses) {
        return true;
    }

    QueryEvaluator::QueryEvaluator(PKB::PKB database) {
        this->database = database;
    }

    /**
     *  Returns the input string stripped of leading spaces and newline characters.
     *
     *  @param  input   input string to strip.
     *  @return left-trimmed copy of the input string.
     */
    string ParserUtils::leftTrim(string input) {
        return input.erase(0, input.find_first_not_of(" \n\r"));
    }

    /**
     *  Splits a string with a given delimiter once, returning a pair of prefix and
     *  suffix.
     *
     *  @param  input   input string to split.
     *  @param  delim   delimiting character.
     *  @return left-trimmed copy of the input string.
     */
    pair<string, string> ParserUtils::splitString(string input, char delim) {
        int pos = input.find_first_of(delim);
        return make_pair(input.substr(0, pos), input.substr(pos + 1, string::npos));
    }

    /**
     *  Splits a given input string with a given character as delimiter into trimmed
     *  substrings.
     *
     *  @param  input   input string to split.
     *  @param  delim   delimiting character.
     *  @return a vect<string> containing all trimmed split substrings.
     */
    vector<string> ParserUtils::tokeniseString(string input, char delim) {
        vector<string> substrings;
        string substr;

        istringstream stream(input);
        while (stream.good()) {
            getline(stream, substr, delim);
            string token = leftTrim(substr);
            if (token.length() > 0) {
                substrings.push_back(token);
            }
        }

        return substrings;
    }

    /**
     *  Performs two levels of regex matching on an input string with two separate
     *  regex patterns. Returns a vector of strings that match the second regex in
     *  strings that match the first regex.
     *
     *  @param  input   input string to perform dual regex matching.
     *  @param  first   first (outer) regex pattern, e.g structure of compound clauses.
     *  @param  second  second (inner) regex pattern, e.g. structure of atomic clauses.
     */
    vector<string> ParserUtils::dualMatch(string input, string first, string second) {
        vector<string> clauses;

        regex COMPOUND_CLAUSE(first);
        smatch ccmatch;

        while (regex_search(input, ccmatch, COMPOUND_CLAUSE)) {
            for (auto token : ccmatch) {
                string cclause = token.str();

                regex CLAUSE(second);
                smatch cmatch;

                while (regex_search(cclause, cmatch, CLAUSE)) {
                    for (auto atom : cmatch) {
                        clauses.push_back(ParserUtils::leftTrim(atom.str()));
                    }
                    cclause = cmatch.suffix().str();
                }
                input = ccmatch.suffix().str();
            }
        }

        return clauses;
    }
}
