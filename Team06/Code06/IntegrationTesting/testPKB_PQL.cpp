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

        string select_DesignEntity_STMT = "stmt s; Select s";
        string select_DesignEntity_READ = "read r; Select r";
        string select_DesignEntity_PRINT = "print pn; Select pn";
        string select_DesignEntity_CALL = "call c; Select c";
        string select_DesignEntity_WHILE = "while w; Select w";
        string select_DesignEntity_IF = "if ifs; Select ifs";
        string select_DesignEntity_ASSIGN = "assign a; Select a";
        string select_DesignEntity_VAR = "variable v; Select v";
        string select_DesignEntity_CONST = "variable v; Select v";
        string select_DesignEntity_PROGLINE = "prog_line n; Select n";
        string select_DesignEntity_PROC = "procedure p; Select p";
        
        
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
