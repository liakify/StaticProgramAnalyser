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
        query.status = "ok";
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

    // Validates synonyms correspond to an existing design entity in the synonym table
    // Validates allowed entity types as arguments to clauses
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
        string PATTERN_COMPOUND_CLAUSE = "pattern [A-Za-z][A-Za-z0-9]* \\([A-Za-z_,\"\\+\\-\\*\\/\\%\\s]*\\)(?: and [A-Za-z][A-Za-z0-9]* \\([A-Za-z_,\"\\+\\-\\*\\/\\%\\s]*\\))*";
        string RELATION_CLAUSE = "[A-Za-z*]+ \\([A-Za-z_,\\s]*\\)";
        string PATTERN_CLAUSE = "[A-Za-z][A-Za-z0-9]* \\([A-Za-z_,\"\\+\\-\\*\\/\\%\\s]*\\)";

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

    // Receives a vector of pattern clause strings and constructs pattern clause objects
    bool QueryParser::parsePatternCluases(Query& query, vector<string> patternClauses) {
        vector<PatternClause> patterns;

        for (auto clause : patternClauses) {
            // Each candidate pattern clause is of form <synonym> (arg1, arg2, [arg3 if while])

            pair<string, string> splitPair = ParserUtils::splitString(clause, ' ');
            string synonym = splitPair.first;
            string argString = splitPair.second.erase(0, 1);
            argString.pop_back();
            vector<string> args = ParserUtils::tokeniseString(argString, ',');

            auto synonymMapping = query.synonymTable.find(synonym);
            if (synonymMapping == query.synonymTable.end()) {
                // SEMANTIC ERROR: unknown synonym
                query.status = "semantic error: undeclared synonym in pattern clause";
                return false;
            }

            DesignEntity entityType = synonymMapping->second;
            string referenceString = args.at(0);

            // If first argument directly references a variable entity, validate it
            if (referenceString != "_" && referenceString.find('\"') != string::npos) {
                // String contains a " - interpret literally as variable name
                if (!ParserUtils::validateEntityRef(referenceString)) {
                    // SYNTAX ERROR: invalid entity reference
                    query.status = "syntax error: pattern has invalid entity reference";
                    return false;
                }
            }

            PatternClause pattern;

            switch (entityType) {
            case ASSIGN:
                if (args.size() != 2) {
                    // SYNTAX ERROR: incorrect number of arguments
                    query.status = "syntax error: assign pattern does not have 2 arguments";
                }
                else if (!ParserUtils::validatePattern(args.at(1))) {
                    // SYNTAX ERROR: invalid pattern string
                    query.status = "syntax error: assign pattern has invalid pattern string";
                }
                else {
                    pattern = { clause, ASSIGN_PATTERN, referenceString, args.at(1) };
                }
                break;
            case IF:
                if (args.size() != 2) {
                    // SYNTAX ERROR: incorrect number of arguments
                    query.status = "syntax error: if pattern does not have 2 arguments";
                }
                else if (args.at(1) != "_") {
                    // SYNTAX ERROR: unallowed argument for if pattern clause
                    query.status = "syntax error: if pattern only supports '_' as second argument";
                }
                else {
                    pattern = { clause, IF_PATTERN, referenceString, "_" };
                }
                break;
            case WHILE:
                if (args.size() != 3) {
                    // SYNTAX ERROR: incorrect number of arguments
                    query.status = "syntax error: while pattern does not have 3 arguments";
                }
                else if (args.at(1) != "_" || args.at(2) != "_") {
                    // SYNTAX ERROR: unallowed argument for while pattern clause
                    query.status = "syntax error: while pattern only supports '_' for last two arguments";
                }
                else {
                    // Pattern struct only stores first two args since third arg is fixed as '_' anyway
                    pattern = { clause, WHILE_PATTERN, referenceString, "_" };
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

    QueryEvaluator::QueryEvaluator(PKB::PKB database) {
        this->database = database;
    }

    /**
     *  Validates an input string as a direct entity reference.
     *
     *  @param  input   candidate entity reference string
     *  @return boolean describing if the string correctly references an entity.
     */
    bool ParserUtils::validateEntityRef(string input) {
        regex VALID_ENTITY_REFERENCE("^\"[A-Za-z][A-Za-z0-9]*\"$");
        smatch ematch;

        if (!regex_search(input, ematch, VALID_ENTITY_REFERENCE)) {
            // Entity reference is not a string of form "<identifier>"
            return false;
        }

        return true;
    }

    /**
     *  Validates an input string as a pattern, returning true if it is a valid pattern.
     *
     *  @param  input   candidate pattern string
     *  @return boolean describing if the string forms a valid pattern.
     */
    bool ParserUtils::validatePattern(string input) {
        if (input == "_") {
            return true;
        }

        // Regex describing an allowed infix arithemtic expression
        regex VALID_EXPRESSION("\"(?:(?:[0-9]+)|(?:[A-Za-z][A-Za-z0-9]*)(?: [\\+\\-\\*\\/\\%] (?:(?:[0-9]+)|(?:[A-Za-z][A-Za-z0-9]*)))*)\"");
        smatch pmatch;

        if (!regex_search(input, pmatch, VALID_EXPRESSION)) {
            // Arithmetic expression is not a substring
            return false;
        }

        string expr = pmatch.str();
        int diff = input.length() - expr.length();
        if (diff != 0 && diff != 2) {
            // Pattern string is not of form "<expr>" or [X]"<expr>"[Y] where X + Y = 2
            return false;
        }
        else if (diff == 2 && (input.at(0) != '_' || input.at(input.length() - 1) != '_')) {
            // Pattern string does not have symmetric '_'
            return false;
        }

        return true;
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
