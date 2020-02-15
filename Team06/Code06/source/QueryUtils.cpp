#include "QueryUtils.h"

namespace PQL {

    /**
     *  Validates an input string as a valid integer.
     *
     *  @param  input   candidate integer string
     *  @return boolean describing if the string is an integer.
     */
    bool QueryUtils::isInteger(string input) {
        regex VALID_INTEGER("^[0-9]+$");
        smatch imatch;

        return regex_search(input, imatch, VALID_INTEGER);
    }
    /**
     *  Validates an input string as a valid identifier or synonym.
     *
     *  @param  input   candidate identifier
     *  @return boolean describing if the string is a valid identifier.
     */
    bool QueryUtils::isValidIdentifier(string input) {
        regex VALID_IDENTIFIER("^[A-Za-z][A-Za-z0-9]*$");
        smatch imatch;

        return regex_search(input, imatch, VALID_IDENTIFIER);
    }

    /**
     *  Validates if an input string is a valid statement reference argument.
     *
     *  @param  input   candidate entity reference string
     *  @return boolean describing if the string correctly references an entity.
     */
    bool QueryUtils::isValidStmtRef(string input) {
        if (input == "_") {
            return true;
        }

        // Input string can either be an integer (referencing statement line number) or
        // an identifier (a synonym)
        return QueryUtils::isInteger(input) || QueryUtils::isValidIdentifier(input);
    }

    /**
     *  Validates if an input string is a valid entity reference argument.
     *
     *  @param  input   candidate entity reference string
     *  @return boolean describing if the string correctly references an entity.
     */
    bool QueryUtils::isValidEntityRef(string input) {
        if (input.find('\"') != string::npos) {
            // String contains a " - interpret literally as variable name
            regex VALID_ENTITY_REFERENCE("^\"[A-Za-z][A-Za-z0-9]*\"$");
            smatch ematch;

            // Entity reference is not a string of form "<identifier>"
            return regex_search(input, ematch, VALID_ENTITY_REFERENCE);
        }
        else {
            // String is either '_' or just "<synonym>" - validate identifier
            return input == "_" || QueryUtils::isValidIdentifier(input);
        }
    }

    /**
     *  Validates if an input string is a valid pattern.
     *
     *  @param  input   candidate pattern string
     *  @return boolean describing if the string forms a valid pattern.
     */
    bool QueryUtils::isValidPattern(string input) {
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
    string QueryUtils::leftTrim(string input) {
        return input.erase(0, input.find_first_not_of(" \n\r"));
    }

    /**
     *  Returns the input string stripped of trailing spaces and newline characters.
     *
     *  @param  input   input string to strip.
     *  @return right-trimmed copy of the input string.
     */
    string QueryUtils::rightTrim(string input) {
        return input.erase(input.find_last_not_of(" \n\r") + 1);
    }

    /**
     *  Returns the input string stripped of all leading and trailing spaces.
     *
     *  @param  input   input string to strip.
     *  @return left- and right-trimmed copy of the input string.
     */
    string QueryUtils::trimString(string input) {
        return rightTrim(leftTrim(input));
    }

    /**
     *  Splits a string with a given delimiter once, returning a pair of the
     *  trimmed prefix and suffix strings.
     *
     *  @param  input   input string to split.
     *  @param  delim   delimiting character.
     *  @return left-trimmed copy of the input string.
     */
    pair<string, string> QueryUtils::splitString(string input, char delim) {
        int pos = input.find_first_of(delim);
        return { trimString(input.substr(0, pos)), trimString(input.substr(pos + 1, string::npos)) };
    }

    /**
     *  Splits a given input string with a given character as delimiter into trimmed
     *  substrings.
     *
     *  @param  input   input string to split.
     *  @param  delim   delimiting character.
     *  @return a vect<string> containing all trimmed split substrings.
     */
    vector<string> QueryUtils::tokeniseString(string input, char delim) {
        vector<string> substrings;
        string substr;

        istringstream stream(input);
        while (stream.good()) {
            getline(stream, substr, delim);
            string token = trimString(substr);
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
    vector<string> QueryUtils::dualMatch(string input, string first, string second) {
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
                        clauses.push_back(QueryUtils::trimString(atom.str()));
                    }
                    cclause = cmatch.suffix().str();
                }
                input = ccmatch.suffix().str();
            }
        }

        return clauses;
    }
    
}
