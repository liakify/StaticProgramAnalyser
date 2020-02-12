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

    bool QueryParser::validateQuerySyntax(string queryString) {
        // Validate each declaration is of form <design-entity> <synonym>(, <synonym>)*;
        return true;
    }

    bool QueryParser::validateQuerySemantics(Query& query) {
        return true;
    }

    vector<string> QueryParser::splitStatements(string queryString) {
        vector<string> statements;
        return statements;
    }

    // Assumes syntactically invalid compound clauses do not exist after syntax checking, e.g.
    // Select a such that Modifies (a, v) and such that Parent (a, w) and pattern (v, _)
    // C++ regex does not support negative lookbehind, (?<!(x))y - match y only if not preceded by x
    // Returns an array of clauses
    pair<vector<string>, vector<string>> QueryParser::splitConstraints(string queryBody) {
        vector<string> relationClauses, patternClauses;
        return make_pair(relationClauses, patternClauses);
    }

    bool QueryParser::parseDeclarations(Query& query, vector<string> statements) {
        return true;
    }

    bool QueryParser::parseQueryTarget(Query& query, string queryBody) {
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
     *  @param  input   input string to strip.
     *  @return left-trimmed copy of the input string.
     */
    string ParserUtils::leftTrim(string input) {
        return input.erase(0, input.find_first_not_of(" \n\r"));
    }

    /**
     *  Splits a given input string with a given character as delimiter into trimmed
     *  substrings.
     *  @param  input   input string to split.
     *  @param  delim   delimiting character
     *  @return a vect<string> containing all trimmed split substrings
     */
    vector<string> ParserUtils::splitString(string input, char delim) {
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
     *  @param  input   input string to perform dual regex matching.
     *  @param  first   first (outer) regex pattern, e.g structure of compound clauses
     *  @param  second  second (inner) regex pattern, e.g. structure of atomic clauses
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
