#include "stdafx.h"

#include "QueryParser.h"

using std::pair;
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
        string TRIVIAL_TUPLE_QUERY = "procedure p; variable BOOLEAN; Select <p, BOOLEAN>";
        string TRIVIAL_TUPLE_ATTRIBUTES_QUERY = "constant procName; variable value; procedure stmtNum; read varName; Select <varName.stmt#, value.varName, stmtNum.procName, procName.value>";
        string TRIVIAL_REDUNDANT_ATTRIBUTE_QUERY = "print pn; Select <pn, pn.stmt#, pn.varName>";
        string TRIVIAL_TUPLE_MIXED_QUERY = "call cl; read rd; Select <cl, cl.procName, rd, rd.varName>";
        string SIMPLE_CONSTANTS_QUERY = "assign a; Select a such that Follows*(4, 9) pattern a(\"x\", _\"(0)\"_)";
        string SIMPLE_VALID_QUERY = "variable v; if ifs; assign a; Select v such that Parent(ifs, a) pattern a(v, _)";
        string SIMPLE_VALID_BOOLEAN_QUERY = "stmt progline; prog_line stmt; Select BOOLEAN such that Follows*(stmt, progline)";
        string SIMPLE_VALID_TUPLE_QUERY = "procedure p, q; constant BOOLEAN; Select <p, q, BOOLEAN> such that Modifies(p, _)";
        string SIMPLE_VALID_ATTRIBUTES_QUERY = "while w; read rd; Select <w.stmt#, rd, rd.varName> such that Parent*(w, rd)";
        string SIMPLE_PATTERN_STRING_QUERY = "assign a; Select a pattern a(_, \"x + 1 - 2 * y / z % 3\")";
        string SIMPLE_LITERAL_INTEGER_EQUALITY_QUERY = "Select BOOLEAN with 1231 = 4231";
        string SIMPLE_LITERAL_IDENTIFIER_EQUALITY_QUERY = "constant c; variable v; Select <c, v> with \"bL\" = \"satan\"";
        string SIMPLE_MIXED_EQUALITY_QUERY = "call cl; Select cl with cl.procName = \"procedure\"";
        string SIMPLE_INTEGER_EQUALITY_QUERY = "print pn; prog_line l; Select <pn, l> with pn.stmt# = l";
        string SIMPLE_IDENTIFIER_EQUALITY_QUERY = "procedure p; variable v; Select <p, v> with p.procName = v.varName";
        string COMPLEX_VALID_BASIC_QUERY = "assign pattern; while Modifies; variable while, constant; Select pattern such that Uses(Modifies, while) pattern pattern(constant, _\"1\"_)";
        string COMPLEX_VALID_ATTRIBUTES_QUERY = "assign a; call cl; variable v; Select <a, cl, cl.procName, v.varName> pattern a(v, _) such that Follows(a, cl)";
        string COMPLEX_PATTERN_STRING_QUERY = "assign a; Select a pattern a(\"var\", _\"((p) - (q / 2) % r) * 3 - ((s + t % 5) - u) + v / 7\"_)";
        string COMPLEX_VALID_ADVANCED_QUERY = "read rd; while w; variable v; print pn; Select <w, rd.varName> with rd.varName = v.varName pattern w(v, _) such that Uses(pn, v)";
        string CHAINED_BOOLEAN_QUERY = "Select BOOLEAN such that Parent*(16, 25) and Uses(16, \"i\") and Modifies(25, \"i\")";
        string CHAINED_BASIC_RELATIONS_QUERY = "prog_line l; variable v; if ifs; read rd; Select <l, rd, v> such that Follows(l, ifs) and Parent*(ifs, rd) such that Uses(ifs, v) and Modifies(rd, v)";
        string CHAINED_PROCEDURE_RELATIONS_QUERY = "Select BOOLEAN such that Modifies(\"main\", \"argv\") and Uses(\"main\", \"argv\")";
        string CHAINED_MIXED_CLAUSES_QUERY = "assign a; procedure p; variable v1, v2; while w; if ifs; prog_line l; Select <p, w, a> pattern a(v1, \"0\") and w(v1, _) such that Follows*(a, ifs) pattern ifs(v2, _, _) such that Modifies(p, v2) and Parent(ifs, l) and Uses(l, v1)";
        string CHAINED_MIXED_RETURN_QUERY = "constant c; call cl; read rd; variable v; Select <c.value, cl.stmt#, cl.procName, rd.varName, v> such that Next*(cl, rd) and Modifies(rd, v) such that Calls(\"init\", _)";
        string CHAINED_CALLS_QUERY = "procedure p, q; Select <p, q> such that Calls(\"main\", p) and Calls*(p, q) and Calls(q, _)";
        string CHAINED_NEXT_QUERY = "prog_line l; call cl; Select <l, cl> such that Next(20, 30) and Next*(_, l) and Next(l, cl) and Next*(cl, 25)";
        string CHAINED_AFFECTS_QUERY = "stmt s; assign a; Select <s, a> such that Affects(20, 40) and Affects*(1, s) and Affects*(a, s) and Affects(s, _)";
        string CHAINED_STMT_PROG_LINE_QUERY = "stmt s; prog_line l1, l2; variable v; Select <l1, v> such that Parent*(l1, s) and Next*(s, l2) and Affects(l2, l1) and Uses(l1, v) and Modifies(l1, _)";
        string CHAINED_ADV_RELATIONS_QUERY = "prog_line l; call cl; assign a; variable v; Select <l, a, v> such that Uses(\"error\", v) and Modifies(a, v) such that Affects(l, a) and Next*(a, cl) and Calls*(_, \"error\")";
        string CHAINED_EQUALITIES_QUERY = "constant c; prog_line l; print pn; if ifs; variable v; procedure p; Select <c, v, p> with p.procName = \"lambda\" with ifs.stmt# = 8 and v.varName = pn.varName with l = c.value";
        string CHAINED_CONFUSING_AND_QUERY = "prog_line and, operand; assign a; Select BOOLEAN with 19 = and with 20 = operand such that Next(operand, and) with operand = and pattern a(_, \"and\") such that Affects*(and, and)";
        string CHAINED_ALL_RELATIONS_QUERY = "procedure p; stmt s; prog_line l; call cl; assign a; variable v; Select <p, v> such that Modifies(\"procedure\", v) such that Calls(_, p) and Uses(p, v) such that Parent(s, cl) such that Modifies(cl, v) and Follows*(cl, l) and Next(l, a) such that Affects*(a, a)";
        string CHAINED_ALL_PATTERNS_QUERY = "while w1, w2; assign a1, a2; if ifs; variable v; Select <a1, a2, v> pattern w1(v, _) pattern ifs(v, _, _) and a1(\"x\", _\"b * b - (4 * a * c) / 2\"_) and a2(v, \"x0 + y0 + x * x - y * y\") pattern w2(\"x0\", _)";
        string CHAINED_ALL_NON_LITERAL_EQUALITIES_QUERY = "constant c1, c2; call cl; read rd; print pn; prog_line l1, l2, l3; Select BOOLEAN with l3 = 420 and rd.varName = pn.varName with l1 = l2 with cl.procName = \"lambda\" and l1 = rd.stmt# and pn.stmt# = c1.value with c2.value = 666";
        string CHAINED_NO_WHITESPACE_QUERY = "assign a1,a2;while w;variable v;call cl;print pn;Select<a1,w,v,pn>pattern a1(v,_)and w(v,_)with pn.varName=cl.procName and pn.stmt#=69 pattern a2(_,_\"f*x+f*(x-dx)+dx*dy\"_)such that Next*(a1,a2)with \"xyz\"=\"xyz\" such that Uses(a1,v)and Modifies(w,v)";
        string CHAINED_EXTRA_WHITESPACE_QUERY = "\nprocedure  p\v;\rwhile\tw ;assign\va\r;  variable\fv1,  v2\n;\tcall\ncl\t;\vprog_line\rl\f;\nSelect\f<\vp. procName\r,\tw\n,  cl\f,\ncl\t.\rprocName\v,\fa >\n pattern\ta(  v1,\r_\"\t69-\v 420\"_ )\nwith\t\"i\"  =\r\"i\"\vand  17\r=w\t.  stmt#  and\fl\n=\tcl\r.\vstmt#\nsuch  that\fModifies\r(p\t,v1)\fand\rUses(\tw\v,  v1\n)  pattern\tw  (v2, _\v\f) such\t\tthat  Next*\f (\ta, w\v)\rand\tFollows(\na\r,\fcl  )\vwith p\n.\fprocName\r=\n\" function\t\"  and\rcl\v.\fprocName\n=  v2  .\tvarName\n";
        string PROJECT_REPORT_SAMPLE_QUERY = "assign a1, a2; constant c; prog_line l; while w; if ifs; read rd; print pn; procedure p; variable v1, v2; Select <a1, w.stmt#, rd.varName, pn, p> such that Modifies(a1, v1) and Uses(pn, v1) and Parent*(w, a1) with v2.varName = \"input\" pattern w(\"i\", _) and a1(_, _\"i + 1 + x / 2 + y - (3 * z)\"_) such that Calls(\"driver\", _) and Uses(p, \"j\") and Affects*(1, 64) with c.value = l and rd.varName = v2.varName and 3203 = 3230 pattern ifs(v2, _, _)";
        string EXT_SIMPLE_NOT_QUERY = "assign a; constant c; while w; variable v; Select <a, w, v> such that not Parent*(w, a) pattern a(v, _) with not a.stmt# = c.value pattern not w(v, _) with not \"taiwan\" = \"china\" such that Uses(w, v) with c.value = w.stmt#";
        string EXT_CHAINED_NOT_QUERY = "constant c; if ifs; while w; call cl; read rd; print pn; variable v1, v2; Select <v1, v2> such that Uses(pn, v1) and not Next*(rd, pn) with not cl.procName = v1.varName and v2.varName = cl.procName and not 2019 = 2020 pattern w(v1, _) and not ifs(v1, _, _) such that not Next*(pn, rd) and Modifies(rd, v2) pattern not w(v2, _) and ifs(v2, _, _) with w.stmt# = c.value and not c.value = ifs.stmt#";
        string EXT_CONFUSING_NOT_QUERY = "assign not; constant c; variable v; Select not pattern not(\"x\", _) and not not(_, \"0\") with not not.stmt# = 1 and not.stmt# = c.value such that not Affects(not, _) and Modifies(not, v) pattern not(_, _\"0\"_)";
        string EXT_CONFUSING_NOT_AND_QUERY = "assign and; procedure pattern; prog_line not; variable with; Select and with not not = 1337 and not and.stmt# = 62353535 pattern and(with, _) with not = and.stmt# and not with.varName = pattern.procName and and.stmt# = not and not \"not\" = pattern.procName";

        // Invalid queries that fail in validateQuerySyntax
        string EMPTY_QUERY = "";
        string FOREIGN_CHARACTER_IN_DECLARATION_QUERY = "assign a; read r1 & r2; Select r1";
        string INCORRECT_DECLARATION_SYNTAX_QUERY = "if ifs; while w1 w2; Select ifs";
        string MISSING_BODY_QUERY = "print pn;";
        string MISSING_BODY_SUFFIX_QUERY = "read rd; Select ";
        string MISSING_SELECT_QUERY = "stmt s; select s";
        string ENDING_SEMICOLON_QUERY = "procedure p; Select p;";
        string FOREIGN_CHARACTER_IN_BODY_QUERY = "constant c; Select | c";

        // Invalid queries that fail in validateQuerySemantics
        string AMBIGUOUS_BOOLEAN_QUERY = "stmt BOOLEAN; Select BOOLEAN such that Uses(BOOLEAN, _)";
        string INCORRECT_BOOLEAN_KEYWORD_QUERY = "Select Boolean";
        string MISSING_RETURN_TYPE_SYNONYM_QUERY = "if ifs; read rd; Select v such that Parent(ifs, rd)";
        string MISSING_BOOLEAN_IN_TUPLE_QUERY = "Select <BOOLEAN>";
        string MISSING_SYNONYM_IN_TUPLE_QUERY = "prog_line l; while BOOLEAN; Select <BOOLEAN, ifs> such that Parent(BOOLEAN, l)";
        string INVALID_PROG_LINE_ATTRIBUTE_QUERY = "prog_line l; Select l.stmt# such that Follows(l, 4231)";
        string INVALID_SYNONYM_ATTRIBUTE_QUERY = "assign a; Select a.varName pattern a(_, \"0\")";
        string INVALID_SYNONYM_ATTRIBUTE_TYPE_IN_TUPLE_QUERY = "call cl; print pn; Select <cl, cl.procName, pn.value> such that Next(cl, pn)";
        string USESS_AMBIGUOUS_WILDCARD_QUERY = "variable v; Select v such that Uses(_, v)";
        string MODIFIESS_MISSING_FIRST_SYNONYM_QUERY = "variable v; Select v such that Modifies(w, v)";
        string USESS_UNDEFINED_RELATION_QUERY = "read rd; Select rd such that Uses(rd, _)";
        string MODIFIESS_UNDEFINED_RELATION_QUERY = "print pn; Select pn such that Modifies(pn, \"n\")";
        string USESS_MISSING_SECOND_SYNONYM_QUERY = "assign a; Select a such that Uses(a, v)";
        string MODIFIESP_MISSING_SECOND_SYNONYM_QUERY = "procedure p; Select p such that Modifies(p, v)";
        string USESP_LITERAL_MISSING_SECOND_SYNONYM_QUERY = "Select BOOLEAN such that Uses(\"f\", v)";
        string USESP_NON_VARIABLE_ARG_QUERY = "procedure p; while w; Select p such that Uses(p, w)";
        string MODIFIESS_NON_VARIABLE_ARG_QUERY = "constant c; prog_line l; Select l such that Modifies(l, c)";
        string MODIFIESP_LITERAL_NON_VARIABLE_ARG_QUERY = "if ifs; Select BOOLEAN such that Modifies(\"func\", ifs)";
        string CALLS_MISSING_SYNONYM_QUERY = "Select BOOLEAN such that Calls(p, _)";
        string CALLS_NON_PROCEDURE_ARG_QUERY = "call cl; Select cl such that Calls*(\"main\", cl)";
        string INVALID_STMT_NUMBER_OVERFLOW_QUERY = "Select BOOLEAN such that Next*(2147483648, _)";
        string INVALID_STMT_NUMBER_QUERY = "stmt s; Select s such that Follows(0, s)";
        string INVALID_PROG_LINE_QUERY = "prog_line l; Select l such that Next*(0, l)";
        string FOLLOWS_MISSING_SYNONYM_QUERY = "print pn; Select pn such that Follows(rd, pn)";
        string NEXT_MISSING_SYNONYM_QUERY = "prog_line l1; Select l1 such that Next(l1, l2)";
        string AFFECTS_NON_ASSIGN_ARG_QUERY = "read rd; Select rd such that Affects*(rd, 69)";
        string PARENT_NON_STMT_ARG_QUERY = "variable v; Select BOOLEAN such that Parent*(_, v)";
        string NEXT_NON_STMT_ARG_QUERY = "constant c; Select c such that Next(c, 42)";
        string PATTERN_MISSING_SYNONYM_QUERY = "assign a; Select a pattern a(v, \"x\")";
        string PATTERN_NON_VARIABLE_ARG_QUERY = "call cl; assign a; Select cl pattern a(cl, _)";
        string WITH_DIFFERING_LITERAL_ARG_TYPE_QUERY = "Select BOOLEAN with 1 = \"one\"";
        string WITH_MISSING_SYNONYM_ARG_QUERY = "stmt s; Select s with s.stmt# = l";
        string WITH_NON_PROG_LINE_SYNONYM_ARG_QUERY = "constant c; prog_line l; Select c with c = l";
        string WITH_MISSING_SYNONYM_IN_ATTRIBUTE_ARG_QUERY = "read rd; Select rd.varName with rd.varName = pn.varName";
        string WITH_INVALID_SYNONYM_ATTRIBUTE_TYPE_QUERY = "variable v; Select v with v.value = 0";
        string WITH_INVALID_LITERAL_SYNONYM_ARGS_QUERY = "prog_line l; Select l with l = \"ll\"";
        string WITH_INVALID_LITERAL_INTEGER_ATTRIBUTE_ARGS_QUERY = "assign a; Select a with \"procedure\" = a.stmt#";
        string WITH_INVALID_LITERAL_IDENTIFIER_ATTRIBUTE_ARGS_QUERY = "call cl; Select BOOLEAN with cl.procName = 737";
        string WITH_INVALID_SYNONYM_ATTRIBUTE_ARGS_QUERY = "prog_line l; print pn; Select pn with l = pn.varName";
        string WITH_INVALID_ATTRIBUTE_ATTRIBUTE_ARGS_QUERY = "while w; call cl; Select BOOLEAN with cl.procName = w.stmt#";

        // Invalid queries that fail in splitClauses
        string INVALID_AND_RELATION_QUERY = "while w1, w2; Select w1 such that Parent(w1, w2) and such that Follows(w2, _)";
        string INVALID_AND_PATTERN_QUERY = "assign a1, a2; variable v; Select v pattern a1(v, _) and pattern a2(v, _)";
        string INVALID_AND_WITH_QUERY = "prog_line and; Select BOOLEAN with 1 = and and with \"bob\" = \"theBuilder\"";
        string MISSING_RETURN_TYPE_QUERY = "print pn; stmt s1; Select such that Follows(pn, s1)";
        string INCORRECT_RELATION_KEYWORD_QUERY = "stmt s; print pn; Select pn such tat Follows*(s, p)";
        string INCORRECT_PATTERN_KEYWORD_QUERY = "assign a; Select a patern a(_, \"0\")";
        string INCORRECT_EQUALITY_KEYWORD_QUERY = "Select BOOLEAN wit 1 = 1";
        string RELATION_INVALID_ARG_CHARACTER_QUERY = "while w; Select w such that Uses(w, v > 0)";
        string RELATION_INVALID_ATTRIBUTE_ARG_QUERY = "procedure p; call cl; Select cl.stmt# such that Calls(p, cl.procName)";
        string PATTERN_INVALID_SYNTAX_QUERY = "read rd; print pn; assign a; Select a pattern(_, _) and Follows(rd, pn)";
        string PATTERN_TOO_FEW_ARGS_QUERY = "if ifs; Select ifs pattern ifs(_)";
        string PATTERN_INVALID_FIRST_ARG_CHARACTER_QUERY = "while w; Select w.stmt# pattern w(\"i > 0\", _)";
        string PATTERN_INVALID_SECOND_ARG_CHARACTER_QUERY = "assign a; Select a pattern a(\"x\", \"i = 1\")";
        string PATTERN_INVALID_ATTRIBUTE_ARG_QUERY = "if ifs; Select ifs pattern ifs(v.value, _, _)";
        string PATTERN_MALFORMED_PATTERN_STRING_QUERY = "assign a; Select a pattern a(_, \"_6_\")";
        string PATTERN_UNBALANCED_UNDERSCORES_QUERY = "assign a; Select a pattern a(_, _\"x\")";
        string INVALID_SYNONYM_PATTERN_STRING_QUERY = "assign a; variable v; Select v pattern a(\"notFound\", v)";
        string WITH_INCORRECT_EQUALITY_OPERATOR_QUERY = "Select BOOLEAN with \"big\" == \"bang\"";
        string WITH_INVALID_OPERATOR_QUERY = "constant c; Select c with c > 7";
        string WITH_INVALID_WILDCARD_ARG_QUERY = "constant c; Select c with c.value = _";
        string WITH_UNQUOTED_IDENTIFIER_ARG_QUERY = "Select BOOLEAN with \"ok\" = \"missingQuote";
        string WITH_INVALID_ATTRIBUTE_SYNTAX_QUERY = "stmt s; Select s with s.stmt#.value = 1";

        // Invalid queries that fail in parseDeclarations
        string DUPLICATE_BODY_QUERY = "prog_line l1, l2; Select l1; Select l2";
        string UNRECOGNISED_DESIGN_ENTITY_QUERY = "program pr; Select pr such that Follows(1, 2)";
        string CONFLICTING_SYNONYM_QUERY = "constant c; procedure p; call c; Select p";

        // Invalid queries that fail in OR due to parseQueryTarget
        string INVALID_RETURN_TYPE_QUERY = "Select _";
        string INVALID_LITERAL_RETURN_TYPE_QUERY = "Select \"v\"";
        string INVALID_RETURN_SYNONYM_QUERY = "Select 0xF";
        string MULTIPLE_RETURN_TYPE_QUERY = "if ifs; assign a; Select ifs, a such that Parent*(ifs, a)";
        string MISSING_ATTRIBUTE_KEYWORD_QUERY = "constant c; Select c.";
        string INVALID_ATTRIBUTE_SPECIFIER_QUERY = "variable v; Select v->varName";
        string INVALID_ATTRIBUTE_CHARACTER_QUERY = "while w; Select w.stmt1";
        string INVALID_ATTRIBUTE_SYNTAX_QUERY = "read rd; Select rd.varName.value";
        string EMPTY_TUPLE_QUERY = "Select <>";
        string INVALID_TUPLE_SPECIFIER_QUERY = "procedure p, q, r; Select (p, q, r)";
        string INVALID_TUPLE_DELIMITER_QUERY = "stmt s1, s2, s3; Select <s1 + s2 + s3>";
        string INVALID_SYNONYM_IN_TUPLE_QUERY = "assign a; Select <a, 0xFFFFF>";
        string MISSING_ATTRIBUTE_KEYWORD_IN_TUPLE_QUERY = "while w; variable v; Select <w, v.>";
        string INVALID_SINGLE_WITH_TUPLE_QUERY = "procedure p; call cl; Select <p>, cl";
        string INVALID_ATTRIBUTE_KEYWORD_QUERY = "if ifs; Select ifs.stmtNo";
        string INVALID_ATTRIBUTE_KEYWORD_IN_TUPLE_QUERY = "read rd; Select <rd, rd.var>";

        // Invalid queries that fail in parseRelationClauses
        string INVALID_RELATION_TYPE_QUERY = "procedure p, q; Select p such that Recurses(p, q)";
        string INVALID_RELATION_KEYWORD_QUERY = "read rd; print pn; Select s1 such that Folows(rd, pn)";
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

        // Invalid queries that fail in parseWithClauses
        string WITH_INVALID_INTEGER_ARG_QUERY = "Select BOOLEAN with 9.11 = 2001";
        string WITH_INVALID_IDENTIFIER_ARG_QUERY = "procedure p; Select p with p.procName = \"2020\"";
        string WITH_INVALID_SYNONYM_ARG_QUERY = "print pn; Select pn with \"i\" = 1ol";
        string WITH_INVALID_ATTRIBUTE_KEYWORD_QUERY = "assign a; Select a with 666 = a.stmtNo";

        // All expected query results for valid query testcases
        Query TRIVIAL_SYNONYM_QUERY_RESULT = {
            STATUS_SUCCESS, TRIVIAL_SYNONYM_QUERY, false,
            {
                { "Select", AttrType::NONE }
            },
            {
                { "Select", DesignEntity::CONSTANT }
            },
            { }, { }, { }
        };

        Query TRIVIAL_BOOLEAN_QUERY_RESULT = {
            STATUS_SUCCESS, TRIVIAL_BOOLEAN_QUERY, true,
            { }, { },
            { }, { }, { }
        };

        Query TRIVIAL_ATTRIBUTE_QUERY_RESULT = {
            STATUS_SUCCESS, TRIVIAL_ATTRIBUTE_QUERY, false,
            {
                { "pn", AttrType::VAR_NAME }
            },
            {
                { "pn", DesignEntity::PRINT }
            },
            { }, { }, { }
        };

        Query TRIVIAL_MANY_DECLARATIONS_QUERY_RESULT = {
            STATUS_SUCCESS, TRIVIAL_MANY_DECLARATIONS_QUERY, false,
            {
                { "variable", AttrType::NONE }
            },
            {
                { "procedure", DesignEntity::WHILE },
                { "constant", DesignEntity::WHILE },
                { "call", DesignEntity::ASSIGN },
                { "print", DesignEntity::IF },
                { "variable", DesignEntity::READ },
                { "such", DesignEntity::CALL }
            },
            { }, { }, { }
        };

        Query TRIVIAL_ONE_TUPLE_QUERY_RESULT = {
            STATUS_SUCCESS, TRIVIAL_ONE_TUPLE_QUERY, false,
            {
                { "l", AttrType::NONE }
            },
            {
                { "l", DesignEntity::PROG_LINE }
            },
            { }, { }, { }
        };

        Query TRIVIAL_TUPLE_QUERY_RESULT = {
            STATUS_SUCCESS, TRIVIAL_TUPLE_QUERY, false,
            {
                { "p", AttrType::NONE },
                { "BOOLEAN", AttrType::NONE }
            },
            {
                { "p", DesignEntity::PROCEDURE },
                { "BOOLEAN", DesignEntity::VARIABLE }
            },
            { }, { }, { }
        };

        Query TRIVIAL_TUPLE_ATTRIBUTES_QUERY_RESULT = {
            STATUS_SUCCESS, TRIVIAL_TUPLE_ATTRIBUTES_QUERY, false,
            {
                { "varName", AttrType::STMT_NUM },
                { "value", AttrType::VAR_NAME },
                { "stmtNum", AttrType::PROC_NAME },
                { "procName", AttrType::VALUE }
            },
            {
                { "procName", DesignEntity::CONSTANT },
                { "value", DesignEntity::VARIABLE },
                { "stmtNum", DesignEntity::PROCEDURE },
                { "varName", DesignEntity::READ }
            },
            { }, { }, { }
        };

        Query TRIVIAL_REDUNDANT_ATTRIBUTE_QUERY_RESULT = {
            STATUS_SUCCESS, TRIVIAL_REDUNDANT_ATTRIBUTE_QUERY, false,
            {
                { "pn", AttrType::NONE },
                { "pn", AttrType::STMT_NUM },
                { "pn", AttrType::VAR_NAME }
            },
            {
                { "pn", DesignEntity::PRINT }
            },
            { }, { }, { }
        };

        Query TRIVIAL_TUPLE_MIXED_QUERY_RESULT = {
            STATUS_SUCCESS, TRIVIAL_TUPLE_MIXED_QUERY, false,
            {
                { "cl", AttrType::NONE },
                { "cl", AttrType::PROC_NAME },
                { "rd", AttrType::NONE },
                { "rd", AttrType::VAR_NAME }
            },
            {
                { "cl", DesignEntity::CALL },
                { "rd", DesignEntity::READ }
            },
            { }, { }, { }
        };

        Query SIMPLE_CONSTANTS_QUERY_RESULT = {
            STATUS_SUCCESS, SIMPLE_CONSTANTS_QUERY, false,
            {
                { "a", AttrType::NONE }
            },
            {
                { "a", DesignEntity::ASSIGN }
            },
            {
                { "Follows*(4, 9)", false, RelationType::FOLLOWST,
                    { ArgType::INTEGER, "4" }, { ArgType::INTEGER, "9" } }
            },
            {
                { "a(\"x\", _\"(0)\"_)", false, PatternType::ASSIGN_PATTERN, "a",
                    { ArgType::IDENTIFIER, "x" }, { ArgType::INCLUSIVE_PATTERN, "_0_" } }
            },
            { }
        };

        Query SIMPLE_VALID_QUERY_RESULT = {
            STATUS_SUCCESS, SIMPLE_VALID_QUERY, false,
            {
                { "v", AttrType::NONE }
            },
            {
                { "v", DesignEntity::VARIABLE },
                { "ifs", DesignEntity::IF },
                { "a", DesignEntity::ASSIGN },
            },
            {
                { "Parent(ifs, a)", false, RelationType::PARENT,
                    { ArgType::SYNONYM, "ifs" }, { ArgType::SYNONYM, "a" } }
            },
            {
                { "a(v, _)", false, PatternType::ASSIGN_PATTERN, "a",
                    { ArgType::SYNONYM, "v" }, { ArgType::WILDCARD, "_" } }
            },
            { }
        };

        Query SIMPLE_VALID_BOOLEAN_QUERY_RESULT = {
            STATUS_SUCCESS, SIMPLE_VALID_BOOLEAN_QUERY, true,
            { },
            {
                { "progline", DesignEntity::STATEMENT },
                { "stmt", DesignEntity::PROG_LINE }
            },
            {
                { "Follows*(stmt, progline)", false, RelationType::FOLLOWST,
                    { ArgType::SYNONYM, "stmt" }, { ArgType::SYNONYM, "progline" } }
            },
            { }, { }
        };

        Query SIMPLE_VALID_TUPLE_QUERY_RESULT = {
            STATUS_SUCCESS, SIMPLE_VALID_TUPLE_QUERY, false,
            {
                { "p", AttrType::NONE },
                { "q", AttrType::NONE },
                { "BOOLEAN", AttrType::NONE }
            },
            {
                { "p", DesignEntity::PROCEDURE },
                { "q", DesignEntity::PROCEDURE },
                { "BOOLEAN", DesignEntity::CONSTANT },
            },
            {
                { "Modifies(p, _)", false, RelationType::MODIFIESP,
                    { ArgType::SYNONYM, "p" }, { ArgType::WILDCARD, "_" } }
            },
            { }, { }
        };

        Query SIMPLE_VALID_ATTRIBUTES_QUERY_RESULT = {
            STATUS_SUCCESS, SIMPLE_VALID_ATTRIBUTES_QUERY, false,
            {
                { "w", AttrType::STMT_NUM },
                { "rd", AttrType::NONE },
                { "rd", AttrType::VAR_NAME }
            },
            {
                { "w", DesignEntity::WHILE },
                { "rd", DesignEntity::READ }
            },
            {
                { "Parent*(w, rd)", false, RelationType::PARENTT,
                    { ArgType::SYNONYM, "w" }, { ArgType::SYNONYM, "rd" } }
            },
            { }, { }
        };

        Query SIMPLE_PATTERN_STRING_QUERY_RESULT = {
            STATUS_SUCCESS, SIMPLE_PATTERN_STRING_QUERY, false,
            {
                { "a", AttrType::NONE }
            },
            {
                { "a", DesignEntity::ASSIGN }
            },
            { },
            {
                { "a(_, \"x + 1 - 2 * y / z % 3\")", false, PatternType::ASSIGN_PATTERN, "a",
                    { ArgType::WILDCARD, "_" }, { ArgType::EXACT_PATTERN, "((x+1)-(((2*y)/z)%3))" } }
            }, { }
        };

        Query SIMPLE_LITERAL_INTEGER_EQUALITY_QUERY_RESULT = {
            STATUS_SUCCESS, SIMPLE_LITERAL_INTEGER_EQUALITY_QUERY, true,
            { }, { },
            { }, { },
            {
                { "1231 = 4231", false, WithType::LITERAL_EQUAL,
                    { ArgType::INTEGER, { "1231", AttrType::NONE } }, { ArgType::INTEGER, { "4231", AttrType::NONE } } }
            }
        };

        Query SIMPLE_LITERAL_IDENTIFIER_EQUALITY_QUERY_RESULT = {
            STATUS_SUCCESS, SIMPLE_LITERAL_IDENTIFIER_EQUALITY_QUERY, false,
            {
                { "c", AttrType::NONE },
                { "v", AttrType::NONE }
            },
            {
                { "c", DesignEntity::CONSTANT },
                { "v", DesignEntity::VARIABLE }
            },
            { }, { },
            {
                { "\"bL\" = \"satan\"", false, WithType::LITERAL_EQUAL,
                    { ArgType::IDENTIFIER, { "bL", AttrType::NONE } }, { ArgType::IDENTIFIER, { "satan", AttrType::NONE } } }
            }
        };

        Query SIMPLE_MIXED_EQUALITY_QUERY_RESULT = {
            STATUS_SUCCESS, SIMPLE_MIXED_EQUALITY_QUERY, false,
            {
                { "cl", AttrType::NONE }
            },
            {
                { "cl", DesignEntity::CALL }
            },
            { }, { },
            {
                { "cl.procName = \"procedure\"", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "cl", AttrType::PROC_NAME } }, { ArgType::IDENTIFIER, { "procedure", AttrType::NONE } } }
            }
        };

        Query SIMPLE_INTEGER_EQUALITY_QUERY_RESULT = {
            STATUS_SUCCESS, SIMPLE_INTEGER_EQUALITY_QUERY, false,
            {
                { "pn", AttrType::NONE },
                { "l", AttrType::NONE }
            },
            {
                { "pn", DesignEntity::PRINT },
                { "l", DesignEntity::PROG_LINE }
            },
            { }, { },
            {
                { "pn.stmt# = l", false, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "pn", AttrType::STMT_NUM } }, { ArgType::SYNONYM, { "l", AttrType::NONE } } }
            }
        };

        Query SIMPLE_IDENTIFIER_EQUALITY_QUERY_RESULT = {
            STATUS_SUCCESS, SIMPLE_IDENTIFIER_EQUALITY_QUERY, false,
            {
                { "p", AttrType::NONE },
                { "v", AttrType::NONE }
            },
            {
                { "p", DesignEntity::PROCEDURE },
                { "v", DesignEntity::VARIABLE }
            },
            { }, { },
            {
                { "p.procName = v.varName", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "p", AttrType::PROC_NAME } }, { ArgType::ATTRIBUTE, { "v", AttrType::VAR_NAME } } }
            }
        };

        Query COMPLEX_VALID_BASIC_QUERY_RESULT = {
            STATUS_SUCCESS, COMPLEX_VALID_BASIC_QUERY, false,
            {
                { "pattern", AttrType::NONE }
            },
            {
                { "pattern", DesignEntity::ASSIGN },
                { "Modifies", DesignEntity::WHILE },
                { "while", DesignEntity::VARIABLE },
                { "constant", DesignEntity::VARIABLE }
            },
            {
                { "Uses(Modifies, while)", false, RelationType::USESS,
                    { ArgType::SYNONYM, "Modifies" }, { ArgType::SYNONYM, "while" } }
            },
            {
                { "pattern(constant, _\"1\"_)", false, PatternType::ASSIGN_PATTERN, "pattern",
                    { ArgType::SYNONYM, "constant" }, { ArgType::INCLUSIVE_PATTERN, "_1_" } }
            },
            { }
        };

        Query COMPLEX_VALID_ATTRIBUTES_QUERY_RESULT = {
            STATUS_SUCCESS, COMPLEX_VALID_ATTRIBUTES_QUERY, false,
            {
                { "a", AttrType::NONE },
                { "cl", AttrType::NONE },
                { "cl", AttrType::PROC_NAME },
                { "v", AttrType::VAR_NAME }
            },
            {
                { "a", DesignEntity::ASSIGN },
                { "cl", DesignEntity::CALL },
                { "v", DesignEntity::VARIABLE }
            },
            {
                { "Follows(a, cl)", false, RelationType::FOLLOWS,
                    { ArgType::SYNONYM, "a" }, { ArgType::SYNONYM, "cl" } }
            },
            {
                { "a(v, _)", false, PatternType::ASSIGN_PATTERN, "a",
                    { ArgType::SYNONYM, "v" }, { ArgType::WILDCARD, "_" } }
            }, { }
        };

        Query COMPLEX_PATTERN_STRING_QUERY_RESULT = {
            STATUS_SUCCESS, COMPLEX_PATTERN_STRING_QUERY, false,
            {
                { "a", AttrType::NONE }
            },
            {
                { "a", DesignEntity::ASSIGN }
            },
            { },
            {
                { "a(\"var\", _\"((p) - (q / 2) % r) * 3 - ((s + t % 5) - u) + v / 7\"_)", false, PatternType::ASSIGN_PATTERN, "a",
                    { ArgType::IDENTIFIER, "var" },
                    { ArgType::INCLUSIVE_PATTERN, "_((((p-((q/2)%r))*3)-((s+(t%5))-u))+(v/7))_" } }
            }, { }
        };

        Query COMPLEX_VALID_ADVANCED_QUERY_RESULT = {
            STATUS_SUCCESS, COMPLEX_VALID_ADVANCED_QUERY, false,
            {
                { "w", AttrType::NONE },
                { "rd", AttrType::VAR_NAME }
            },
            {
                { "rd", DesignEntity::READ },
                { "w", DesignEntity::WHILE },
                { "v", DesignEntity::VARIABLE },
                { "pn", DesignEntity::PRINT }
            },
            {
                { "Uses(pn, v)", false, RelationType::USESS,
                    { ArgType::SYNONYM, "pn" }, { ArgType::SYNONYM, "v" } }
            },
            {
                { "w(v, _)", false, PatternType::WHILE_PATTERN, "w",
                    { ArgType::SYNONYM, "v" }, { ArgType::WILDCARD, "_" } }
            },
            {
                { "rd.varName = v.varName", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "rd", AttrType::VAR_NAME } }, { ArgType::ATTRIBUTE, { "v", AttrType::VAR_NAME } } }
            }
        };

        Query CHAINED_BOOLEAN_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_BOOLEAN_QUERY, true,
            { },
            { },
            {
                { "Parent*(16, 25)", false, RelationType::PARENTT,
                    { ArgType::INTEGER, "16" }, { ArgType::INTEGER, "25" } },
                { "Uses(16, \"i\")", false, RelationType::USESS,
                    { ArgType::INTEGER, "16" }, { ArgType::IDENTIFIER, "i" } },
                { "Modifies(25, \"i\")", false, RelationType::MODIFIESS,
                    { ArgType::INTEGER, "25" }, { ArgType::IDENTIFIER, "i" } }
            },
            { }, { }
        };

        Query CHAINED_BASIC_RELATIONS_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_BASIC_RELATIONS_QUERY, false,
            {
                { "l", AttrType::NONE },
                { "rd", AttrType::NONE },
                { "v", AttrType::NONE }
            },
            {
                { "l", DesignEntity::PROG_LINE },
                { "v", DesignEntity::VARIABLE },
                { "ifs", DesignEntity::IF },
                { "rd", DesignEntity::READ }
            },
            {
                { "Follows(l, ifs)", false, RelationType::FOLLOWS,
                    { ArgType::SYNONYM, "l" }, { ArgType::SYNONYM, "ifs" } },
                { "Parent*(ifs, rd)", false, RelationType::PARENTT,
                    { ArgType::SYNONYM, "ifs" }, { ArgType::SYNONYM, "rd" } },
                { "Uses(ifs, v)", false, RelationType::USESS,
                    { ArgType::SYNONYM, "ifs" }, { ArgType::SYNONYM, "v" } },
                { "Modifies(rd, v)", false, RelationType::MODIFIESS,
                    { ArgType::SYNONYM, "rd" }, { ArgType::SYNONYM, "v" } }
            },
            { }, { }
        };

        Query CHAINED_PROCEDURE_RELATIONS_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_PROCEDURE_RELATIONS_QUERY, true,
            { }, { },
            {
                { "Modifies(\"main\", \"argv\")", false, RelationType::MODIFIESP,
                    { ArgType::IDENTIFIER, "main" }, { ArgType::IDENTIFIER, "argv" } },
                { "Uses(\"main\", \"argv\")", false, RelationType::USESP,
                    { ArgType::IDENTIFIER, "main" }, { ArgType::IDENTIFIER, "argv" } }
            },
            { }, { }
        };

        Query CHAINED_MIXED_CLAUSES_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_MIXED_CLAUSES_QUERY, false,
            {
                { "p", AttrType::NONE },
                { "w", AttrType::NONE },
                { "a", AttrType::NONE }
            },
            {
                { "a", DesignEntity::ASSIGN },
                { "p", DesignEntity::PROCEDURE },
                { "v1", DesignEntity::VARIABLE },
                { "v2", DesignEntity::VARIABLE },
                { "w", DesignEntity::WHILE },
                { "ifs", DesignEntity::IF },
                { "l", DesignEntity::PROG_LINE }
            },
            {
                { "Follows*(a, ifs)", false, RelationType::FOLLOWST,
                    { ArgType::SYNONYM, "a" }, { ArgType::SYNONYM, "ifs" } },
                { "Modifies(p, v2)", false, RelationType::MODIFIESP,
                    { ArgType::SYNONYM, "p" }, { ArgType::SYNONYM, "v2" } },
                { "Parent(ifs, l)", false, RelationType::PARENT,
                    { ArgType::SYNONYM, "ifs" }, { ArgType::SYNONYM, "l" } },
                { "Uses(l, v1)", false, RelationType::USESS,
                    { ArgType::SYNONYM, "l" }, { ArgType::SYNONYM, "v1" } }
            },
            {
                { "a(v1, \"0\")", false, PatternType::ASSIGN_PATTERN, "a",
                    { ArgType::SYNONYM, "v1" }, { ArgType::EXACT_PATTERN, "0" } },
                { "w(v1, _)", false, PatternType::WHILE_PATTERN, "w",
                    { ArgType::SYNONYM, "v1" }, { ArgType::WILDCARD, "_" } },
                { "ifs(v2, _, _)", false, PatternType::IF_PATTERN, "ifs",
                    { ArgType::SYNONYM, "v2" }, { ArgType::WILDCARD, "_" } }
            },
            { }
        };

        Query CHAINED_MIXED_RETURN_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_MIXED_RETURN_QUERY, false,
            {
                { "c", AttrType::VALUE },
                { "cl", AttrType::STMT_NUM },
                { "cl", AttrType::PROC_NAME },
                { "rd", AttrType::VAR_NAME },
                { "v", AttrType::NONE }
            },
            {
                { "c", DesignEntity::CONSTANT },
                { "cl", DesignEntity::CALL },
                { "rd", DesignEntity::READ },
                { "v", DesignEntity::VARIABLE }
            },
            {
                { "Next*(cl, rd)", false, RelationType::NEXTT,
                    { ArgType::SYNONYM, "cl" }, { ArgType::SYNONYM, "rd" } },
                { "Modifies(rd, v)", false, RelationType::MODIFIESS,
                    { ArgType::SYNONYM, "rd" }, { ArgType::SYNONYM, "v" } },
                { "Calls(\"init\", _)", false, RelationType::CALLS,
                    { ArgType::IDENTIFIER, "init" }, { ArgType::WILDCARD, "_" } }
            },
            { }, { }
        };

        Query CHAINED_CALLS_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_CALLS_QUERY, false,
            {
                { "p", AttrType::NONE },
                { "q", AttrType::NONE }
            },
            {
                { "p", DesignEntity::PROCEDURE },
                { "q", DesignEntity::PROCEDURE }
            },
            {
                { "Calls(\"main\", p)", false, RelationType::CALLS,
                    { ArgType::IDENTIFIER, "main" }, { ArgType::SYNONYM, "p" } },
                { "Calls*(p, q)", false, RelationType::CALLST,
                    { ArgType::SYNONYM, "p" }, { ArgType::SYNONYM, "q" } },
                { "Calls(q, _)", false, RelationType::CALLS,
                    { ArgType::SYNONYM, "q" }, { ArgType::WILDCARD, "_" } }
            },
            { }, { }
        };

        Query CHAINED_NEXT_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_NEXT_QUERY, false,
            {
                { "l", AttrType::NONE },
                { "cl", AttrType::NONE }
            },
            {
                { "l", DesignEntity::PROG_LINE },
                { "cl", DesignEntity::CALL }
            },
            {
                { "Next(20, 30)", false, RelationType::NEXT,
                    { ArgType::INTEGER, "20" }, { ArgType::INTEGER, "30" } },
                { "Next*(_, l)", false, RelationType::NEXTT,
                    { ArgType::WILDCARD, "_" }, { ArgType::SYNONYM, "l" } },
                { "Next(l, cl)", false, RelationType::NEXT,
                    { ArgType::SYNONYM, "l" }, { ArgType::SYNONYM, "cl" } },
                { "Next*(cl, 25)", false, RelationType::NEXTT,
                    { ArgType::SYNONYM, "cl" }, { ArgType::INTEGER, "25" } }
            },
            { }, { }
        };

        Query CHAINED_AFFECTS_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_AFFECTS_QUERY, false,
            {
                { "s", AttrType::NONE },
                { "a", AttrType::NONE }
            },
            {
                { "s", DesignEntity::STATEMENT },
                { "a", DesignEntity::ASSIGN }
            },
            {
                { "Affects(20, 40)", false, RelationType::AFFECTS,
                    { ArgType::INTEGER, "20" }, { ArgType::INTEGER, "40" } },
                { "Affects*(1, s)", false, RelationType::AFFECTST,
                    { ArgType::INTEGER, "1" }, { ArgType::SYNONYM, "s" } },
                { "Affects*(a, s)", false, RelationType::AFFECTST,
                    { ArgType::SYNONYM, "a" }, { ArgType::SYNONYM, "s" } },
                { "Affects(s, _)", false, RelationType::AFFECTS,
                    { ArgType::SYNONYM, "s" }, { ArgType::WILDCARD, "_" } }
            },
            { }, { }
        };

        Query CHAINED_STMT_PROG_LINE_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_STMT_PROG_LINE_QUERY, false,
            {
                { "l1", AttrType::NONE },
                { "v", AttrType::NONE }
            },
            {
                { "s", DesignEntity::STATEMENT },
                { "l1", DesignEntity::PROG_LINE },
                { "l2", DesignEntity::PROG_LINE },
                { "v", DesignEntity::VARIABLE }
            },
            {
                { "Parent*(l1, s)", false, RelationType::PARENTT,
                    { ArgType::SYNONYM, "l1" }, { ArgType::SYNONYM, "s" } },
                { "Next*(s, l2)", false, RelationType::NEXTT,
                    { ArgType::SYNONYM, "s" }, { ArgType::SYNONYM, "l2" } },
                { "Affects(l2, l1)", false, RelationType::AFFECTS,
                    { ArgType::SYNONYM, "l2" }, { ArgType::SYNONYM, "l1" } },
                { "Uses(l1, v)", false, RelationType::USESS,
                    { ArgType::SYNONYM, "l1" }, { ArgType::SYNONYM, "v" } },
                { "Modifies(l1, _)", false, RelationType::MODIFIESS,
                    { ArgType::SYNONYM, "l1" }, { ArgType::WILDCARD, "_" } }
            },
            { }, { }
        };

        Query CHAINED_ADV_RELATIONS_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_ADV_RELATIONS_QUERY, false,
            {
                { "l", AttrType::NONE },
                { "a", AttrType::NONE },
                { "v", AttrType::NONE }
            },
            {
                { "l", DesignEntity::PROG_LINE },
                { "cl", DesignEntity::CALL },
                { "a", DesignEntity::ASSIGN },
                { "v", DesignEntity::VARIABLE }
            },
            {
                { "Uses(\"error\", v)", false, RelationType::USESP,
                    { ArgType::IDENTIFIER, "error" }, { ArgType::SYNONYM, "v" } },
                { "Modifies(a, v)", false, RelationType::MODIFIESS,
                    { ArgType::SYNONYM, "a" }, { ArgType::SYNONYM, "v" } },
                { "Affects(l, a)", false, RelationType::AFFECTS,
                    { ArgType::SYNONYM, "l" }, { ArgType::SYNONYM, "a" } },
                { "Next*(a, cl)", false, RelationType::NEXTT,
                    { ArgType::SYNONYM, "a" }, { ArgType::SYNONYM, "cl" } },
                { "Calls*(_, \"error\")", false, RelationType::CALLST,
                    { ArgType::WILDCARD, "_" }, { ArgType::IDENTIFIER, "error"} }
            },
            { }, { }
        };

        Query CHAINED_EQUALITIES_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_EQUALITIES_QUERY, false,
            {
                { "c", AttrType::NONE },
                { "v", AttrType::NONE },
                { "p", AttrType::NONE }
            },
            {
                { "c", DesignEntity::CONSTANT },
                { "l", DesignEntity::PROG_LINE },
                { "pn", DesignEntity::PRINT },
                { "ifs", DesignEntity::IF },
                { "v", DesignEntity::VARIABLE },
                { "p", DesignEntity::PROCEDURE }
            },
            { }, { },
            {
                { "p.procName = \"lambda\"", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "p", AttrType::PROC_NAME } }, { ArgType::IDENTIFIER, { "lambda", AttrType::NONE } } },
                { "ifs.stmt# = 8", false, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "ifs", AttrType::STMT_NUM } }, { ArgType::INTEGER, { "8", AttrType::NONE } } },
                { "v.varName = pn.varName", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "v", AttrType::VAR_NAME } }, { ArgType::ATTRIBUTE, { "pn", AttrType::VAR_NAME } } },
                { "l = c.value", false, WithType::INTEGER_EQUAL,
                    { ArgType::SYNONYM, { "l", AttrType::NONE } }, { ArgType::ATTRIBUTE, { "c", AttrType::VALUE } } }
            }
        };

        Query CHAINED_CONFUSING_AND_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_CONFUSING_AND_QUERY, true,
            { },
            {
                { "and", DesignEntity::PROG_LINE },
                { "operand", DesignEntity::PROG_LINE },
                { "a", DesignEntity::ASSIGN }
            },
            {
                { "Next(operand, and)", false, RelationType::NEXT,
                    { ArgType::SYNONYM, "operand" }, { ArgType::SYNONYM, "and" } },
                { "Affects*(and, and)", false, RelationType::AFFECTST,
                    { ArgType::SYNONYM, "and" }, { ArgType::SYNONYM, "and" } }
            },
            {
                { "a(_, \"and\")", false, PatternType::ASSIGN_PATTERN, "a",
                    { ArgType::WILDCARD, "_" }, { ArgType::EXACT_PATTERN, "and" } }
            },
            {
                { "19 = and", false, WithType::INTEGER_EQUAL,
                    { ArgType::INTEGER, { "19", AttrType::NONE } }, { ArgType::SYNONYM, { "and", AttrType::NONE } } },
                { "20 = operand", false, WithType::INTEGER_EQUAL,
                    { ArgType::INTEGER, { "20", AttrType::NONE } }, { ArgType::SYNONYM, { "operand", AttrType::NONE } } },
                { "operand = and", false, WithType::INTEGER_EQUAL,
                    { ArgType::SYNONYM, { "operand", AttrType::NONE } }, { ArgType::SYNONYM, { "and", AttrType::NONE } } }
            }
        };

        Query CHAINED_ALL_RELATIONS_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_ALL_RELATIONS_QUERY, false,
            {
                { "p", AttrType::NONE },
                { "v", AttrType::NONE }
            },
            {
                { "p", DesignEntity::PROCEDURE },
                { "s", DesignEntity::STATEMENT },
                { "l", DesignEntity::PROG_LINE },
                { "cl", DesignEntity::CALL },
                { "a", DesignEntity::ASSIGN },
                { "v", DesignEntity::VARIABLE }
            },
            {
                { "Modifies(\"procedure\", v)", false, RelationType::MODIFIESP,
                    { ArgType::IDENTIFIER, "procedure" }, { ArgType::SYNONYM, "v" } },
                { "Calls(_, p)", false, RelationType::CALLS,
                    { ArgType::WILDCARD, "_" }, { ArgType::SYNONYM, "p" } },
                { "Uses(p, v)", false, RelationType::USESP,
                    { ArgType::SYNONYM, "p" }, { ArgType::SYNONYM, "v" } },
                { "Parent(s, cl)", false, RelationType::PARENT,
                    { ArgType::SYNONYM, "s" }, { ArgType::SYNONYM, "cl" } },
                { "Modifies(cl, v)", false, RelationType::MODIFIESS,
                    { ArgType::SYNONYM, "cl" }, { ArgType::SYNONYM, "v" } },
                { "Follows*(cl, l)", false, RelationType::FOLLOWST,
                    { ArgType::SYNONYM, "cl" }, { ArgType::SYNONYM, "l" } },
                { "Next(l, a)", false, RelationType::NEXT,
                    { ArgType::SYNONYM, "l" }, { ArgType::SYNONYM, "a" } },
                { "Affects*(a, a)", false, RelationType::AFFECTST,
                    { ArgType::SYNONYM, "a" }, { ArgType::SYNONYM, "a" } }
            },
            { }, { }
        };

        Query CHAINED_ALL_PATTERNS_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_ALL_PATTERNS_QUERY, false,
            {
                { "a1", AttrType::NONE },
                { "a2", AttrType::NONE },
                { "v", AttrType::NONE }
            },
            {
                { "w1", DesignEntity::WHILE },
                { "w2", DesignEntity::WHILE },
                { "a1", DesignEntity::ASSIGN },
                { "a2", DesignEntity::ASSIGN },
                { "ifs", DesignEntity::IF },
                { "v", DesignEntity::VARIABLE }
            },
            { },
            {
                { "w1(v, _)", false, PatternType::WHILE_PATTERN, "w1",
                    { ArgType::SYNONYM, "v" }, { ArgType::WILDCARD, "_" } },
                { "ifs(v, _, _)", false, PatternType::IF_PATTERN, "ifs",
                    { ArgType::SYNONYM, "v"}, { ArgType::WILDCARD, "_" } },
                { "a1(\"x\", _\"b * b - (4 * a * c) / 2\"_)", false, PatternType::ASSIGN_PATTERN, "a1",
                    { ArgType::IDENTIFIER, "x" }, { ArgType::INCLUSIVE_PATTERN, "_((b*b)-(((4*a)*c)/2))_" } },
                { "a2(v, \"x0 + y0 + x * x - y * y\")", false, PatternType::ASSIGN_PATTERN, "a2",
                    { ArgType::SYNONYM, "v" }, { ArgType::EXACT_PATTERN, "(((x0+y0)+(x*x))-(y*y))" } },
                { "w2(\"x0\", _)", false, PatternType::WHILE_PATTERN, "w2",
                    { ArgType::IDENTIFIER, "x0" }, { ArgType::WILDCARD, "_" } }
            },
            { }
        };

        Query CHAINED_ALL_NON_LITERAL_EQUALITIES_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_ALL_NON_LITERAL_EQUALITIES_QUERY, true,
            { },
            {
                { "c1", DesignEntity::CONSTANT },
                { "c2", DesignEntity::CONSTANT },
                { "cl", DesignEntity::CALL },
                { "rd", DesignEntity::READ },
                { "pn", DesignEntity::PRINT },
                { "l1", DesignEntity::PROG_LINE },
                { "l2", DesignEntity::PROG_LINE },
                { "l3", DesignEntity::PROG_LINE }
            },
            { }, { },
            {
                { "l3 = 420", false, WithType::INTEGER_EQUAL,
                    { ArgType::SYNONYM, { "l3", AttrType::NONE } }, { ArgType::INTEGER, { "420", AttrType::NONE } } },
                { "rd.varName = pn.varName", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "rd", AttrType::VAR_NAME } }, { ArgType::ATTRIBUTE, { "pn", AttrType::VAR_NAME } } },
                { "l1 = l2", false, WithType::INTEGER_EQUAL,
                    { ArgType::SYNONYM, { "l1", AttrType::NONE } }, { ArgType::SYNONYM, { "l2", AttrType::NONE } } },
                { "cl.procName = \"lambda\"", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "cl", AttrType::PROC_NAME } }, { ArgType::IDENTIFIER, { "lambda", AttrType::NONE } } },
                { "l1 = rd.stmt#", false, WithType::INTEGER_EQUAL,
                    { ArgType::SYNONYM, { "l1", AttrType::NONE } }, { ArgType::ATTRIBUTE, { "rd", AttrType::STMT_NUM } } },
                { "pn.stmt# = c1.value", false, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "pn", AttrType::STMT_NUM } }, { ArgType::ATTRIBUTE, { "c1", AttrType::VALUE } } },
                { "c2.value = 666", false, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "c2", AttrType::VALUE } }, { ArgType::INTEGER, { "666", AttrType::NONE } } }
            }
        };

        Query CHAINED_NO_WHITESPACE_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_NO_WHITESPACE_QUERY, false,
            {
                { "a1", AttrType::NONE },
                { "w", AttrType::NONE },
                { "v", AttrType::NONE },
                { "pn", AttrType::NONE }
            },
            {
                { "a1", DesignEntity::ASSIGN },
                { "a2", DesignEntity::ASSIGN },
                { "w", DesignEntity::WHILE },
                { "v", DesignEntity::VARIABLE },
                { "cl", DesignEntity::CALL },
                { "pn", DesignEntity::PRINT }
            },
            {
                { "Next*(a1,a2)", false, RelationType::NEXTT,
                    { ArgType::SYNONYM, "a1" }, { ArgType::SYNONYM, "a2" } },
                { "Uses(a1,v)", false, RelationType::USESS,
                    { ArgType::SYNONYM, "a1" }, { ArgType::SYNONYM, "v" } },
                { "Modifies(w,v)", false, RelationType::MODIFIESS,
                    { ArgType::SYNONYM, "w" }, { ArgType::SYNONYM, "v" } }
            },
            {
                { "a1(v,_)", false, PatternType::ASSIGN_PATTERN, "a1",
                    { ArgType::SYNONYM, "v" }, { ArgType::WILDCARD, "_" } },
                { "w(v,_)", false, PatternType::WHILE_PATTERN, "w",
                    { ArgType::SYNONYM, "v" }, { ArgType::WILDCARD, "_" } },
                { "a2(_,_\"f*x+f*(x-dx)+dx*dy\"_)", false, PatternType::ASSIGN_PATTERN, "a2",
                    { ArgType::WILDCARD, "_" }, { ArgType::INCLUSIVE_PATTERN, "_(((f*x)+(f*(x-dx)))+(dx*dy))_" } }
            },
            {
                { "pn.varName=cl.procName", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "pn", AttrType::VAR_NAME } }, { ArgType::ATTRIBUTE, { "cl", AttrType::PROC_NAME } } },
                { "pn.stmt#=69", false, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "pn", AttrType::STMT_NUM } }, { ArgType::INTEGER, { "69", AttrType::NONE } } },
                { "\"xyz\"=\"xyz\"", false, WithType::LITERAL_EQUAL,
                    { ArgType::IDENTIFIER, { "xyz", AttrType::NONE } }, { ArgType::IDENTIFIER, { "xyz", AttrType::NONE } } }
            }
        };

        Query CHAINED_EXTRA_WHITESPACE_QUERY_RESULT = {
            STATUS_SUCCESS, CHAINED_EXTRA_WHITESPACE_QUERY, false,
            {
                { "p", AttrType::PROC_NAME },
                { "w", AttrType::NONE },
                { "cl", AttrType::NONE },
                { "cl", AttrType::PROC_NAME },
                { "a", AttrType::NONE }
            },
            {
                { "p", DesignEntity::PROCEDURE },
                { "w", DesignEntity::WHILE },
                { "a", DesignEntity::ASSIGN },
                { "v1", DesignEntity::VARIABLE },
                { "v2", DesignEntity::VARIABLE },
                { "cl", DesignEntity::CALL },
                { "l", DesignEntity::PROG_LINE }
            },
            {
                { "Modifies\r(p\t,v1)", false, RelationType::MODIFIESP,
                    { ArgType::SYNONYM, "p" }, { ArgType::SYNONYM, "v1" } },
                { "Uses(\tw\v,  v1\n)", false, RelationType::USESS,
                    { ArgType::SYNONYM, "w" }, { ArgType::SYNONYM, "v1" } },
                { "Next*\f (\ta, w\v)", false, RelationType::NEXTT,
                    { ArgType::SYNONYM, "a" }, { ArgType::SYNONYM, "w" } },
                { "Follows(\na\r,\fcl  )", false, RelationType::FOLLOWS,
                    { ArgType::SYNONYM, "a" }, { ArgType::SYNONYM, "cl" } }
            },
            {
                { "a(  v1,\r_\"\t69-\v 420\"_ )", false, PatternType::ASSIGN_PATTERN, "a",
                    { ArgType::SYNONYM, "v1" }, { ArgType::INCLUSIVE_PATTERN, "_(69-420)_" } },
                { "w  (v2, _\v\f)", false, PatternType::WHILE_PATTERN, "w",
                    { ArgType::SYNONYM, "v2" }, { ArgType::WILDCARD, "_" } }
            },
            {
                { "\"i\"  =\r\"i\"", false, WithType::LITERAL_EQUAL,
                    { ArgType::IDENTIFIER, { "i", AttrType::NONE } }, { ArgType::IDENTIFIER, { "i", AttrType::NONE } } },
                { "17\r=w\t.  stmt#", false, WithType::INTEGER_EQUAL,
                    { ArgType::INTEGER, { "17", AttrType::NONE } }, { ArgType::ATTRIBUTE, { "w", AttrType::STMT_NUM } } },
                { "l\n=\tcl\r.\vstmt#", false, WithType::INTEGER_EQUAL,
                    { ArgType::SYNONYM, { "l", AttrType::NONE } }, { ArgType::ATTRIBUTE, { "cl", AttrType::STMT_NUM } } },
                { "p\n.\fprocName\r=\n\" function\t\"", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "p", AttrType::PROC_NAME } }, { ArgType::IDENTIFIER, { "function", AttrType::NONE } } },
                { "cl\v.\fprocName\n=  v2  .\tvarName", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "cl", AttrType::PROC_NAME } }, { ArgType::ATTRIBUTE, { "v2", AttrType::VAR_NAME } } }
            }
        };

        Query PROJECT_REPORT_SAMPLE_QUERY_RESULT = {
            STATUS_SUCCESS, PROJECT_REPORT_SAMPLE_QUERY, false,
            {
                { "a1", AttrType::NONE },
                { "w", AttrType::STMT_NUM },
                { "rd", AttrType::VAR_NAME },
                { "pn", AttrType::NONE },
                { "p", AttrType::NONE }
            },
            {
                { "a1", DesignEntity::ASSIGN },
                { "a2", DesignEntity::ASSIGN },
                { "c", DesignEntity::CONSTANT },
                { "l", DesignEntity::PROG_LINE },
                { "w", DesignEntity::WHILE },
                { "ifs", DesignEntity::IF },
                { "rd", DesignEntity::READ },
                { "pn", DesignEntity::PRINT },
                { "p", DesignEntity::PROCEDURE },
                { "v1", DesignEntity::VARIABLE },
                { "v2", DesignEntity::VARIABLE }
            },
            {
                { "Modifies(a1, v1)", false, RelationType::MODIFIESS,
                    { ArgType::SYNONYM, "a1" }, { ArgType::SYNONYM, "v1" } },
                { "Uses(pn, v1)", false, RelationType::USESS,
                    { ArgType::SYNONYM, "pn" }, { ArgType::SYNONYM, "v1" } },
                { "Parent*(w, a1)", false, RelationType::PARENTT,
                    { ArgType::SYNONYM, "w" }, { ArgType::SYNONYM, "a1" } },
                { "Calls(\"driver\", _)", false, RelationType::CALLS,
                    { ArgType::IDENTIFIER, "driver" }, { ArgType::WILDCARD, "_" } },
                { "Uses(p, \"j\")", false, RelationType::USESP,
                    { ArgType::SYNONYM, "p" }, { ArgType::IDENTIFIER, "j" } },
                { "Affects*(1, 64)", false, RelationType::AFFECTST,
                    { ArgType::INTEGER, "1" }, { ArgType::INTEGER, "64" } }
            },
            {
                { "w(\"i\", _)", false, PatternType::WHILE_PATTERN, "w",
                    { ArgType::IDENTIFIER, "i" }, { ArgType::WILDCARD, "_" } },
                { "a1(_, _\"i + 1 + x / 2 + y - (3 * z)\"_)", false, PatternType::ASSIGN_PATTERN, "a1",
                    { ArgType::WILDCARD, "_" }, { ArgType::INCLUSIVE_PATTERN, "_((((i+1)+(x/2))+y)-(3*z))_" } },
                { "ifs(v2, _, _)", false, PatternType::IF_PATTERN, "ifs",
                    { ArgType::SYNONYM, "v2" }, { ArgType::WILDCARD, "_" } }
            },
            {
                { "v2.varName = \"input\"", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "v2", AttrType::VAR_NAME } }, { ArgType::IDENTIFIER, { "input", AttrType::NONE } } },
                { "c.value = l", false, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "c", AttrType::VALUE } }, { ArgType::SYNONYM, { "l", AttrType::NONE } } },
                { "rd.varName = v2.varName", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "rd", AttrType::VAR_NAME } }, { ArgType::ATTRIBUTE, { "v2", AttrType::VAR_NAME } } },
                { "3203 = 3230", false, WithType::LITERAL_EQUAL,
                    { ArgType::INTEGER, { "3203", AttrType::NONE } }, { ArgType::INTEGER, { "3230", AttrType::NONE } } }
            }
        };

        Query EXT_SIMPLE_NOT_QUERY_RESULT = {
            STATUS_SUCCESS, EXT_SIMPLE_NOT_QUERY, false,
            {
                { "a", AttrType::NONE },
                { "w", AttrType::NONE },
                { "v", AttrType::NONE }
            },
            {
                { "a", DesignEntity::ASSIGN },
                { "c", DesignEntity::CONSTANT },
                { "w", DesignEntity::WHILE },
                { "v", DesignEntity::VARIABLE }
            },
            {
                { "not Parent*(w, a)", true, RelationType::PARENTT,
                    { ArgType::SYNONYM, "w" }, { ArgType::SYNONYM, "a" } },
                { "Uses(w, v)", false, RelationType::USESS,
                    { ArgType::SYNONYM, "w" }, { ArgType::SYNONYM, "v" } }
            },
            {
                { "a(v, _)", false, PatternType::ASSIGN_PATTERN, "a",
                    { ArgType::SYNONYM, "v" }, { ArgType::WILDCARD, "_" } },
                { "not w(v, _)", true, PatternType::WHILE_PATTERN, "w",
                    { ArgType::SYNONYM, "v" }, { ArgType::WILDCARD, "_" } }
            },
            {
                { "not a.stmt# = c.value", true, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "a", AttrType::STMT_NUM } }, { ArgType::ATTRIBUTE, { "c", AttrType::VALUE } } },
                { "not \"taiwan\" = \"china\"", true, WithType::LITERAL_EQUAL,
                    { ArgType::IDENTIFIER, { "taiwan", AttrType::NONE } }, { ArgType::IDENTIFIER, { "china", AttrType::NONE } } },
                { "c.value = w.stmt#", false, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "c", AttrType::VALUE } }, { ArgType::ATTRIBUTE, { "w", AttrType::STMT_NUM } } }
            }
        };

        Query EXT_CHAINED_NOT_QUERY_RESULT = {
            STATUS_SUCCESS, EXT_CHAINED_NOT_QUERY, false,
            {
                { "v1", AttrType::NONE },
                { "v2", AttrType::NONE }
            },
            {
                { "c", DesignEntity::CONSTANT },
                { "ifs", DesignEntity::IF },
                { "w", DesignEntity::WHILE },
                { "cl", DesignEntity::CALL },
                { "rd", DesignEntity::READ },
                { "pn", DesignEntity::PRINT },
                { "v1", DesignEntity::VARIABLE },
                { "v2", DesignEntity::VARIABLE }
            },
            {
                { "Uses(pn, v1)", false, RelationType::USESS,
                    { ArgType::SYNONYM, "pn" }, { ArgType::SYNONYM, "v1" } },
                { "not Next*(rd, pn)", true, RelationType::NEXTT,
                    { ArgType::SYNONYM, "rd", }, { ArgType::SYNONYM, "pn" } },
                { "not Next*(pn, rd)", true, RelationType::NEXTT,
                    { ArgType::SYNONYM, "pn" }, { ArgType::SYNONYM, "rd" } },
                { "Modifies(rd, v2)", false, RelationType::MODIFIESS,
                    { ArgType::SYNONYM, "rd" }, { ArgType::SYNONYM, "v2" } }
            },
            {
                { "w(v1, _)", false, PatternType::WHILE_PATTERN, "w",
                    { ArgType::SYNONYM, "v1" }, { ArgType::WILDCARD, "_" } },
                { "not ifs(v1, _, _)", true, PatternType::IF_PATTERN, "ifs",
                    { ArgType::SYNONYM, "v1" }, { ArgType::WILDCARD, "_" } },
                { "not w(v2, _)", true, PatternType::WHILE_PATTERN, "w",
                    { ArgType::SYNONYM, "v2" }, { ArgType::WILDCARD, "_" } },
                { "ifs(v2, _, _)", false, PatternType::IF_PATTERN, "ifs",
                    { ArgType::SYNONYM, "v2" }, { ArgType::WILDCARD, "_" } }
            },
            {
                { "not cl.procName = v1.varName", true, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "cl", AttrType::PROC_NAME } }, { ArgType::ATTRIBUTE, { "v1", AttrType::VAR_NAME } } },
                { "v2.varName = cl.procName", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "v2", AttrType::VAR_NAME } }, { ArgType::ATTRIBUTE, { "cl", AttrType::PROC_NAME } } },
                { "not 2019 = 2020", true, WithType::LITERAL_EQUAL,
                    { ArgType::INTEGER, { "2019", AttrType::NONE } }, { ArgType::INTEGER, { "2020", AttrType::NONE } } },
                { "w.stmt# = c.value", false, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "w", AttrType::STMT_NUM } }, { ArgType::ATTRIBUTE, { "c", AttrType::VALUE } } },
                { "not c.value = ifs.stmt#", true, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "c", AttrType::VALUE } }, { ArgType::ATTRIBUTE, { "ifs", AttrType::STMT_NUM } } }
            }
        };

        Query EXT_CONFUSING_NOT_QUERY_RESULT = {
            STATUS_SUCCESS, EXT_CONFUSING_NOT_QUERY, false,
            {
                { "not", AttrType::NONE }
            },
            {
                { "not", DesignEntity::ASSIGN },
                { "c", DesignEntity::CONSTANT },
                { "v", DesignEntity::VARIABLE }
            },
            {
                { "not Affects(not, _)", true, RelationType::AFFECTS,
                    { ArgType::SYNONYM, "not" }, { ArgType::WILDCARD, "_" } },
                { "Modifies(not, v)", false, RelationType::MODIFIESS,
                    { ArgType::SYNONYM, "not" }, { ArgType::SYNONYM, "v" } }
            },
            {
                { "not(\"x\", _)", false, PatternType::ASSIGN_PATTERN, "not",
                    { ArgType::IDENTIFIER, "x" }, { ArgType::WILDCARD, "_" } },
                { "not not(_, \"0\")", true, PatternType::ASSIGN_PATTERN, "not",
                    { ArgType::WILDCARD, "_" }, { ArgType::EXACT_PATTERN, "0" } },
                { "not(_, _\"0\"_)", false, PatternType::ASSIGN_PATTERN, "not",
                    { ArgType::WILDCARD, "_" }, { ArgType::INCLUSIVE_PATTERN, "_0_" } }
            },
            {
                { "not not.stmt# = 1", true, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "not", AttrType::STMT_NUM } }, { ArgType::INTEGER, { "1", AttrType::NONE } } },
                { "not.stmt# = c.value", false, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "not", AttrType::STMT_NUM } }, { ArgType::ATTRIBUTE, { "c", AttrType::VALUE } } }

            }
        };

        Query EXT_CONFUSING_NOT_AND_QUERY_RESULT = {
            STATUS_SUCCESS, EXT_CONFUSING_NOT_AND_QUERY, false,
            {
                { "and", AttrType::NONE }
            },
            {
                { "and", DesignEntity::ASSIGN },
                { "pattern", DesignEntity::PROCEDURE },
                { "not", DesignEntity::PROG_LINE },
                { "with", DesignEntity::VARIABLE }
            },
            { },
            {
                { "and(with, _)", false, PatternType::ASSIGN_PATTERN, "and",
                    { ArgType::SYNONYM, "with" }, { ArgType::WILDCARD, "_" } }
            },
            {
                { "not not = 1337", true, WithType::INTEGER_EQUAL,
                    { ArgType::SYNONYM, { "not", AttrType::NONE } }, { ArgType::INTEGER, { "1337", AttrType::NONE } } },
                { "not and.stmt# = 62353535", true, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "and", AttrType::STMT_NUM } }, { ArgType::INTEGER, { "62353535", AttrType::NONE } } },
                { "not = and.stmt#", false, WithType::INTEGER_EQUAL,
                    { ArgType::SYNONYM, { "not", AttrType::NONE } }, { ArgType::ATTRIBUTE, { "and", AttrType::STMT_NUM } } },
                { "not with.varName = pattern.procName", true, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "with", AttrType::VAR_NAME } }, { ArgType::ATTRIBUTE, { "pattern", AttrType::PROC_NAME } } },
                { "and.stmt# = not", false, WithType::INTEGER_EQUAL,
                    { ArgType::ATTRIBUTE, { "and", AttrType::STMT_NUM } }, { ArgType::SYNONYM, { "not", AttrType::NONE } } },
                { "not \"not\" = pattern.procName", true, WithType::IDENTIFIER_EQUAL,
                    { ArgType::IDENTIFIER, { "not", AttrType::NONE } }, { ArgType::ATTRIBUTE, { "pattern", AttrType::PROC_NAME } } }
            }
        };

        // Array containing all positive testcases, each a pair comprising a valid query and the
        // expected Query object produced by the Query Parser after parsing and validation
        vector<pair<string, Query>> VALID_QUERY_TESTCASES = {
            { TRIVIAL_SYNONYM_QUERY, TRIVIAL_SYNONYM_QUERY_RESULT },
            { TRIVIAL_BOOLEAN_QUERY, TRIVIAL_BOOLEAN_QUERY_RESULT },
            { TRIVIAL_ATTRIBUTE_QUERY, TRIVIAL_ATTRIBUTE_QUERY_RESULT },
            { TRIVIAL_MANY_DECLARATIONS_QUERY, TRIVIAL_MANY_DECLARATIONS_QUERY_RESULT },
            { TRIVIAL_ONE_TUPLE_QUERY, TRIVIAL_ONE_TUPLE_QUERY_RESULT },
            { TRIVIAL_TUPLE_QUERY, TRIVIAL_TUPLE_QUERY_RESULT },
            { TRIVIAL_TUPLE_ATTRIBUTES_QUERY, TRIVIAL_TUPLE_ATTRIBUTES_QUERY_RESULT },
            { TRIVIAL_REDUNDANT_ATTRIBUTE_QUERY, TRIVIAL_REDUNDANT_ATTRIBUTE_QUERY_RESULT },
            { TRIVIAL_TUPLE_MIXED_QUERY, TRIVIAL_TUPLE_MIXED_QUERY_RESULT },
            { SIMPLE_CONSTANTS_QUERY, SIMPLE_CONSTANTS_QUERY_RESULT },
            { SIMPLE_VALID_QUERY, SIMPLE_VALID_QUERY_RESULT },
            { SIMPLE_VALID_BOOLEAN_QUERY, SIMPLE_VALID_BOOLEAN_QUERY_RESULT },
            { SIMPLE_VALID_TUPLE_QUERY, SIMPLE_VALID_TUPLE_QUERY_RESULT },
            { SIMPLE_VALID_ATTRIBUTES_QUERY, SIMPLE_VALID_ATTRIBUTES_QUERY_RESULT },
            { SIMPLE_PATTERN_STRING_QUERY, SIMPLE_PATTERN_STRING_QUERY_RESULT },
            { SIMPLE_LITERAL_INTEGER_EQUALITY_QUERY, SIMPLE_LITERAL_INTEGER_EQUALITY_QUERY_RESULT },
            { SIMPLE_LITERAL_IDENTIFIER_EQUALITY_QUERY, SIMPLE_LITERAL_IDENTIFIER_EQUALITY_QUERY_RESULT },
            { SIMPLE_MIXED_EQUALITY_QUERY, SIMPLE_MIXED_EQUALITY_QUERY_RESULT },
            { SIMPLE_INTEGER_EQUALITY_QUERY, SIMPLE_INTEGER_EQUALITY_QUERY_RESULT },
            { SIMPLE_IDENTIFIER_EQUALITY_QUERY, SIMPLE_IDENTIFIER_EQUALITY_QUERY_RESULT },
            { COMPLEX_VALID_BASIC_QUERY, COMPLEX_VALID_BASIC_QUERY_RESULT },
            { COMPLEX_VALID_ATTRIBUTES_QUERY, COMPLEX_VALID_ATTRIBUTES_QUERY_RESULT },
            { COMPLEX_PATTERN_STRING_QUERY, COMPLEX_PATTERN_STRING_QUERY_RESULT },
            { COMPLEX_VALID_ADVANCED_QUERY, COMPLEX_VALID_ADVANCED_QUERY_RESULT },
            { CHAINED_BOOLEAN_QUERY, CHAINED_BOOLEAN_QUERY_RESULT },
            { CHAINED_BASIC_RELATIONS_QUERY, CHAINED_BASIC_RELATIONS_QUERY_RESULT },
            { CHAINED_PROCEDURE_RELATIONS_QUERY, CHAINED_PROCEDURE_RELATIONS_QUERY_RESULT },
            { CHAINED_MIXED_CLAUSES_QUERY, CHAINED_MIXED_CLAUSES_QUERY_RESULT },
            { CHAINED_MIXED_RETURN_QUERY, CHAINED_MIXED_RETURN_QUERY_RESULT },
            { CHAINED_CALLS_QUERY, CHAINED_CALLS_QUERY_RESULT },
            { CHAINED_NEXT_QUERY, CHAINED_NEXT_QUERY_RESULT },
            { CHAINED_AFFECTS_QUERY, CHAINED_AFFECTS_QUERY_RESULT },
            { CHAINED_STMT_PROG_LINE_QUERY, CHAINED_STMT_PROG_LINE_QUERY_RESULT },
            { CHAINED_ADV_RELATIONS_QUERY, CHAINED_ADV_RELATIONS_QUERY_RESULT },
            { CHAINED_EQUALITIES_QUERY, CHAINED_EQUALITIES_QUERY_RESULT },
            { CHAINED_CONFUSING_AND_QUERY, CHAINED_CONFUSING_AND_QUERY_RESULT },
            { CHAINED_ALL_RELATIONS_QUERY, CHAINED_ALL_RELATIONS_QUERY_RESULT },
            { CHAINED_ALL_PATTERNS_QUERY, CHAINED_ALL_PATTERNS_QUERY_RESULT },
            { CHAINED_ALL_NON_LITERAL_EQUALITIES_QUERY, CHAINED_ALL_NON_LITERAL_EQUALITIES_QUERY_RESULT },
            { CHAINED_NO_WHITESPACE_QUERY, CHAINED_NO_WHITESPACE_QUERY_RESULT },
            { CHAINED_EXTRA_WHITESPACE_QUERY, CHAINED_EXTRA_WHITESPACE_QUERY_RESULT },
            { PROJECT_REPORT_SAMPLE_QUERY, PROJECT_REPORT_SAMPLE_QUERY_RESULT },
            { EXT_SIMPLE_NOT_QUERY, EXT_SIMPLE_NOT_QUERY_RESULT },
            { EXT_CHAINED_NOT_QUERY, EXT_CHAINED_NOT_QUERY_RESULT },
            { EXT_CONFUSING_NOT_QUERY, EXT_CONFUSING_NOT_QUERY_RESULT },
            { EXT_CONFUSING_NOT_AND_QUERY, EXT_CONFUSING_NOT_AND_QUERY_RESULT }
        };

        // Array containing all negative testcases, each comprising an invalid query with a single
        // syntax or semantic error and the expected error message returned by the Query Parser
        vector<pair<string, string>> INVALID_QUERY_TESTCASES = {
            // validateQuerySyntax
            { EMPTY_QUERY, SYNTAX_ERR_EMPTY_QUERY },
            { FOREIGN_CHARACTER_IN_DECLARATION_QUERY, SYNTAX_ERR_INVALID_DECLARATION },
            { INCORRECT_DECLARATION_SYNTAX_QUERY, SYNTAX_ERR_INVALID_DECLARATION },
            { MISSING_BODY_QUERY, SYNTAX_ERR_MISSING_SELECT_OR_UNKNOWN_CHAR },
            { MISSING_BODY_SUFFIX_QUERY, SYNTAX_ERR_MISSING_SELECT_OR_UNKNOWN_CHAR },
            { MISSING_SELECT_QUERY, SYNTAX_ERR_MISSING_SELECT_OR_UNKNOWN_CHAR },
            { ENDING_SEMICOLON_QUERY, SYNTAX_ERR_MISSING_SELECT_OR_UNKNOWN_CHAR },
            { FOREIGN_CHARACTER_IN_BODY_QUERY, SYNTAX_ERR_MISSING_SELECT_OR_UNKNOWN_CHAR },
            // validateQuerySemantics
            { AMBIGUOUS_BOOLEAN_QUERY, SEMANTIC_ERR_AMBIGUOUS_USE_OF_BOOLEAN },
            { INCORRECT_BOOLEAN_KEYWORD_QUERY, SEMANTIC_ERR_UNDECLARED_SYNONYM_IN_RETURN_TYPE },
            { MISSING_RETURN_TYPE_SYNONYM_QUERY, SEMANTIC_ERR_UNDECLARED_SYNONYM_IN_RETURN_TYPE },
            { MISSING_BOOLEAN_IN_TUPLE_QUERY, SEMANTIC_ERR_UNDECLARED_SYNONYM_IN_RETURN_TYPE },
            { MISSING_SYNONYM_IN_TUPLE_QUERY, SEMANTIC_ERR_UNDECLARED_SYNONYM_IN_RETURN_TYPE },
            { INVALID_PROG_LINE_ATTRIBUTE_QUERY, SEMANTIC_ERR_INVALID_SYNONYM_ATTRIBUTE_IN_RETURN_TYPE },
            { INVALID_SYNONYM_ATTRIBUTE_QUERY, SEMANTIC_ERR_INVALID_SYNONYM_ATTRIBUTE_IN_RETURN_TYPE },
            { INVALID_SYNONYM_ATTRIBUTE_TYPE_IN_TUPLE_QUERY, SEMANTIC_ERR_INVALID_SYNONYM_ATTRIBUTE_IN_RETURN_TYPE },
            { USESS_AMBIGUOUS_WILDCARD_QUERY, SEMANTIC_ERR_USES_MODIFIES_AMBIGUOUS_WILDCARD },
            { MODIFIESS_MISSING_FIRST_SYNONYM_QUERY, SEMANTIC_ERR_USES_MODIFIES_UNDECLARED_FIRST_SYNONYM },
            { USESS_UNDEFINED_RELATION_QUERY, SEMANTIC_ERR_USES_INVALID_FIRST_SYNONYM },
            { MODIFIESS_UNDEFINED_RELATION_QUERY, SEMANTIC_ERR_MODIFIES_INVALID_FIRST_SYNONYM },
            { USESS_MISSING_SECOND_SYNONYM_QUERY, SEMANTIC_ERR_USES_MODIFIES_UNDECLARED_SECOND_SYNONYM },
            { MODIFIESP_MISSING_SECOND_SYNONYM_QUERY, SEMANTIC_ERR_USES_MODIFIES_UNDECLARED_SECOND_SYNONYM },
            { USESP_LITERAL_MISSING_SECOND_SYNONYM_QUERY, SEMANTIC_ERR_USES_MODIFIES_UNDECLARED_SECOND_SYNONYM },
            { USESP_NON_VARIABLE_ARG_QUERY, SEMANTIC_ERR_USES_MODIFIES_NON_VARIABLE_SECOND_SYNONYM },
            { MODIFIESS_NON_VARIABLE_ARG_QUERY, SEMANTIC_ERR_USES_MODIFIES_NON_VARIABLE_SECOND_SYNONYM },
            { MODIFIESP_LITERAL_NON_VARIABLE_ARG_QUERY, SEMANTIC_ERR_USES_MODIFIES_NON_VARIABLE_SECOND_SYNONYM },
            { CALLS_MISSING_SYNONYM_QUERY, SEMANTIC_ERR_CALLS_UNDECLARED_SYNONYM },
            { CALLS_NON_PROCEDURE_ARG_QUERY, SEMANTIC_ERR_CALLS_NON_PROCEDURE_SYNONYM },
            { INVALID_STMT_NUMBER_OVERFLOW_QUERY, SEMANTIC_ERR_FPNA_STMT_NUMBER_OVERFLOW },
            { INVALID_STMT_NUMBER_QUERY, SEMANTIC_ERR_FPNA_NON_POSITIVE_STMT_NUMBER },
            { INVALID_PROG_LINE_QUERY, SEMANTIC_ERR_FPNA_NON_POSITIVE_STMT_NUMBER },
            { FOLLOWS_MISSING_SYNONYM_QUERY, SEMANTIC_ERR_FPNA_UNDECLARED_SYNONYM },
            { NEXT_MISSING_SYNONYM_QUERY, SEMANTIC_ERR_FPNA_UNDECLARED_SYNONYM },
            { AFFECTS_NON_ASSIGN_ARG_QUERY, SEMANTIC_ERR_AFFECTS_NON_ASSIGN_SYNONYM },
            { PARENT_NON_STMT_ARG_QUERY, SEMANTIC_ERR_FPN_NON_STMT_SYNONYM },
            { NEXT_NON_STMT_ARG_QUERY, SEMANTIC_ERR_FPN_NON_STMT_SYNONYM },
            { PATTERN_MISSING_SYNONYM_QUERY, SEMANTIC_ERR_PATTERN_UNDECLARED_FIRST_SYNONYM },
            { PATTERN_NON_VARIABLE_ARG_QUERY, SEMANTIC_ERR_PATTERN_NON_VARIABLE_FIRST_SYNONYM },
            { WITH_DIFFERING_LITERAL_ARG_TYPE_QUERY, SEMANTIC_ERR_WITH_CLAUSE_DIFF_LITERAL_TYPE },
            { WITH_MISSING_SYNONYM_ARG_QUERY, SEMANTIC_ERR_WITH_CLAUSE_UNDECLARED_SYNONYM_ARG },
            { WITH_NON_PROG_LINE_SYNONYM_ARG_QUERY, SEMANTIC_ERR_WITH_CLAUSE_NON_PROG_LINE_SYNONYM_ARG },
            { WITH_MISSING_SYNONYM_IN_ATTRIBUTE_ARG_QUERY, SEMANTIC_ERR_WITH_CLAUSE_UNDECLARED_SYNONYM_IN_ATTRIBUTE_ARG },
            { WITH_INVALID_SYNONYM_ATTRIBUTE_TYPE_QUERY, SEMANTIC_ERR_WITH_CLAUSE_INVALID_SYNONYM_ATTRIBUTE_PAIR_ARG },
            { WITH_INVALID_LITERAL_SYNONYM_ARGS_QUERY, SEMANTIC_ERR_WITH_CLAUSE_DIFF_RETURN_TYPE_OF_ARGS },
            { WITH_INVALID_LITERAL_INTEGER_ATTRIBUTE_ARGS_QUERY, SEMANTIC_ERR_WITH_CLAUSE_DIFF_RETURN_TYPE_OF_ARGS },
            { WITH_INVALID_LITERAL_IDENTIFIER_ATTRIBUTE_ARGS_QUERY, SEMANTIC_ERR_WITH_CLAUSE_DIFF_RETURN_TYPE_OF_ARGS },
            { WITH_INVALID_SYNONYM_ATTRIBUTE_ARGS_QUERY, SEMANTIC_ERR_WITH_CLAUSE_DIFF_RETURN_TYPE_OF_ARGS },
            { WITH_INVALID_ATTRIBUTE_ATTRIBUTE_ARGS_QUERY, SEMANTIC_ERR_WITH_CLAUSE_DIFF_RETURN_TYPE_OF_ARGS },
            // splitClauses
            { INVALID_AND_RELATION_QUERY, SYNTAX_ERR_INVALID_AND_CHAINED_CLAUSES },
            { INVALID_AND_PATTERN_QUERY, SYNTAX_ERR_INVALID_AND_CHAINED_CLAUSES },
            { INVALID_AND_WITH_QUERY, SYNTAX_ERR_INVALID_AND_CHAINED_CLAUSES },
            { MISSING_RETURN_TYPE_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            { INCORRECT_RELATION_KEYWORD_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            { INCORRECT_PATTERN_KEYWORD_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            { INCORRECT_EQUALITY_KEYWORD_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            { RELATION_INVALID_ARG_CHARACTER_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            { RELATION_INVALID_ATTRIBUTE_ARG_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            { PATTERN_INVALID_SYNTAX_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            { PATTERN_TOO_FEW_ARGS_QUERY, SYNTAX_ERR_MISSING_OR_MALFORMED_PATTERN_ARG },
            { PATTERN_INVALID_FIRST_ARG_CHARACTER_QUERY, SYNTAX_ERR_MISSING_OR_MALFORMED_PATTERN_ARG },
            { PATTERN_INVALID_SECOND_ARG_CHARACTER_QUERY, SYNTAX_ERR_MISSING_OR_MALFORMED_PATTERN_ARG },
            { PATTERN_INVALID_ATTRIBUTE_ARG_QUERY, SYNTAX_ERR_MISSING_OR_MALFORMED_PATTERN_ARG },
            { PATTERN_MALFORMED_PATTERN_STRING_QUERY, SYNTAX_ERR_MISSING_OR_MALFORMED_PATTERN_ARG },
            { PATTERN_UNBALANCED_UNDERSCORES_QUERY, SYNTAX_ERR_MISSING_OR_MALFORMED_PATTERN_ARG },
            { INVALID_SYNONYM_PATTERN_STRING_QUERY, SYNTAX_ERR_MISSING_OR_MALFORMED_PATTERN_ARG },
            { WITH_INCORRECT_EQUALITY_OPERATOR_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            { WITH_INVALID_OPERATOR_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            { WITH_INVALID_WILDCARD_ARG_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            { WITH_UNQUOTED_IDENTIFIER_ARG_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            { WITH_INVALID_ATTRIBUTE_SYNTAX_QUERY, SYNTAX_ERR_INVALID_CLAUSES_IN_QUERY_BODY },
            // parseDeclarations
            { DUPLICATE_BODY_QUERY, SYNTAX_ERR_UNKNOWN_DESIGN_ENTITY_KEYWORD },
            { UNRECOGNISED_DESIGN_ENTITY_QUERY, SYNTAX_ERR_UNKNOWN_DESIGN_ENTITY_KEYWORD },
            { CONFLICTING_SYNONYM_QUERY, SEMANTIC_ERR_CONFLICTING_SYNONYM_DECLARATIONS },
            // parseQueryTarget
            { INVALID_RETURN_TYPE_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { INVALID_LITERAL_RETURN_TYPE_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { INVALID_RETURN_SYNONYM_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { MULTIPLE_RETURN_TYPE_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { MISSING_ATTRIBUTE_KEYWORD_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { INVALID_ATTRIBUTE_SPECIFIER_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { INVALID_ATTRIBUTE_CHARACTER_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { INVALID_ATTRIBUTE_SYNTAX_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { EMPTY_TUPLE_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { INVALID_TUPLE_SPECIFIER_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { INVALID_TUPLE_DELIMITER_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { INVALID_SYNONYM_IN_TUPLE_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { MISSING_ATTRIBUTE_KEYWORD_IN_TUPLE_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { INVALID_SINGLE_WITH_TUPLE_QUERY, SYNTAX_ERR_MISSING_OR_INVALID_QUERY_TARGET },
            { INVALID_ATTRIBUTE_KEYWORD_QUERY, SYNTAX_ERR_INVALID_ATTRIBUTE_KEYWORD_IN_RETURN_TYPE },
            { INVALID_ATTRIBUTE_KEYWORD_IN_TUPLE_QUERY, SYNTAX_ERR_INVALID_ATTRIBUTE_KEYWORD_IN_RETURN_TYPE },
            // parseRelationClauses
            { INVALID_RELATION_TYPE_QUERY, SYNTAX_ERR_INVALID_RELATION_KEYWORD },
            { INVALID_RELATION_KEYWORD_QUERY, SYNTAX_ERR_INVALID_RELATION_KEYWORD },
            { INCORRECT_RELATION_NUM_ARGS_QUERY, SYNTAX_ERR_RELATION_INVALID_NUM_ARGS },
            { FOLLOWS_NON_STMT_REF_ARG_QUERY, SYNTAX_ERR_FOLLOWS_PARENTS_INVALID_STMT_REF },
            { PARENT_NON_STMT_REF_ARG_QUERY, SYNTAX_ERR_FOLLOWS_PARENTS_INVALID_STMT_REF },
            { USES_INVALID_SECOND_ARG_QUERY, SYNTAX_ERR_USES_MODIFIES_INVALID_SECOND_ENT_REF },
            { MODIFIES_INVALID_FIRST_ARG_QUERY, SYNTAX_ERR_USES_MODIFIES_INVALID_FIRST_ARG },
            { CALLS_NON_ENTITY_REF_ARG_QUERY, SYNTAX_ERR_CALLS_INVALID_ENT_REF },
            { NEXT_NON_LINE_REF_ARG_QUERY, SYNTAX_ERR_NEXT_INVALID_LINE_REF },
            { AFFECTS_NON_STMT_REF_QUERY, SYNTAX_ERR_AFFECTS_INVALID_STMT_REF },
            // parsePatternClauses
            { PATTERN_UNDECLARED_SYNONYM_QUERY, SEMANTIC_ERR_UNDECLARED_PATTERN_TYPE_SYNONYM },
            { PATTERN_NON_ENTITY_REF_ARG_QUERY, SYNTAX_ERR_PATTERN_INVALID_ENT_REF },
            { PATTERN_ASSIGN_TOO_MANY_ARGS_QUERY, SYNTAX_ERR_ASSIGN_PATTERN_INVALID_NUM_ARGS },
            { PATTERN_STRING_INVALID_NAME_QUERY, SYNTAX_ERR_ASSIGN_PATTERN_INVALID_PATTERN },
            { PATTERN_STRING_INVALID_STRUCTURE_QUERY, SYNTAX_ERR_ASSIGN_PATTERN_INVALID_PATTERN },
            { PATTERN_STRING_INVALID_PARENTHESES_QUERY, SYNTAX_ERR_ASSIGN_PATTERN_INVALID_PATTERN },
            { PATTERN_WHILE_TOO_MANY_ARGS_QUERY, SYNTAX_ERR_WHILE_PATTERN_INVALID_NUM_ARGS },
            { PATTERN_WHILE_INVALID_SECOND_ARG_QUERY, SYNTAX_ERR_WHILE_PATTERN_INVALID_SECOND_ARG },
            { PATTERN_IF_TOO_FEW_ARGS_QUERY, SYNTAX_ERR_IF_PATTERN_INVALID_NUM_ARGS },
            { PATTERN_IF_INVALID_ARGS_QUERY, SYNTAX_ERR_IF_PATTERN_INVALID_SECOND_THIRD_ARG },
            { INVALID_PATTERN_TYPE_QUERY, SYNTAX_ERR_INVALID_PATTERN_TYPE },
            // parseWithClauses
            { WITH_INVALID_INTEGER_ARG_QUERY, SYNTAX_ERR_WITH_CLAUSE_INVALID_REF_ARG },
            { WITH_INVALID_IDENTIFIER_ARG_QUERY, SYNTAX_ERR_WITH_CLAUSE_INVALID_REF_ARG },
            { WITH_INVALID_SYNONYM_ARG_QUERY, SYNTAX_ERR_WITH_CLAUSE_INVALID_REF_ARG },
            { WITH_INVALID_ATTRIBUTE_KEYWORD_QUERY, SYNTAX_ERR_WITH_CLAUSE_INVALID_ATTRIBUTE_KEYWORD }
        };

        TEST_CLASS_INITIALIZE(initParser) {
            parser = PQL::QueryParser();
        }

        TEST_METHOD(parseQueryValidCases) {
            for (auto testcase : VALID_QUERY_TESTCASES) {
                Query result = parser.parseQuery(testcase.first);
                Assert::IsTrue(result == testcase.second);
            }
        }

        TEST_METHOD(parseQueryInvalidCases) {
            for (pair<string, string> testcase : INVALID_QUERY_TESTCASES) {
                Query result = parser.parseQuery(testcase.first);
                Assert::IsTrue(result.status == testcase.second);
            }
        }

    };
}
