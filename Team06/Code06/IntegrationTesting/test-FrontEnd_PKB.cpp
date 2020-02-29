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

	TEST_CLASS(UnitTest1)
	{
    std::string src = "procedure p{x = 7; y = 2; if(x>y) then {z = x + y;} else {print y;} read a; }";

    std::unordered_set<VarName> allVars;
    allVars.insert("x");
    allVars.insert("y");
    allVars.insert("z");
    allVars.insert("a");



	public:
		TEST_CLASS_INITIALIZE(setup) {
      parser = FrontEnd::Parser();
      pkb = parser.parseSimple(src);
    }

		TEST_METHOD(Test-FrontEnd_PKB)
		{
			CollectionAssert::AreEqual(pkb.varTable.getAllVars(), allVars);
      Assert::IsTrue(pkb.constTable.size() == 2);
     	Assert::IsFalse(pkb.constTable.getConstId("7") == -1);
      Assert::IsFalse(pkb.constTable.getConstId("2") == -1);
      Assert::IsTrue(pkb.procTable.size() == 1);
      Assert::IsFalse(pkb.constTable.getProcId("p") == -1);
		}

	};
}
