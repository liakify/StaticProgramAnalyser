#include "stdafx.h"
#include "QueryUtils.h"

using std::pair;
using std::string;
using std::vector;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PQL;

namespace UnitTesting {

	TEST_CLASS(TestQueryUtils) {
		public:
		string EMPTY_STRING = "";

		vector<string> VALID_INTEGERS = { "0", "3203", "00000000" };
		vector<string> INVALID_INTEGERS = { EMPTY_STRING, "-1", "3216 ", " 3217", "20 40", "0xF" };

		vector<string> VALID_IDENTIFIERS = { "A", "fOr", "softwareEng1n33r1ng", "CS3203" };
		vector<string> INVALID_IDENTIFIERS = { EMPTY_STRING, "1", "3203S", " Give", "us ", "A+", "thank you", "xoxo_team6" };

		vector<string> VALID_STMT_REFS = { "_" , "3281", "dAmYtH" };
		vector<string> INVALID_STMT_REFS = { EMPTY_STRING, "-65536", "2040S" };

		vector<string> VALID_ENTITY_REFS = { "_", "synonym", "\"quotes\"", "\"  varQuotes \"" };
		vector<string> INVALID_ENTITY_REFS = { EMPTY_STRING, "1", "1231S", " space ", "\"noEnd", "noStart\"", "\"\"dupes\"\"", "\"expr + 1\"" };

		vector<string> VALID_PATTERNS = {
			// Wildcard pattern
			"_",
			// Exact patterns with well-formed expressions
			"\"nCOV19\"", "\"big % nUiSaNcE\"", "\"S + U - this * Mod / plox % thanks\"", "\"b - B + bB * Bb / b9 \"",
			// Inclusive patterns with well-formed expressions
			"_\"SARSagain\"_", "_\"deaths + 1\"_", "_\"big / KEK * 420\"_",
			// Exact patterns with variably spaced expressions
			"  \"frontSpace\"", "\"backSpace\" ", "  \"both\" ", "\"420+69\"", "\" starts+here  \"", "\"fib/1 + 1*   two  - Thr33   \"",
			// Inclusive patterns with variably spaced expressions
			"_ \"leftOnly\"_", "_\"rightOnly\" _", "_ \"b0th / S1des\" _", "_ \" 3230 %   D *c - b+a\"  _"
		};
		vector<string> INVALID_PATTERNS = {
			// Missing or unmatched quotation marks
			EMPTY_STRING, "3 + 5 - 7", "_b * b - 4 * A * c", "\"noClosing % quotation", "_missing * start\"_"
			// Quoted wildcards
			"\"_\"", "_\"_\"_",
			// Invalid identifiers
			"\"2good4me\"", "_\"1WO / eNc1k\"_", "\"pascal_case - sucks\"", "\"github.com\"", "_\"wat?\"_", "\"split\n - line\"",
			// Unrecognised operators
			"\"2 ^ 32\"", "_\"x ** x\"_", "\"x < 3\"", "_\"now == deadline\"_", "\"beep - (boop + bop)\"", "_\" n %= 1000000007\"_", "\"1 && 0\"",
			// Invalid arithmetic expressions
			"_\"A +\"_", "\"- 31415\"", "\"i++\"", "_\"20 20\"_", "\"taiWan / numba * 1 +\"",
			// Untrimmed inclusive pattern strings
			" _\"hello\"_", "_\"its\"_ ", " _\"meeeeee\"_ ", " _\"i % was - wondering * if\"_  ", "_\"asymmetr1C \" _  ",
			// Misplaced quotes or inclusive pattern underscores
			"\"_inside_\"", "outside + \"inside\"", "\"looks\" + _\"cl0se\"_"
			// Unmatched inclusive pattern underscores
			"_\"missingRight\"", "\"missingLeft\"_", "  _ \"1\" ", "\"unmatchedHalf \"_"
			// Supernumary inclusive pattern underscores
			"_\"x + y\"__", "_ _\"x86ISA\"_", "_ _\"ARM\"_  _"
		};

		vector<pair<string, string>> LEFT_TRIM_TESTS = {
			{ EMPTY_STRING, EMPTY_STRING },
			{ "var", "var" },
			{ " left", "left" },
			{ "      many_Left", "many_Left" },
			{ "right ", "right " },
			{ " multiple words", "multiple words" },
			{ "\n\rlineBreaks", "lineBreaks" },
			{ "\r left & right \n", "left & right \n" }
		};

		vector<pair<string, string>> RIGHT_TRIM_TESTS = {
			{ EMPTY_STRING, EMPTY_STRING },
			{ "test", "test" },
			{ "right ", "right" },
			{ "many.Right    ", "many.Right" },
			{ " left ", " left" },
			{ "split tokens  ", "split tokens" },
			{ "lineBreaks\n\r", "lineBreaks" },
			{ "\r left and right \n", "\r left and right" }
		};

		vector<pair<string, string>> TRIMMED_STRING_TESTS = {
			{ EMPTY_STRING, EMPTY_STRING },
			{ "idempot3nt", "idempot3nt" },
			{ "\n \rfront & bacc _ g0n3  \r \n", "front & bacc _ g0n3" },
			{ "< div&= >\n", "< div&= >" }
		};

		vector<pair<string, string>> STRIPPED_PATTERN_TESTS = {
			{ EMPTY_STRING, EMPTY_STRING },
			{ "\"x\"", "x" },
			{ "_\"DORSCON - orang3\"_", "_DORSCON-orang3_" },
			{ "\" pls * 1  -star+rep0\"    ", "pls*1-star+rep0" },
			{ " _\"n0ne+ l3ft *righT % both -  many  \" _","_n0ne+l3ft*righT%both-many_" },
			{ "_  \"      wutDis  -1\" _  " , "_wutDis-1_" }
		};

		vector<pair<pair<string, char>, pair<string, string>>> SPLIT_STRING_TESTS = {
			{ { EMPTY_STRING, ' ' }, { EMPTY_STRING, EMPTY_STRING } },
			{ { "a b ", ' ' }, { "a", "b" } },
			{ { "a b c", ' ' }, { "a", "b c" } },
			{ { "  p q l ", ' ' }, { "", "p q l" } },
			{ { " x ,y, z  ", ',' }, { "x", "y, z" } },
			{ { " p k b ", '.' }, { "p k b", EMPTY_STRING } },
			{ { "std::out << var;", ':' }, { "std", ":out << var;" } },
			{ { " <div> text </div>  ", '>' }, { "<div", "text </div>" } },
			{ { "x++; y--; z += y ; ", ';' }, { "x++", "y--; z += y ;" } },
			{ { "Modifies ( stt, var) ", '(' }, { "Modifies", "stt, var)" } },
			{ { "Select < x, BOOLEAN, var > ", '<' }, { "Select", "x, BOOLEAN, var >" } }
		};
		
		vector<pair<pair<string, char>, vector<string>>> TOKENISED_STRING_TESTS = {
			{ { EMPTY_STRING, ' ' }, { } },
			{ { "no_space", ' ' }, { "no_space" } },
			{ { "  www.google.com ", '/' }, { "www.google.com" } },
			{ { "a la carbonara", 'a' }, { "l", "c", "rbon", "r" } },
			{ { " t z  e r   b i n ", ' ' }, { "t", "z", "e", "r", "b", "i", "n" } },
			{ { "<p> <b> sample HTML </b> </p>", '>' }, { "<p", "<b", "sample HTML </b", " </p" } },
			{ { " <tuple,p, q , r > ", ',' }, { "<tuple", "p", "q", "r >" } },
			{ { "pattern ( Uses ,_, _) ", ',' }, { "pattern ( Uses", "_", "_)" } },
			{ { " assign a1", ',' }, { "assign a1" } },
			{ { " stmt s ,s1, s2, s3 ", ',' }, { "stmt s", "s1", "s2", "s3" } },
			{ { "stmt s1,s2 ;call c; ifs  i1,   i2 ;  while w  ; Select s1", ';' }, { "stmt s1,s2", "call c", "ifs  i1,   i2", "while w", "Select s1" } }
		};

		vector<pair<pair<string, string>, vector<string>>> REGEX_MATCHER_TESTS = {
			{ { EMPTY_STRING, ".+" }, { } },
			{ { "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "[0-9]" }, { } },
			{ { "a -> b -> 3 <- d -> e <- f -> 6", "[A-Za-z]+(?! <-)" }, { "a", "b", "d", "f" } },
			{ { "<p><b><i> bolded text </p></b></i>", "</?[A-Za-z]+>" }, { "<p>", "<b>", "<i>", "</p>", "</b>", "</i>" } },
			{ { "recurse(M[0:N/2], M[N/2 + 1:N], R[0:N])", "\\[[A-Za-z0-9\\+\\-\\*\\/\\%\\s:]+\\]" }, { "[0:N/2]", "[N/2 + 1:N]", "[0:N]" } }
		};

		TEST_METHOD(isInteger) {
			for (auto input : VALID_INTEGERS) {
				Assert::IsTrue(QueryUtils::isInteger(input));
			}
			for (auto input : INVALID_INTEGERS) {
				Assert::IsFalse(QueryUtils::isInteger(input));
			}
		}

		TEST_METHOD(isValidIdentifier) {
			for (auto input : VALID_IDENTIFIERS) {
				Assert::IsTrue(QueryUtils::isValidIdentifier(input));
			}
			for (auto input : INVALID_IDENTIFIERS) {
				Assert::IsFalse(QueryUtils::isValidIdentifier(input));
			}
		}

		TEST_METHOD(isValidStmtRef) {
			for (auto input : VALID_STMT_REFS) {
				Assert::IsTrue(QueryUtils::isValidStmtRef(input));
			}
			for (auto input : INVALID_STMT_REFS) {
				Assert::IsFalse(QueryUtils::isValidStmtRef(input));
			}
		}

		TEST_METHOD(isValidPattern) {
			for (auto input : VALID_PATTERNS) {
				Assert::IsTrue(QueryUtils::isValidPattern(input));
			}
			for (auto input : INVALID_PATTERNS) {
				Assert::IsFalse(QueryUtils::isValidPattern(input));
			}
		}

		TEST_METHOD(trimMethods) {
			for (auto testcase : LEFT_TRIM_TESTS) {
				Assert::IsTrue(QueryUtils::leftTrim(testcase.first) == testcase.second);
			}
			for (auto testcase : RIGHT_TRIM_TESTS) {
				Assert::IsTrue(QueryUtils::rightTrim(testcase.first) == testcase.second);
			}
			for (auto testcase : TRIMMED_STRING_TESTS) {
				Assert::IsTrue(QueryUtils::trimString(testcase.first) == testcase.second);
			}
		}

		TEST_METHOD(stripPattern) {
			for (auto testcase : STRIPPED_PATTERN_TESTS) {
				Assert::IsTrue(QueryUtils::stripPattern(testcase.first) == testcase.second);
			}
		}

		TEST_METHOD(splitString) {
			for (auto testcase : SPLIT_STRING_TESTS) {
				Assert::IsTrue(QueryUtils::splitString(testcase.first.first, testcase.first.second) == testcase.second);
			}
		}

		TEST_METHOD(tokeniseString) {
			for (auto testcase : SPLIT_STRING_TESTS) {
				Assert::IsTrue(QueryUtils::splitString(testcase.first.first, testcase.first.second) == testcase.second);
			}
		}
		
		TEST_METHOD(matchAll) {
			for (auto testcase : REGEX_MATCHER_TESTS) {
				Assert::IsTrue(QueryUtils::matchAll(testcase.first.first, testcase.first.second) == testcase.second);
			}
		}

	};
}
