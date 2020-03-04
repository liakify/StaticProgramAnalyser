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
        string testQuery = "stmt s; Select s";
        std::list<string> testQueryResult = {"1", "2", "3"};

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
        pql.evaluateQuery(testQuery, results);
        Assert::IsTrue(results == testQueryResult);
    }

  };
}
