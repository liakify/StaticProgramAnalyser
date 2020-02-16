#include "QueryUtils.h"

namespace PQL {

    bool QueryUtils::isInteger(string input) {
        regex VALID_INTEGER("^[0-9]+$");
        smatch imatch;

        return regex_search(input, imatch, VALID_INTEGER);
    }

    bool QueryUtils::isValidIdentifier(string input) {
        regex VALID_IDENTIFIER("^[A-Za-z][A-Za-z0-9]*$");
        smatch imatch;

        return regex_search(input, imatch, VALID_IDENTIFIER);
    }

    bool QueryUtils::isValidStmtRef(string input) {
        if (input == "_") {
            return true;
        }

        // Input string can either be an integer (referencing statement line number) or
        // an identifier (a synonym)
        return QueryUtils::isInteger(input) || QueryUtils::isValidIdentifier(input);
    }

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

    string QueryUtils::leftTrim(string input) {
        return input.erase(0, input.find_first_not_of(" \n\r"));
    }

    string QueryUtils::rightTrim(string input) {
        return input.erase(input.find_last_not_of(" \n\r") + 1);
    }

    string QueryUtils::trimString(string input) {
        return rightTrim(leftTrim(input));
    }

    pair<string, string> QueryUtils::splitString(string input, char delim) {
        int pos = input.find_first_of(delim);
        return { trimString(input.substr(0, pos)), trimString(input.substr(pos + 1, string::npos)) };
    }

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
