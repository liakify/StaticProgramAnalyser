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
        // Use an optional capturing group for the underscore and substitute it at the end
        // Ensures symmetric pattern string (both underscores present or absent together)
        regex VALID_EXPRESSION("^(_?) *\" *(?:(?:[0-9]+)|(?:[A-Za-z][A-Za-z0-9]*)(?: *[\\+\\-\\*\\/\\%] *(?:(?:[0-9]+)|(?:[A-Za-z][A-Za-z0-9]*)))*) *\" *\\1$");
        smatch pmatch;

        if (!regex_search(input, pmatch, VALID_EXPRESSION)) {
            // Arithmetic expression is not a substring or pattern string is asymmetric
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

    string QueryUtils::stripPattern(string input) {
        regex STRIP_TARGET("[ \"]");
        smatch cmatch;

        return regex_replace(input, STRIP_TARGET, "");
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