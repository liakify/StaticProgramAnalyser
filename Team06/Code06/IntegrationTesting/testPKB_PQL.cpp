#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "PQL.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PQL;

namespace IntegrationTesting
{		
  PKB::PKB pkb;
  PQL::PQLManager pql;
  std::list<std::string> results;

	TEST_CLASS(Test)
	{
    std::string testQuery = "stmt s; Select s";
    std::list<std::string> testQueryResult = {"1", "2", "3"};

	public:
		TEST_CLASS_INITIALIZE(setup) {
      pkb = PKB::PKB();
			pkb.stmtTable.insertStmt(&ReadStmt(1));
			pkb.stmtTable.insertStmt(&ReadStmt(2));
			pkb.stmtTable.insertStmt(&ReadStmt(3));
      pql = PQL::PQLManager(pkb);
    }
    
		TEST_METHOD(evaluateQuery)
		{
      pql.evaluateQuery(testQuery, &results);
      CollectionAssert::AreEqual(results, testQueryResult);
		}

	};
}
