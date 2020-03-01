#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "PQL.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PQL;
using namespace SIMPLE;

namespace IntegrationTesting
{		
  PKB::PKB pkb;
  PQL::PQLManager pql;
 	FrontEnd::DesignExtractor DE;
  std::list<std::string> results;

	TEST_CLASS(Test-PKB_PQL)
	{
    std::string testQuery1 = "stmt s; Select s";
    std::list<std::string> testQueryResult1 = {"1", "2", "3"};
    std::string testQuery2 = "stmt s; Select s such that Follows(1, s)";
    std::list<std::string> testQueryResult2 = {"2"};
    std::string testQuery3 = "stmt s; Select s such that Parent(2, s)";
    std::list<std::string> testQueryResult3 = {"3"};

	public:
		TEST_CLASS_INITIALIZE(setup) {
      pkb = PKB::PKB();
			pkb.stmtTable.insertStmt(&ReadStmt(1));
			pkb.stmtTable.insertStmt(&ReadStmt(2));
			pkb.stmtTable.insertStmt(&ReadStmt(3));
 			pkb.parentKB.addParent(2, 3);
      pkb = DE.run(pkb);
      pql = PQL::PQLManager(pkb);
    }
    
		TEST_METHOD(evaluateQuery)
		{
      pql.evaluateQuery(testQuery1, &results);
      CollectionAssert::AreEqual(results, testQueryResult1);
      pql.evaluateQuery(testQuery2, &results);
      CollectionAssert::AreEqual(results, testQueryResult2);
      pql.evaluateQuery(testQuery3, &results);
      CollectionAssert::AreEqual(results, testQueryResult3);
		}

	};
}
