#pragma once

#include <regex>
#include <sstream>

#include "PQLTypes.h"

namespace PQL {

    /**
     *  The QueryUtils namespace contains all utility methods in use by the Query
     *  Parser for string validation and manipulation.
     */
    namespace QueryUtils {
        
        /**
         *  Validates an input string as a valid integer.
         *
         *  @param      input       candidate integer string.
         *  @return     boolean describing if the string is an integer.
         */
        bool isInteger(std::string input);

        /**
         *  Validates an input string as a valid identifier or synonym.
         *
         *  @param      input       candidate identifier.
         *  @return     boolean describing if the string is a valid identifier.
         */
        bool isValidIdentifier(std::string input);

        /**
         *  Validates if an input string is a valid statement reference argument.
         *
         *  @param      input       candidate entity reference string.
         *  @return     boolean describing if the string correctly references an entity.
         */
        bool isValidStmtRef(std::string input);

        /**
         *  Validates if an input string is a valid entity reference argument.
         *
         *  @param      input       candidate entity reference string.
         *  @return     boolean describing if the string correctly references an entity.
         */
        bool isValidEntityRef(std::string input);

        /**
         *  Validates if an input string is a valid pattern string.
         *
         *  @param      input       candidate pattern string.
         *  @return     boolean describing if the string forms a valid pattern.
         */
        bool isValidPattern(std::string input);

        /**
         *  Returns the input string stripped of leading spaces and newline characters.
         *
         *  @param      input       input string to strip.
         *  @return     left-trimmed copy of the input string.
         */
        std::string leftTrim(std::string input);

        /**
         *  Returns the input string stripped of trailing spaces and newline characters.
         *
         *  @param      input       input string to strip.
         *  @return     right-trimmed copy of the input string.
         */
        std::string rightTrim(std::string input);

        /**
         *  Returns the input string stripped of all leading and trailing spaces and newlines.
         *
         *  @param      input       input string to strip.
         *  @return     left- and right-trimmed copy of the input string.
         */
        std::string trimString(std::string input);

        /**
         *  Strips an input pattern argument string of all whitespaces and quotation marks.
         *
         *  @param      input       input pattern string to strip.
         *  @return     an equivalent pattern string without spaces and quotation marks.
         */
        std::string stripPattern(std::string input);

        /**
         *  Splits a string with a given delimiter once, returning a pair of the
         *  trimmed prefix and suffix strings. If the delimiting character is not found in
         *  the input string, returns a pair of the trimmed input string and an empty string.
         *
         *  @param      input       input string to split.
         *  @param      delim       delimiting character.
         *  @return     a pair containing the trimmed prefix and suffix strings.
         */
        std::pair<std::string, std::string> splitString(std::string input, char delim);

        /**
         *  Splits a given input string with a given character as delimiter into trimmed
         *  substrings.
         *
         *  @param      input       input string to split.
         *  @param      delim       delimiting character.
         *  @return     a vector containing all trimmed split substrings.
         */
        std::vector<std::string> tokeniseString(std::string input, char delim);

        /**
         *  Searches the input string and returns a vector of all strings that match the regular
         *  expression constructed from the input regex pattern. Does not return any sub-strings
         *  corresponding to capture groups in the regex.
         *
         *  @param      input       input string to perform regex matching.
         *  @param      pattern     regex pattern to match substrings with.
         *  @return     a vector containing all substring matches for the provided pattern.
         */
        std::vector<std::string> matchAll(std::string input, std::string pattern);
    };

}
