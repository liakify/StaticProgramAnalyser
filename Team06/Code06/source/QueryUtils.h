#pragma once

#include <regex>
#include <sstream>

#include "PQLTypes.h"

using std::istringstream;
using std::regex;
using std::smatch;

namespace PQL {
    
    class QueryUtils {
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
