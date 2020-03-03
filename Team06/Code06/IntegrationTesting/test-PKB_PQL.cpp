#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PQL.h"
#include "Simple.h"

using std::string;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PQL;
using namespace SIMPLE;

namespace IntegrationTesting {
    PQL::PQLManager pql = PQL::PQLManager(PKB::PKB());
    FrontEnd::DesignExtractor DE;
    std::list<string> results;

    TEST_CLASS(TestPKB_PQL) {
    public:
        string testQuery1 = "stmt s; Select s";
        std::list<string> testQueryResult1 = {"1", "2", "3"};
        std::string testQuery2 = "stmt s; Select s such that Follows(1, s)";
        std::list<string> testQueryResult2 = {"2"};
        std::string testQuery3 = "stmt s; Select s such that Parent(2, s)";
        std::list<string> testQueryResult3 = {"3"};

    TEST_CLASS_INITIALIZE(setup) {
        PKB::PKB pkb = PKB::PKB();
        pkb.stmtTable.insertStmt(&ReadStmt(1));
        pkb.stmtTable.insertStmt(&ReadStmt(2));
        pkb.stmtTable.insertStmt(&ReadStmt(3));
        pkb.parentKB.addParent(2, 3);
        pkb = DE.run(pkb);
        pql = PQL::PQLManager(pkb);
    }

    TEST_METHOD(evaluateQuery) {
        pql.evaluateQuery(testQuery1, results);
        Assert::IsTrue(results == testQueryResult1);
        pql.evaluateQuery(testQuery2, results);
        Assert::IsTrue(results == testQueryResult2);
        pql.evaluateQuery(testQuery3, results);
        Assert::IsTrue(results == testQueryResult3);
    }

  };
}
