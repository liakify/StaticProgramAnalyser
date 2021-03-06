#include "QueryUtils.h"

using std::istringstream;
using std::pair;
using std::regex;
using std::smatch;
using std::string;
using std::vector;

namespace PQL {

    bool QueryUtils::isValidInteger(string input) {
        regex VALID_INTEGER("^[0-9]+(?!\n)$");
        smatch imatch;

        return regex_search(input, imatch, VALID_INTEGER);
    }

    bool QueryUtils::isValidIdentifier(string input) {
        regex VALID_IDENTIFIER("^[A-Za-z][A-Za-z0-9]*(?!\n)$");
        smatch imatch;

        return regex_search(input, imatch, VALID_IDENTIFIER);
    }

    bool QueryUtils::isValidLiteral(string input) {
        regex VALID_ENTITY_REFERENCE("^\"\\s*[A-Za-z][A-Za-z0-9]*\\s*\"(?!\n)$");
        smatch ematch;

        // Literal reference is not a string of form "<identifier>"
        return regex_search(input, ematch, VALID_ENTITY_REFERENCE);
    }

    bool QueryUtils::isValidStmtRef(string input) {
        if (input == "_") {
            return true;
        }

        // Input string can either be an integer (referencing statement line number) or
        // an identifier (a synonym)
        return isValidInteger(input) || isValidIdentifier(input);
    }

    bool QueryUtils::isValidEntityRef(string input) {
        if (input.find('\"') != string::npos) {
            // String contains a " - interpret literally as a variable or procedure name
            return isValidLiteral(input);
        } else {
            // String is either '_' or just "<synonym>" - validate identifier
            return input == "_" || isValidIdentifier(input);
        }
    }

    bool QueryUtils::isValidAttrRef(string input) {
        regex VALID_ATTRIBUTE_REF("^[A-Za-z][A-Za-z0-9]*\\s*\\.\\s*[A-Za-z#]+(?!\n)$");
        smatch amatch;

        return regex_search(input, amatch, VALID_ATTRIBUTE_REF);
    }

    bool QueryUtils::isValidRef(string input) {
        if (input == "_") {
            return false;
        } else if (input.find('\"') != string::npos) {
            // String contains a " - interpret literally as a variable or procedure name
            return isValidLiteral(input);
        } else {
            // Input string can either be an integer, identifier (a synonym) or attribute ref
            return isValidInteger(input) || isValidIdentifier(input) || isValidAttrRef(input);
        }
    }

    string QueryUtils::leftTrim(string input) {
        return input.erase(0, input.find_first_not_of(" \t\f\v\n\r"));
    }

    string QueryUtils::rightTrim(string input) {
        return input.erase(input.find_last_not_of(" \t\f\v\n\r") + 1);
    }

    string QueryUtils::trimString(string input) {
        return rightTrim(leftTrim(input));
    }

    string QueryUtils::stripPattern(string input) {
        regex STRIP_TARGET("[\\s\"]");
        smatch cmatch;

        return regex_replace(input, STRIP_TARGET, "");
    }

    pair<string, string> QueryUtils::splitString(string input, char delim) {
        int pos = input.find_first_of(delim);
        if (pos == string::npos) {
            // Delimiting character not found
            return { trimString(input), "" };
        }

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

    vector<string> QueryUtils::matchAll(string input, string pattern) {
        vector<string> matches;

        regex INPUT_PATTERN(pattern);
        smatch ssmatch;

        while (regex_search(input, ssmatch, INPUT_PATTERN)) {
            string token = ssmatch.str();
            matches.push_back(trimString(token));
            input = ssmatch.suffix().str();
        }

        return matches;
    }

}
