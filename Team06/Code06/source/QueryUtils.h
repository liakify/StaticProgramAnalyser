#pragma once

#include <regex>
#include <sstream>

#include "PQLTypes.h"

using std::istringstream;
using std::regex;
using std::smatch;

namespace PQL {

    /**
     *  The QueryUtils class is a utility class containing only static methods
     *  in use by the Query Parser for string validation and manipulation. It
     *  should NOT be initialised.
     */
    class QueryUtils {
    public:
        /**
         *  Validates an input string as a valid integer.
         *
         *  @param      input       candidate integer string.
         *  @return     boolean describing if the string is an integer.
         */
        static bool isInteger(string input);

        /**
         *  Validates an input string as a valid identifier or synonym.
         *
         *  @param      input       candidate identifier.
         *  @return     boolean describing if the string is a valid identifier.
         */
        static bool isValidIdentifier(string input);

        /**
         *  Validates if an input string is a valid statement reference argument.
         *
         *  @param      input       candidate entity reference string.
         *  @return     boolean describing if the string correctly references an entity.
         */
        static bool isValidStmtRef(string input);

        /**
         *  Validates if an input string is a valid entity reference argument.
         *
         *  @param      input       candidate entity reference string.
         *  @return     boolean describing if the string correctly references an entity.
         */
        static bool isValidEntityRef(string input);

        /**
         *  Validates if an input string is a valid pattern string.
         *
         *  @param      input       candidate pattern string.
         *  @return     boolean describing if the string forms a valid pattern.
         */
        static bool isValidPattern(string input);

        /**
         *  Returns the input string stripped of leading spaces and newline characters.
         *
         *  @param      input       input string to strip.
         *  @return     left-trimmed copy of the input string.
         */
        static string leftTrim(string input);

        /**
         *  Returns the input string stripped of trailing spaces and newline characters.
         *
         *  @param      input       input string to strip.
         *  @return     right-trimmed copy of the input string.
         */
        static string rightTrim(string input);

        /**
         *  Returns the input string stripped of all leading and trailing spaces and newlines.
         *
         *  @param      input       input string to strip.
         *  @return     left- and right-trimmed copy of the input string.
         */
        static string trimString(string input);

        /**
         *  Strips an input pattern argument string of all whitespaces and quotation marks.
         *
         *  @param      input       input pattern string to strip.
         *  @return     an equivalent pattern string without spaces and quotation marks.
         */
        static string stripPattern(string input);

        /**
         *  Splits a string with a given delimiter once, returning a pair of the
         *  trimmed prefix and suffix strings. If the delimiting character is not found in
         *  the input string, returns a pair of the trimmed input string and an empty string.
         *
         *  @param      input       input string to split.
         *  @param      delim       delimiting character.
         *  @return     a pair containing the trimmed prefix and suffix strings.
         */
        static pair<string, string> splitString(string input, char delim);

        /**
         *  Splits a given input string with a given character as delimiter into trimmed
         *  substrings.
         *
         *  @param      input       input string to split.
         *  @param      delim       delimiting character.
         *  @return     a vector containing all trimmed split substrings.
         */
        static vector<string> tokeniseString(string input, char delim);

        /**
         *  Searches the input string and returns a vector of all strings that match the regular
         *  expression constructed from the input regex pattern.
         *
         *  @param      input       input string to perform regex matching.
         *  @param      pattern     regex pattern to match substrings with.
         *  @return     a vector containing all substring matches for the provided pattern.
         */
        static vector<string> matchAll(string input, string pattern);
    };

}
