#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting
{
	PKB::PKB pkbNext;
	FrontEnd::DesignExtractor DE_next;

	TEST_CLASS(TestDesignExtractor_next)
	{
	public:
		TEST_CLASS_INITIALIZE(setup) {
			/*
			procedure a {
				call b;
			}

			procedure b {
				read;
				while (cond) {
					if (cond) then {
						read;
					} else {
						read;
					}
				}
				read;
			}
			*/
			pkbNext = PKB::PKB();
			pkbNext.procTable.insertProc(Procedure("a", 1));
			pkbNext.procTable.insertProc(Procedure("b", 2));

			StatementList sl1 = StatementList(std::vector<StmtId>{1});
			pkbNext.stmtListTable.insertStmtLst(sl1);
			StatementList sl2 = StatementList(std::vector<StmtId>{2, 3, 7});
			pkbNext.stmtListTable.insertStmtLst(sl2);
			StatementList sl3 = StatementList(std::vector<StmtId>{4});
			pkbNext.stmtListTable.insertStmtLst(sl3);
			StatementList sl4 = StatementList(std::vector<StmtId>{5});
			pkbNext.stmtListTable.insertStmtLst(sl4);
			StatementList sl5 = StatementList(std::vector<StmtId>{6});
			pkbNext.stmtListTable.insertStmtLst(sl5);

			CondExpr cond = CondExpr(Expression("1", 1, ExprType::CONST), Expression("2", 2, ExprType::CONST));

			pkbNext.stmtTable.insertStmt(new CallStmt("b"));
			pkbNext.stmtTable.insertStmt(new ReadStmt(2));
			pkbNext.stmtTable.insertStmt(new WhileStmt(cond, 3));
			pkbNext.stmtTable.insertStmt(new IfStmt(cond, 4, 5));
			pkbNext.stmtTable.insertStmt(new ReadStmt(5));
			pkbNext.stmtTable.insertStmt(new ReadStmt(6));
			pkbNext.stmtTable.insertStmt(new ReadStmt(7));

			pkbNext = DE_next.run(pkbNext);
			int x;
		}

		TEST_METHOD(updateLastStmtId) {
			Assert::IsTrue(pkbNext.stmtListTable.get(1).getFirst() == 1);
			Assert::IsTrue(pkbNext.stmtListTable.get(1).getLast() == 1);
			Assert::IsTrue(pkbNext.stmtListTable.get(2).getFirst() == 2);
			Assert::IsTrue(pkbNext.stmtListTable.get(2).getLast() == 7);
			Assert::IsTrue(pkbNext.stmtListTable.get(3).getFirst() == 4);
			Assert::IsTrue(pkbNext.stmtListTable.get(3).getLast() == 6);
			Assert::IsTrue(pkbNext.stmtListTable.get(4).getFirst() == 5);
			Assert::IsTrue(pkbNext.stmtListTable.get(4).getLast() == 5);
			Assert::IsTrue(pkbNext.stmtListTable.get(5).getFirst() == 6);
			Assert::IsTrue(pkbNext.stmtListTable.get(5).getLast() == 6);
		}

		TEST_METHOD(updateStmtListId) {
			Assert::IsTrue(pkbNext.stmtTable.get(1)->getParentId() == 1);
			Assert::IsTrue(pkbNext.stmtTable.get(2)->getParentId() == 2);
			Assert::IsTrue(pkbNext.stmtTable.get(3)->getParentId() == 2);
			Assert::IsTrue(pkbNext.stmtTable.get(4)->getParentId() == 3);
			Assert::IsTrue(pkbNext.stmtTable.get(5)->getParentId() == 4);
			Assert::IsTrue(pkbNext.stmtTable.get(6)->getParentId() == 5);
			Assert::IsTrue(pkbNext.stmtTable.get(7)->getParentId() == 2);
		}
	};
}
