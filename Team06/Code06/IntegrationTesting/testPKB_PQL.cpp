#include "stdafx.h"
#include "CppUnitTest.h"
#include "PQL.h"
#include "Simple.h"

using std::string;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PQL;
using namespace SIMPLE;

namespace IntegrationTesting {
    PQL::PQLManager pql = PQL::PQLManager(PKB::PKB());
    std::list<string> results;

    TEST_CLASS(TestPKB_PQL) {
    public:

        string query_selectDesignEntity_STMT = "stmt s; Select s";
        string query_selectDesignEntity_READ = "read r; Select r";
        string query_selectDesignEntity_PRINT = "print pn; Select pn";
        string query_selectDesignEntity_CALL = "call c; Select c";
        string query_selectDesignEntity_WHILE = "while w; Select w";
        string query_selectDesignEntity_IF = "if ifs; Select ifs";
        string query_selectDesignEntity_ASSIGN = "assign a; Select a";
        string query_selectDesignEntity_VAR = "variable v; Select v";
        string query_selectDesignEntity_CONST = "variable v; Select v";
        string query_selectDesignEntity_PROGLINE = "prog_line n; Select n";
        string query_selectDesignEntity_PROC = "procedure p; Select p";
        
        string query_relCond_ModifiesP = "variable v; procedure p; Select p such that Modifies(p,v)";
        string query_relCond_ModifiesS = "variable v; assign a; Select BOOLEAN such that Modifies(a,v)";
        string query_relCond_UsesP = "variable v; procedure p; Select p such that Uses(p,v)";
        string query_relCond_UsesS = "variable v; assign a; Select BOOLEAN such that Uses(a,v)";
        string query_relCond_Calls = "procedure p1, p2; Select p1 such that Calls(p1, p2)";
        string query_relCond_CallsStar = "procedure p1, p2; Select BOOLEAN such that Calls*(p1, p2)";
        string query_relCond_Parent = "stmt s1, s2; Select s1 such that Parent(s1, s2)";
        string query_relCond_ParentStar = "stmt s1, s2; Select BOOLEAN such that Parent*(s1, s2)";
        string query_relCond_Follows = "stmt s1, s2; Select s1 such that Follows(s1, s2)";
        string query_relCond_FollowsStar = "stmt s1, s2; Select BOOLEAN such that Follows*(s1, s2)";
        string query_relCond_Next = "prog_line n1, n2; Select n1 such that Next(n1, n2)";
        string query_relCond_NextStar = "prog_line n1, n2; Select BOOLEAN such that Next*(n1, n2)";
        string query_relCond_Affects = "assign a1, a2; Select a1 such that Affects(n1, n2)";
        string query_relCond_AffectsStar = "assign a1, a2; Select BOOLEAN such that Affects*(n1, n2)";
        
        std::list<string> testQueryResult = {"1", "2", "3"};

    TEST_CLASS_INITIALIZE(setup) {
        PKB::PKB pkb = PKB::PKB();
        pkb.stmtTable.insertStmt(&ReadStmt(1));
        pkb.stmtTable.insertStmt(&ReadStmt(2));
        pkb.stmtTable.insertStmt(&ReadStmt(3));
        pkb.parentKB.addParent(2, 3);
        pql = PQL::PQLManager(pkb);
    }

    TEST_METHOD(evaluateQuery) {
        pql.evaluateQuery(testQuery, results);
        Assert::IsTrue(results == testQueryResult);
    }

  };
}
