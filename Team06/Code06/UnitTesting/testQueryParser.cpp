#include "stdafx.h"
#include "QueryParser.h"

using std::string;
using std::vector;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PQL;

namespace UnitTesting {

    QueryParser parser;

    TEST_CLASS(TestQueryParser) {
    public:
        // All valid queries
        string TRIVIAL_SYNONYM_QUERY = "constant Select; Select Select";
        string TRIVIAL_BOOLEAN_QUERY = "Select BOOLEAN";
        string TRIVIAL_ATTRIBUTE_QUERY = "print pn; Select pn.varName";
        string TRIVIAL_MANY_DECLARATIONS_QUERY = "while procedure, constant; assign call; if print; read variable; call such; Select variable";
        string TRIVIAL_ONE_TUPLE_QUERY = "prog_line l; Select <l>";
        string TRIVIAL_TUPLE_QUERY = "procedure p; variable BOOLEAN; Select <p, BOOLEAN> such that Modifies (p, BOOLEAN)";
        string TRIVIAL_TUPLE_ATTRIBUTES_QUERY = "constant procName; variable value; procedure stmtNum; read varName; Select <varName.stmt#, value.varName, stmtNum.procName, procName.value>";
        string TRIVIAL_REDUNDANT_ATTRIBUTE_QUERY = "if ifs; Select <ifs, ifs.stmt#>";
        string TRIVIAL_TUPLE_MIXED_QUERY = "call cl; print pn; Select <cl, cl.procName, pn, pn.varName>";
        string SIMPLE_CONSTANTS_QUERY = "assign a; Select a such that Follows*(4, 9) pattern a(\"x\", _\"(0)\"_)";
        string SIMPLE_VALID_QUERY = "variable v; if ifs; assign a; Select v such that Parent(ifs, a) pattern a(v, _)";
        string SIMPLE_VALID_BOOLEAN_QUERY = "stmt progline; prog_line stmt; Select BOOLEAN such that Follows*(stmt, progline)";
        string SIMPLE_VALID_ATTRIBUTES_QUERY = "while w; read rd; Select <w.stmt#, rd, rd.varName> such that Parent*(w, rd)";
        string SIMPLE_PATTERN_STRING_QUERY = "assign a; Select a pattern a(_, \"x + 1 - 2 * y / z % 3\")";
        string COMPLEX_VALID_QUERY = "assign pattern; while Modifies; variable while, constant; Select pattern such that Uses(Modifies, while) pattern pattern(constant, _\"1\"_)";
        string COMPLEX_VALID_ATTRIBUTES_QUERY = "assign a; call cl; variable v; Select <a, cl, cl.procName, v.varName> pattern a(v, _) such that Follows(a, cl)";
        string COMPLEX_PATTERN_STRING_QUERY = "assign a; Select a pattern a(\"var\", _\"((p) - (q / 2) % r) * 3 - ((s + t % 5) - u) + v / 7\"_)";
        string CHAINED_BOOLEAN_QUERY = "Select BOOLEAN such that Parent*(16, 25) and Uses(16, \"i\") and Modifies(25, \"i\")";
        string CHAINED_BASIC_RELATIONS_QUERY = "prog_line l; variable v; if ifs; read rd; Select <l, rd, v> such that Follows(l, ifs) and Parent*(ifs, rd) such that Uses(ifs, v) and Modifies(rd, v)";
        string CHAINED_PROCEDURE_RELATIONS_QUERY = "Select BOOLEAN such that Modifies(\"main\", \"argv\") and Uses(\"main\", \"argv\")";
        string CHAINED_MIXED_CLAUSES_QUERY = "assign a; procedure p; variable v1, v2; while w; if ifs; prog_line l; Select <p, w, a> pattern a(v1, \"0\") and w(v1, _) such that Follows*(a, ifs) pattern ifs(v2, _, _) such that Modifies(p, v2) and Parent(ifs, l) and Uses(l, v1)";
        string CHAINED_CALLS_QUERY = "procedure p, q; Select <p, q> such that Calls(\"main\", p) and Calls*(p, q) and Calls(q, _)";
        string CHAINED_NEXT_QUERY = "prog_line l; call cl; Select <l, cl> such that Next*(_, l) and Next(l, cl) and Next*(cl, 25)";
        string CHAINED_AFFECTS_QUERY = "stmt s; assign a; Select <s, a> such that Affects*(1, s) and Affects*(a, s) and Affects(s, _)";
        string CHAINED_STMT_PROG_LINE_QUERY = "stmt s; prog_line l1, l2; variable v; Select <l1, v> such that Parent*(l1, s) and Next*(s, l2) and Affects(l2, l1) and Uses(l1, v) and Modifies(l1, _)";
        string CHAINED_MIXED_RETURN_QUERY = "constant c; call cl; read rd; variable v; Select <c.value, cl.stmt#, cl.procName, rd.varName, v> such that Next*(cl, rd) and Modifies(rd, v) such that Calls(\"init\", _)";
        string CHAINED_ADV_RELATIONS_QUERY = "prog_line l; call cl; assign a; variable v; Select <l, a, v> such that Uses(\"error\", v) and Modifies(a, v) such that Affects(l, a) and Next*(a, cl) and Calls*(_, \"error\")";
        string CHAINED_ALL_RELATIONS_QUERY = "procedure p; stmt s; prog_line l; call cl; assign a; variable v; Select <p, v> such that Calls(_, p) and Uses(p, v) and Parent(s, cl) and Modifies(cl, v) and Follows*(cl, l) and Next(l, a) and Affects*(a, a)";
        string CHAINED_ALL_PATTERNS_QUERY = "while w; assign a; if ifs; variable v; Select <a, v> pattern ifs(v, _, _) and a(\"x\", _) pattern w(v, _)";
        string CHAINED_VARYING_WHITESPACE_QUERY = "\nprocedure  p\f;\rwhile\tw ;assign\va\r;  variable\fv1,  v2\n;\tcall\rcl\t;\nSelect\f<\vp. procName\r,\tw\n,  cl\f,\ncl\t.\rprocName\v,\fa >\n pattern\ta(  v1,\r_\"\t69-\v 420\"_ )\nsuch  that\fModifies\r(p\t,v1)\fand\rUses(\tw\v,  v1\n)  pattern\tw  (v2, _\v\f) such\t\tthat  Next*\f (\ta, w\v)\rand\tFollows(\na\r,\fcl  )\n";

        // Invalid queries that fail in validateQuerySyntax
        string EMPTY_QUERY = "";
        string INCORRECT_DECLARATION_QUERY = "read r1 & r2; Select r1";
        string DUPLICATE_BODY_QUERY = "prog_line l1, l2; Select l1; Select l2";
        string MISSING_BODY_SUFFIX_QUERY = "read rd; Select ";
        string MISSING_SELECT_QUERY = "stmt s; select s";
        string ENDING_SEMICOLON_QUERY = "procedure p; Select p;";
        string FOREIGN_CHARACTER_QUERY = "constant c; Select | c";
        string INVALID_AND_RELATION_QUERY = "while w1, w2; Select w1 such that Parent(w1, w2) and such that Follows(w2, _)";
        string INVALID_AND_PATTERN_QUERY = "assign a1, a2; variable v; Select v pattern a1(v, _) and pattern a2(v, _)";

        // Invalid queries that fail in validateQuerySemantics
        string AMBIGUOUS_BOOLEAN_QUERY = "stmt BOOLEAN; Select BOOLEAN such that Uses(BOOLEAN, _)";
        string INCORRECT_BOOLEAN_KEYWORD_QUERY = "Select Boolean";
        string MISSING_RETURN_TYPE_SYNONYM_QUERY = "if ifs; read rd; Select v such that Parent(ifs, rd)";
        string MISSING_BOOLEAN_IN_TUPLE_QUERY = "Select <BOOLEAN>";
        string MISSING_SYNONYM_IN_TUPLE_QUERY = "prog_line l; while BOOLEAN; Select <BOOLEAN, ifs> such that Parent(BOOLEAN, l)";
        string INVALID_PROG_LINE_ATTRIBUTE_QUERY = "prog_line l; Select l.stmt# such that Follows(l, 4231)";
        string INVALID_SYNONYM_ATTRIBUTE_QUERY = "assign a; Select a.varName pattern a(_, \"0\")";
        string INVALID_SYNONYM_ATTRIBUTE_IN_TUPLE_QUERY = "call cl; print pn; Select <cl, cl.procName, pn.value> such that Next(cl, pn)";
        string AMBIGUOUS_WILDCARD_QUERY = "variable v; Select v such that Modifies(_, v)";
        string USES_MISSING_FIRST_SYNONYM_QUERY = "variable v; Select v such that Uses(ifs, v)";
        string USES_UNDEFINED_RELATION_QUERY = "read rd; Select BOOLEAN such that Uses(rd, \"var\")";
        string MODIFIES_UNDEFINED_RELATION_QUERY = "print pn; Select pn such that Modifies(pn, _)";
        string MODIFIES_MISSING_SECOND_SYNONYM_QUERY = "procedure p; Select p such that Modifies(p, v)";
        string USES_NON_VARIABLE_ARG_QUERY = "constant c; prog_line l; Select l such that Uses(l, c)";
        string CALLS_MISSING_SYNONYM_QUERY = "Select BOOLEAN such that Calls(p, _)";
        string CALLS_NON_PROCEDURE_ARG_QUERY = "call cl; Select cl such that Calls*(\"main\", cl)";
        string INVALID_STMT_NUMBER_QUERY = "stmt s; Select s such that Follows(0, s)";
        string INVALID_PROG_LINE_QUERY = "prog_line l; Select l such that Next*(0, l)";
        string FOLLOWS_MISSING_SYNONYM_QUERY = "print pn; Select pn such that Follows(rd, pn)";
        string NEXT_MISSING_SYNONYM_QUERY = "prog_line l1; Select l1 such that Next(l1, l2)";
        string PARENT_NON_STMT_ARG_QUERY = "variable v; Select BOOLEAN such that Parent*(_, v)";
        string NEXT_NON_STMT_ARG_QUERY = "constant c; Select c such that Next(c, 42)";
        string AFFECTS_NON_ASSIGN_ARG_QUERY = "read rd; Select rd such that Affects*(rd, 69)";
        string PATTERN_MISSING_SYNONYM_QUERY = "assign a; Select a pattern a(v, \"x\")";
        string PATTERN_NON_VARIABLE_ARG_QUERY = "call cl; assign a; Select cl pattern a(cl, _)";

        // Invalid queries that fail in splitClauses
        string INCORRECT_RELATION_KEYWORD_QUERY = "stmt s; print pn; Select pn such tat Follows*(s, p)";
        string INCORRECT_PATTERN_KEYWORD_QUERY = "assign a; Select a patern a(_, \"0\")";
        string RELATION_INVALID_ARG_CHARACTER_QUERY = "while w; Select w such that Uses(w, v > 0)";
        string RELATION_INVALID_ATTRIBUTE_ARG_QUERY = "procedure p; call cl; Select cl.stmt# such that Calls(p, cl.procName)";
        string PATTERN_INVALID_SYNTAX_QUERY = "read rd; print pn; assign a; Select a pattern(_, _) and Follows(rd, pn)";
        string PATTERN_TOO_FEW_ARGS_QUERY = "if ifs; Select ifs pattern ifs(_)";
        string PATTERN_INVALID_FIRST_ARG_CHARACTER_QUERY = "while w; Select w.stmt# pattern w(\"i != 0\", _)";
        string PATTERN_INVALID_SECOND_ARG_CHARACTER_QUERY = "assign a; Select a pattern a(\"x\", \"2 ^ 32 - 1\")";
        string PATTERN_INVALID_ATTRIBUTE_ARG_QUERY = "if ifs; Select ifs(v.value, _, _)";
        string PATTERN_MALFORMED_PATTERN_STRING_QUERY = "assign a; Select a pattern a(_, \"_6_\")";
        string PATTERN_UNBALANCED_UNDERSCORES_QUERY = "assign a; Select a pattern a(_, _\"x\")";
        string INVALID_SYNONYM_PATTERN_STRING_QUERY = "assign a; variable v; Select v pattern a(\"notFound\", v)";

        // Invalid queries that fail in parseDeclarations
        string UNRECOGNISED_DESIGN_ENTITY_QUERY = "program pr; Select pr such that Follows(1, 2)";
        string CONFLICTING_SYNONYM_QUERY = "constant c; procedure p; call c; Select p";

        // Invalid queries that fail in OR due to parseQueryTarget
        string INVALID_RETURN_TYPE_QUERY = "Select _";
        string INVALID_LITERAL_RETURN_TYPE_QUERY = "Select \"v\"";
        string INVALID_RETURN_SYNONYM_QUERY = "Select 0xF";
        string MISSING_RETURN_TYPE_QUERY = "print pn; stmt s1; Select such that Follows(pn, s1)";
        string MULTIPLE_RETURN_TYPE_QUERY = "if ifs; assign a; Select ifs, a such that Parent*(ifs, a)";
        string MISSING_ATTRIBUTE_KEYWORD_QUERY = "constant c; Select c.";
        string INVALID_ATTRIBUTE_SPECIFIER_QUERY = "variable v; Select v->varName";
        string INVALID_ATTRIBUTE_CHARACTER_QUERY = "while w; Select w.stmt1";
        string INVALID_ATTRIBUTE_SYNTAX_QUERY = "read rd; Select rd.varName.value";
        string EMPTY_TUPLE_QUERY = "Select <>";
        string INVALID_TUPLE_SPECIFIER_QUERY = "procedure p, q, r; Select [p, q, r]";
        string INVALID_TUPLE_DELIMITER_QUERY = "stmt s1, s2, s3; Select <s1 & s2 & s3>";
        string INVALID_SYNONYM_IN_TUPLE_QUERY = "assign a; Select <a, 0xFFFFF>";
        string MISSING_ATTRIBUTE_KEYWORD_IN_TUPLE_QUERY = "while w; variable v; Select <w, v.>";
        string INVALID_SINGLE_WITH_TUPLE_QUERY = "procedure p; call cl; Select <p>, cl";
        string INVALID_ATTRIBUTE_KEYWORD_QUERY = "if ifs; Select ifs.stmtNo";
        string INVALID_ATTRIBUTE_KEYWORD_IN_TUPLE_QUERY = "read rd; Select <rd, rd.var>";

        // Invalid queries that fail in parseRelationClauses
        string INVALID_RELATION_KEYWORD_QUERY = "read rd; print pn; Select s1 such that Folows(rd, pn)";
        string INVALID_RELATION_TYPE_QUERY = "procedure p, q; Select p such that Recurses(p, q)";
        string INCORRECT_RELATION_NUM_ARGS_QUERY = "stmt s1, s2, s3; Select s1 such that Parent(s1, s2, s3)";
        string FOLLOWS_NON_STMT_REF_ARG_QUERY = "Select BOOLEAN such that Follows*(\"32\", 30)";
        string PARENT_NON_STMT_REF_ARG_QUERY = "Select BOOLEAN such that Parent*(3203, __private)";
        string USES_INVALID_SECOND_ARG_QUERY = "assign a; Select a such that Uses(a, 420)";
        string MODIFIES_INVALID_FIRST_ARG_QUERY = "read rd; variable v; Select rd such that Modifies(\"2\", v)";
        string CALLS_NON_ENTITY_REF_ARG_QUERY = "procedure p; Select p such that Calls*(p, __)";
        string NEXT_NON_LINE_REF_ARG_QUERY = "Select BOOLEAN such that Next(1231, \"tough\")";
        string AFFECTS_NON_STMT_REF_QUERY = "assign a; Select a such that Affects*(a, 1_024)";

        // Invalid queries that fail in parsePatternClauses
        string PATTERN_UNDECLARED_SYNONYM_QUERY = "variable v; Select v pattern a(v, _)";
        string PATTERN_NON_ENTITY_REF_ARG_QUERY = "assign a; Select a pattern a(1, _)";
        string PATTERN_ASSIGN_TOO_MANY_ARGS_QUERY = "assign a; Select a pattern a(\"x\", _, _)";
        string PATTERN_STRING_INVALID_NAME_QUERY = "assign a; Select a pattern a(_, _\"(x) + 0xF\"_)";
        string PATTERN_STRING_INVALID_STRUCTURE_QUERY = "assign a; Select a pattern a(_, \"send - (SOS) *\")";
        string PATTERN_STRING_INVALID_PARENTHESES_QUERY = "assign a; Select a pattern a(_, _\"3 + (4 / (5 - 6)\"_)";
        string PATTERN_WHILE_TOO_MANY_ARGS_QUERY = "while w; Select w pattern w(_, _, _)";
        string PATTERN_WHILE_INVALID_SECOND_ARG_QUERY = "while w; Select w pattern w(_, _\"v\"_)";
        string PATTERN_IF_TOO_FEW_ARGS_QUERY = "if ifs; Select ifs pattern ifs(_, _)";
        string PATTERN_IF_INVALID_ARGS_QUERY = "if ifs; Select ifs pattern ifs(_, _, _\"v\"_)";
        string INVALID_PATTERN_TYPE_QUERY = "call c; Select c pattern c(\"main\", _)";

        vector<string> VALID_QUERIES = {
            TRIVIAL_SYNONYM_QUERY, TRIVIAL_BOOLEAN_QUERY, TRIVIAL_ATTRIBUTE_QUERY,
            TRIVIAL_REDUNDANT_ATTRIBUTE_QUERY, TRIVIAL_MANY_DECLARATIONS_QUERY,
            TRIVIAL_ONE_TUPLE_QUERY, TRIVIAL_TUPLE_QUERY, TRIVIAL_TUPLE_ATTRIBUTES_QUERY,
            TRIVIAL_TUPLE_MIXED_QUERY, SIMPLE_CONSTANTS_QUERY, SIMPLE_VALID_QUERY,
            SIMPLE_VALID_BOOLEAN_QUERY, SIMPLE_VALID_ATTRIBUTES_QUERY, SIMPLE_PATTERN_STRING_QUERY, 
            COMPLEX_VALID_QUERY, COMPLEX_VALID_ATTRIBUTES_QUERY, COMPLEX_PATTERN_STRING_QUERY,
            CHAINED_BOOLEAN_QUERY, CHAINED_BASIC_RELATIONS_QUERY, CHAINED_MIXED_CLAUSES_QUERY,
            CHAINED_CALLS_QUERY, CHAINED_NEXT_QUERY, CHAINED_AFFECTS_QUERY,
            CHAINED_STMT_PROG_LINE_QUERY, CHAINED_MIXED_RETURN_QUERY, CHAINED_PROCEDURE_RELATIONS_QUERY,
            CHAINED_ADV_RELATIONS_QUERY, CHAINED_ALL_RELATIONS_QUERY, CHAINED_ALL_PATTERNS_QUERY,
            CHAINED_VARYING_WHITESPACE_QUERY
        };

        // Array containing all invalid queries with a single deviation from the syntax grammar or
        // proper query semantics
        vector<string> INVALID_QUERIES = {
            // validateQuerySyntax
            EMPTY_QUERY, INCORRECT_DECLARATION_QUERY, DUPLICATE_BODY_QUERY,
            MISSING_BODY_SUFFIX_QUERY, MISSING_SELECT_QUERY, ENDING_SEMICOLON_QUERY, FOREIGN_CHARACTER_QUERY,
            INVALID_AND_RELATION_QUERY, INVALID_AND_PATTERN_QUERY,
            // validateQuerySemantics
            AMBIGUOUS_BOOLEAN_QUERY, INCORRECT_BOOLEAN_KEYWORD_QUERY,
            MISSING_RETURN_TYPE_SYNONYM_QUERY, MISSING_BOOLEAN_IN_TUPLE_QUERY,
            MISSING_SYNONYM_IN_TUPLE_QUERY, INVALID_PROG_LINE_ATTRIBUTE_QUERY,
            INVALID_SYNONYM_ATTRIBUTE_QUERY, INVALID_SYNONYM_ATTRIBUTE_IN_TUPLE_QUERY,            
            AMBIGUOUS_WILDCARD_QUERY, USES_MISSING_FIRST_SYNONYM_QUERY,
            USES_UNDEFINED_RELATION_QUERY, MODIFIES_UNDEFINED_RELATION_QUERY,
            MODIFIES_MISSING_SECOND_SYNONYM_QUERY, USES_NON_VARIABLE_ARG_QUERY,
            CALLS_MISSING_SYNONYM_QUERY, CALLS_NON_PROCEDURE_ARG_QUERY,
            INVALID_STMT_NUMBER_QUERY, INVALID_PROG_LINE_QUERY,
            FOLLOWS_MISSING_SYNONYM_QUERY, NEXT_MISSING_SYNONYM_QUERY,
            PARENT_NON_STMT_ARG_QUERY, NEXT_NON_STMT_ARG_QUERY, AFFECTS_NON_ASSIGN_ARG_QUERY,
            PATTERN_MISSING_SYNONYM_QUERY, PATTERN_NON_VARIABLE_ARG_QUERY,
            // splitConstraints
            INCORRECT_RELATION_KEYWORD_QUERY, INCORRECT_PATTERN_KEYWORD_QUERY,
            RELATION_INVALID_ARG_CHARACTER_QUERY, RELATION_INVALID_ATTRIBUTE_ARG_QUERY,
            PATTERN_INVALID_SYNTAX_QUERY, PATTERN_TOO_FEW_ARGS_QUERY,
            PATTERN_INVALID_FIRST_ARG_CHARACTER_QUERY, PATTERN_INVALID_SECOND_ARG_CHARACTER_QUERY,
            PATTERN_INVALID_ATTRIBUTE_ARG_QUERY, PATTERN_MALFORMED_PATTERN_STRING_QUERY,
            PATTERN_UNBALANCED_UNDERSCORES_QUERY, INVALID_SYNONYM_PATTERN_STRING_QUERY,
            // parseDeclarations
            UNRECOGNISED_DESIGN_ENTITY_QUERY, CONFLICTING_SYNONYM_QUERY,
            // parseQueryTarget
            INVALID_RETURN_TYPE_QUERY, INVALID_LITERAL_RETURN_TYPE_QUERY, INVALID_RETURN_SYNONYM_QUERY,
            MISSING_RETURN_TYPE_QUERY, MULTIPLE_RETURN_TYPE_QUERY, MISSING_ATTRIBUTE_KEYWORD_QUERY,
            INVALID_ATTRIBUTE_SPECIFIER_QUERY, INVALID_ATTRIBUTE_CHARACTER_QUERY, INVALID_ATTRIBUTE_SYNTAX_QUERY,
            EMPTY_TUPLE_QUERY, INVALID_TUPLE_SPECIFIER_QUERY, INVALID_TUPLE_DELIMITER_QUERY,
            INVALID_SYNONYM_IN_TUPLE_QUERY, MISSING_ATTRIBUTE_KEYWORD_IN_TUPLE_QUERY,
            INVALID_SINGLE_WITH_TUPLE_QUERY, INVALID_ATTRIBUTE_KEYWORD_QUERY, INVALID_ATTRIBUTE_KEYWORD_IN_TUPLE_QUERY,
            // parseRelationClauses
            INVALID_RELATION_TYPE_QUERY, INVALID_RELATION_KEYWORD_QUERY, INCORRECT_RELATION_NUM_ARGS_QUERY,
            FOLLOWS_NON_STMT_REF_ARG_QUERY, PARENT_NON_STMT_REF_ARG_QUERY,
            USES_INVALID_SECOND_ARG_QUERY, MODIFIES_INVALID_FIRST_ARG_QUERY,
            CALLS_NON_ENTITY_REF_ARG_QUERY, NEXT_NON_LINE_REF_ARG_QUERY, AFFECTS_NON_STMT_REF_QUERY,
            // parsePatternClauses
            PATTERN_UNDECLARED_SYNONYM_QUERY, PATTERN_NON_ENTITY_REF_ARG_QUERY,
            PATTERN_ASSIGN_TOO_MANY_ARGS_QUERY, PATTERN_STRING_INVALID_NAME_QUERY,
            PATTERN_STRING_INVALID_STRUCTURE_QUERY, PATTERN_STRING_INVALID_PARENTHESES_QUERY,
            PATTERN_WHILE_TOO_MANY_ARGS_QUERY, PATTERN_WHILE_INVALID_SECOND_ARG_QUERY,
            PATTERN_IF_TOO_FEW_ARGS_QUERY, PATTERN_IF_INVALID_ARGS_QUERY, INVALID_PATTERN_TYPE_QUERY
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
