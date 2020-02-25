#include "stdafx.h"
#include "QueryParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PQL;

namespace UnitTesting {

	QueryParser parser;

	TEST_CLASS(TestQueryParser) {
	public:
		string TRIVIAL_QUERY = "constant c; Select c";
		string MANY_DECLARATIONS_TRIVIAL_QUERY = "while procedure, constant; assign stmt; if print; read variable; assign such; Select variable";
		string SIMPLE_VALID_QUERY = "variable v; if ifs; assign a; Select v such that Parent* (ifs, a) pattern a(v, _)";
		string COMPLEX_VALID_QUERY = "assign pattern; while Modifies; variable while, constant; Select pattern such that Uses (Modifies, while) pattern pattern(constant, _\"1\"_)";

		// Invalid queries that fail in validateQuerySyntax
		string EMPTY_QUERY = "";
		string INCORRECT_DECLARATION_QUERY = "read r1 & r2; Select r1";
		string MISSING_SELECT_QUERY = "stmt s; select s";
		string ENDING_SEMICOLON_QUERY = "procedure p; Select p;";
		string FOREIGN_CHARACTER_QUERY = "constant c; Select c < 100";
		string INVALID_AND_RELATION_QUERY = "while w1, w2; Select w1 such that Parent(w1, w2) and such that Follows(w2, _)";
		string INVALID_AND_PATTERN_QUERY = "assign a1, a2; variable v; Select v pattern a1(v, _) and pattern a2(v, _)";

		// Invalid queries that fail in validateQuerySemantics
		string MISSING_RETURN_TYPE_SYNONYM_QUERY = "if ifs; read rd; Select v such that Parent(ifs, rd)";
		string AMBIGUOUS_WILDCARD_QUERY = "variable v; Select v such that Modifies(_, v)";
		string USES_MISSING_SYNONYM_QUERY = "variable v; Select v such that Uses(i, v)";
		string USES_NON_VARIABLE_ARG_QUERY = "constant c; procedure p; Select a1 such that Uses(p, c)";
		string INVALID_LINE_NUMBER_QUERY = "stmt s; Select s such that Parent(0, s)";
		string FOLLOWS_MISSING_SYNONYM_QUERY = "print pn; Select pn such that Follows(rd, pn)";
		string FOLLOWS_NON_STATEMENT_ARG_QUERY = "stmt s1; Select s1 such that Parent*(s1, \"function\")";
		string PATTERN_MISSING_SYNONYM_QUERY = "assign a; Select a pattern a(v, \"x\")";
		string PATTERN_NON_VARIABLE_ARG_QUERY = "call cl; assign a; Select cl pattern a(cl, _)";

		// Invalid queries that fail in splitConstraints
		string INCORRECT_KEYWORD_QUERY = "stmt s; print p; Select p such tat Follows*(s, p)";
		string INCORRECT_CLAUSE_SYNTAX_QUERY = "read rd; print pn; assign a; Select a pattern(_, _) and Follows(rd, pn)";

		// Invalid queries that fail in parseDeclarations
		string UNRECOGNISED_DESIGN_ENTITY_QUERY = "while w1, w2; program pr; Select pr such that Follows(w1, w2)";
		string CONFLICTING_SYNONYM_QUERY = "constant c; procedure p; call c; Select p";

		// Invalid queries that fail in parseQueryTarget
		string MISSING_RETURN_TYPE_QUERY = "print pn; stmt s1; Select such that Follows(pn, s1)";
		string MULTIPLE_RETURN_TYPE_QUERY = "if ifs; assign a; Select ifs, a such that Parent*(ifs, a)";

		// Invalid queries that fail in parseRelationClauses
		string INCORRECT_RELATION_NUM_ARGS_QUERY = "stmt s1, s2, s3; Select s1 such that Parent(s1, s2, s3)";
		string FOLLOWS_NON_STATEMENT_REF_ARG_QUERY = "assign a; Select a such that Parent*(\"x\", a)";
		string USES_NON_ENTITY_REF_ARG_QUERY = "assign a; Select a such that Uses(a, 420)";
		string USES_INVALID_FIRST_ARG_QUERY = "read rd; variable v; Select rd such that Uses(\"2\", v)";
		string INVALID_RELATION_KEYWORD_QUERY = "read rd; print pn; Select s1 such that Folows(rd, pn)";

		// Invalid queries that fail in parsePatternClauses
		string PATTERN_UNDECLARED_SYNONYM_QUERY = "variable v; Select v pattern a(v, _)";
		string PATTERN_INVALID_ENTITY_REF_QUERY = "assign a; Select a pattern a(1, _)";
		string INCORRECT_ASSIGN_PATTERN_NUM_ARGS_QUERY = "assign a; Select a pattern a(\"x\", _, _)";
		string INVALID_PATTERN_STRING_QUERY = "assign a; variable v, v1; Select v pattern a(\"notFound\", v1)";
		string INVALID_PATTERN_TYPE_QUERY = "call c; Select c pattern c(\"main\", _)";

		vector<string> VALID_QUERIES = {
			TRIVIAL_QUERY, MANY_DECLARATIONS_TRIVIAL_QUERY, SIMPLE_VALID_QUERY, COMPLEX_VALID_QUERY
		};

		// Array containing all invalid queries with a single deviation from the syntax grammar or
		// proper query semantics
		vector<string> INVALID_QUERIES = {
			EMPTY_QUERY, INCORRECT_DECLARATION_QUERY, MISSING_SELECT_QUERY, ENDING_SEMICOLON_QUERY,
			FOREIGN_CHARACTER_QUERY, INVALID_AND_RELATION_QUERY, INVALID_AND_PATTERN_QUERY,
			MISSING_RETURN_TYPE_SYNONYM_QUERY, AMBIGUOUS_WILDCARD_QUERY, USES_MISSING_SYNONYM_QUERY,
			USES_NON_VARIABLE_ARG_QUERY, INVALID_LINE_NUMBER_QUERY, FOLLOWS_MISSING_SYNONYM_QUERY,
			FOLLOWS_NON_STATEMENT_ARG_QUERY, PATTERN_MISSING_SYNONYM_QUERY, PATTERN_NON_VARIABLE_ARG_QUERY,
			INCORRECT_KEYWORD_QUERY, INCORRECT_CLAUSE_SYNTAX_QUERY,
			UNRECOGNISED_DESIGN_ENTITY_QUERY, CONFLICTING_SYNONYM_QUERY,
			MISSING_RETURN_TYPE_QUERY, MULTIPLE_RETURN_TYPE_QUERY,
			INCORRECT_RELATION_NUM_ARGS_QUERY, FOLLOWS_NON_STATEMENT_REF_ARG_QUERY,
			USES_NON_ENTITY_REF_ARG_QUERY, USES_INVALID_FIRST_ARG_QUERY, INVALID_RELATION_KEYWORD_QUERY,
			PATTERN_UNDECLARED_SYNONYM_QUERY, PATTERN_INVALID_ENTITY_REF_QUERY,
			INCORRECT_ASSIGN_PATTERN_NUM_ARGS_QUERY, INVALID_PATTERN_STRING_QUERY, INVALID_PATTERN_STRING_QUERY
		};

		TEST_CLASS_INITIALIZE(initParser) {
			parser = PQL::QueryParser();
		}
		TEST_METHOD(parseQueryValidCases) {
			for (auto query : VALID_QUERIES) {
				Query result = parser.parseQuery(query);
				Assert::IsTrue(result.status == "success");
			}
		}
		TEST_METHOD(parseQueryInvalidCases) {
			for (auto testcase : INVALID_QUERIES) {
				Query result = parser.parseQuery(testcase);
				Assert::IsTrue(result.status.find("error") != string::npos);
			}
		}

	};

}
