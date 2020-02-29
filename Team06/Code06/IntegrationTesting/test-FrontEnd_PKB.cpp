#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"
#include "Types.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{		
  PKB::PKB pkb;
  FrontEnd::Parser parser;

	TEST_CLASS(Test-FrontEnd_PKB)
	{
    std::string src = "procedure p{x = 7; y = 2; if(x>y) then {z = x + y;} else {print y;} read a; }";

    std::unordered_set<VarName> allVars;
    allVars.insert("x");
    allVars.insert("y");
    allVars.insert("z");
    allVars.insert("a");

    std::list<std::string> allConstants = {"7", "2"};

    std::string procedureName = "p";

    std::int stmtCount_ASSIGN = 3;
    std::int stmtCount_READ = 1;
    std::int stmtCount_PRINT = 1;
    std::int stmtCount_CALL = 0;
    std::int stmtCount_IF = 1;
    std::int stmtCount_WHILE = 0;

	public:
		TEST_CLASS_INITIALIZE(setup) {
      parser = FrontEnd::Parser();
      pkb = parser.parseSimple(src);
    }

		TEST_METHOD(parseSimple)
		{
			CollectionAssert::AreEqual(pkb.varTable.getAllVars(), allVars);

      Assert::IsTrue(pkb.constTable.size() == allConstants.size());
      for (auto itr = allConstants.begin(); itr != allConstants.end(); ++itr){
        Assert::IsFalse(pkb.constTable.getConstId(*itr) == -1);
      }
     	
      Assert::IsTrue(pkb.procTable.size() == 1);
      Assert::IsFalse(pkb.constTable.getProcId(procedureName) == -1);

      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::ASSIGN).size() == stmtCount_ASSIGN);
      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::READ).size() == stmtCount_READ);
      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::PRINT).size() == stmtCount_PRINT);
      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::CALL).size() == stmtCount_CALL);
      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::IF).size() == stmtCount_IF);
      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::WHILE).size() == stmtCount_WHILE);
		}

	};
}
