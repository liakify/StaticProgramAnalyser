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
    std::string src = "procedure p{x = 7; y = 2; if(x>y) then {z = x + a;} else {print y;} read y; }";

    std::unordered_set<VarName> allVars;
    allVars.insert("x");
    allVars.insert("y");
    allVars.insert("z");
    allVars.insert("a");

    std::unordered_set<VarName> allModifiedVars;
    allModifiedVars.insert("x");
    allModifiedVars.insert("y");
    allModifiedVars.insert("z");

    std::unordered_set<StmtId> allModifiedStmts;
    allModifiedStmts.insert(1);
    allModifiedStmts.insert(2);
    allModifiedStmts.insert(4);

    std::unordered_set<VarName> allUsedVars;
    allUsedVars.insert("x");
    allUsedVars.insert("y");
    allUsedVars.insert("a");

    std::unordered_set<StmtId> allUsedStmts;
    allUsedStmts.insert(4);
    allUsedStmts.insert(5);

    std::list<std::string> allConstants = {"7", "2"};

    std::string procedureName = "p";

    std::int stmtListCount = 3;
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
      
      Assert::IsTrue(pkb.stmtListTable.size() == stmtListCount);
      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::ASSIGN).size() == stmtCount_ASSIGN);
      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::READ).size() == stmtCount_READ);
      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::PRINT).size() == stmtCount_PRINT);
      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::CALL).size() == stmtCount_CALL);
      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::IF).size() == stmtCount_IF);
      Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::WHILE).size() == stmtCount_WHILE);

      Assert::IsTrue(pkb.followsKB.follows(1, 2));
			Assert::IsTrue(pkb.followsKB.follows(2, 3));
      Assert::IsTrue(pkb.followsKB.follows(3, 6));
      Assert::IsTrue(pkb.followsKB.followStar(1, 2));
      Assert::IsTrue(pkb.followsKB.followStar(1, 3));
      Assert::IsTrue(pkb.followsKB.followStar(1, 6));
      Assert::IsTrue(pkb.followsKB.followStar(2, 3));
      Assert::IsTrue(pkb.followsKB.followStar(2, 6));
      Assert::IsTrue(pkb.followsKB.followStar(3, 6));

      Assert::IsFalse(pkb.followsKB.follows(3, 4));
      Assert::IsFalse(pkb.followsKB.follows(4, 5));
      Assert::IsFalse(pkb.followsKB.follows(5, 6));
      Assert::IsFalse(pkb.followsKB.followStar(1, 4));
      Assert::IsFalse(pkb.followsKB.followStar(1, 5));
      Assert::IsFalse(pkb.followsKB.followStar(2, 4));
      Assert::IsFalse(pkb.followsKB.followStar(2, 5));
      Assert::IsFalse(pkb.followsKB.followStar(3, 4));
      Assert::IsFalse(pkb.followsKB.followStar(3, 5));
      Assert::IsFalse(pkb.followsKB.followStar(4, 5));
      Assert::IsFalse(pkb.followsKB.followStar(4, 6));
      Assert::IsFalse(pkb.followsKB.followStar(5, 6));

      Assert::IsTrue(pkb.parentKB.parent(3, 4));
      Assert::IsTrue(pkb.parentKB.parent(3, 5));
      Assert::IsTrue(pkb.parentKB.parentStar(3, 4));
      Assert::IsTrue(pkb.parentKB.parentStar(3, 5));

      Assert::IsFalse(pkb.parentKB.parent(1, 2));
      Assert::IsFalse(pkb.parentKB.parent(2, 3));
      Assert::IsFalse(pkb.parentKB.parent(5, 6));
      Assert::IsFalse(pkb.parentKB.parentStar(1, 2));
      Assert::IsFalse(pkb.parentKB.parentStar(1, 3));
      Assert::IsFalse(pkb.parentKB.parentStar(1, 4));
      Assert::IsFalse(pkb.parentKB.parentStar(1, 5));
      Assert::IsFalse(pkb.parentKB.parentStar(1, 6));
      Assert::IsFalse(pkb.parentKB.parentStar(2, 3));
      Assert::IsFalse(pkb.parentKB.parentStar(2, 4));
      Assert::IsFalse(pkb.parentKB.parentStar(2, 5));
      Assert::IsFalse(pkb.parentKB.parentStar(2, 6));
      Assert::IsFalse(pkb.parentKB.parentStar(3, 6));
      Assert::IsFalse(pkb.parentKB.parentStar(4, 5));
      Assert::IsFalse(pkb.parentKB.parentStar(4, 6));
      Assert::IsFalse(pkb.parentKB.parentStar(5, 6));


      std::unordered_set<VarId> modifiedVarIds = pkb.modifiesKB.getAllVarsModifiedByProc(pkb.constTable.getProcId(procedureName));
      std::unordered_set<VarName> modifiedVarNames;
      std::unordered_set<StmtId> modifiedStmtIds;
      for (auto itr = modifiedVarIds.begin(); itr != modifiedVarIds.end(); itr++){
        modifiedVarNames.insert(pkb.varTable.get(*itr));
        std::unordered_set<StmtId> modifiedStmts = pkb.modifiesKB.getAllStmtsModifyVar(*itr);
        for (auto itr = modifiedStmts.begin(); itr != modifiedStmts.end(); itr++){
          modifiedStmtIds.insert(*itr);
        }
      }
      CollectionAssert::AreEqual(modifiedVarNames, allModifiedVars);
      CollectionAssert::AreEqual(modifiedStmtIds, allModifiedStmts);

      std::unordered_set<VarId> usedVarIds = pkb.usesKB.getAllVarsUsedByProc(pkb.constTable.getProcId(procedureName));
      std::unordered_set<VarName> usedVarNames;
      std::unordered_set<StmtId> usedStmtIds;
      for (auto itr = usedVarIds.begin(); itr != usedVarIds.end(); itr++){
        usedVarNames.insert(pkb.varTable.get(*itr));
        std::unordered_set<StmtId> usedStmts = pkb.usesKB.getAllStmtsUsingVar(*itr);
        for (auto itr = usedStmts.begin(); itr != usedStmts.end(); itr++){
          usedStmtIds.insert(*itr);
        }
      }
      CollectionAssert::AreEqual(usedVarNames, allUsedVars);
      CollectionAssert::AreEqual(usedStmtIds, allUsedStmts);
		}

	};
}
