#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting
{
	PKB::PKB pkb;
	FrontEnd::DesignExtractor DE;

	TEST_CLASS(TestDesignExtractor)
	{
	public:
		TEST_CLASS_INITIALIZE(setup) {
			pkb = PKB::PKB();
			pkb.stmtTable.insertStmt(&ReadStmt(1));
			pkb.stmtTable.insertStmt(&ReadStmt(2));
			pkb.stmtTable.insertStmt(&ReadStmt(3));
			pkb.stmtTable.insertStmt(&ReadStmt(4));
			pkb.stmtTable.insertStmt(&ReadStmt(5));
			pkb.stmtTable.insertStmt(&ReadStmt(6));

			std::vector<StmtId> v1 = { 1,2,6 };
			pkb.stmtListTable.insertStmtLst(StatementList(v1));
			std::vector<StmtId> v2 = { 3,4,5 };
			pkb.stmtListTable.insertStmtLst(StatementList(v2));

			pkb.stmtTable.insertStmt(&ReadStmt(7));
			pkb.stmtTable.insertStmt(&ReadStmt(8));
			pkb.stmtTable.insertStmt(&ReadStmt(9));
			pkb.stmtTable.insertStmt(&ReadStmt(10));
			pkb.stmtTable.insertStmt(&ReadStmt(11));
			pkb.stmtTable.insertStmt(&ReadStmt(12));
			pkb.stmtTable.insertStmt(&ReadStmt(13));
			pkb.stmtTable.insertStmt(&ReadStmt(14));
			pkb.stmtTable.insertStmt(&ReadStmt(15));
			pkb.parentKB.addParent(7, 8);
			pkb.parentKB.addParent(8, 9);
			pkb.parentKB.addParent(8, 10);
			pkb.parentKB.addParent(11, 12);
			pkb.parentKB.addParent(11, 14);
			pkb.parentKB.addParent(12, 13);
			pkb.parentKB.addParent(14, 15);
			pkb = DE.run(pkb);
		}

		TEST_METHOD(populateFollows) {
			Assert::IsTrue(pkb.followsKB.follows(1, 2));
			Assert::IsTrue(pkb.followsKB.follows(3, 4));
			Assert::IsTrue(pkb.followsKB.follows(4, 5));
			Assert::IsTrue(pkb.followsKB.follows(2, 6));

			Assert::IsFalse(pkb.followsKB.follows(1, 6));
			Assert::IsFalse(pkb.followsKB.follows(3, 5));
		}

		TEST_METHOD(populateFollowStar) {
			Assert::IsTrue(pkb.followsKB.followStar(1, 2));
			Assert::IsTrue(pkb.followsKB.followStar(3, 4));
			Assert::IsTrue(pkb.followsKB.followStar(4, 5));
			Assert::IsTrue(pkb.followsKB.followStar(2, 6));
			Assert::IsTrue(pkb.followsKB.followStar(1, 6));
			Assert::IsTrue(pkb.followsKB.followStar(3, 5));

			Assert::IsFalse(pkb.followsKB.followStar(1, 3));
			Assert::IsFalse(pkb.followsKB.followStar(1, 4));
			Assert::IsFalse(pkb.followsKB.followStar(1, 5));
			Assert::IsFalse(pkb.followsKB.followStar(3, 1));
			Assert::IsFalse(pkb.followsKB.followStar(4, 1));
			Assert::IsFalse(pkb.followsKB.followStar(5, 1));

			Assert::IsFalse(pkb.followsKB.followStar(2, 3));
			Assert::IsFalse(pkb.followsKB.followStar(2, 4));
			Assert::IsFalse(pkb.followsKB.followStar(2, 5));
			Assert::IsFalse(pkb.followsKB.followStar(3, 2));
			Assert::IsFalse(pkb.followsKB.followStar(4, 2));
			Assert::IsFalse(pkb.followsKB.followStar(5, 2));

			Assert::IsFalse(pkb.followsKB.followStar(3, 6));
			Assert::IsFalse(pkb.followsKB.followStar(4, 6));
			Assert::IsFalse(pkb.followsKB.followStar(5, 6));
			Assert::IsFalse(pkb.followsKB.followStar(6, 3));
			Assert::IsFalse(pkb.followsKB.followStar(6, 4));
			Assert::IsFalse(pkb.followsKB.followStar(6, 5));
		}

		TEST_METHOD(populateParentStar) {
			Assert::IsTrue(pkb.parentKB.parentStar(7, 8));
			Assert::IsTrue(pkb.parentKB.parentStar(8, 9));
			Assert::IsTrue(pkb.parentKB.parentStar(8, 10));
			Assert::IsTrue(pkb.parentKB.parentStar(7, 9));
			Assert::IsTrue(pkb.parentKB.parentStar(7, 10));

			Assert::IsFalse(pkb.parentKB.parentStar(8, 7));
			Assert::IsFalse(pkb.parentKB.parentStar(9, 8));
			Assert::IsFalse(pkb.parentKB.parentStar(10, 8));
			Assert::IsFalse(pkb.parentKB.parentStar(9, 7));
			Assert::IsFalse(pkb.parentKB.parentStar(10, 7));

			Assert::IsFalse(pkb.parentKB.parentStar(7, 11));
			Assert::IsFalse(pkb.parentKB.parentStar(8, 11));
			Assert::IsFalse(pkb.parentKB.parentStar(9, 11));
			Assert::IsFalse(pkb.parentKB.parentStar(10, 11));
			Assert::IsFalse(pkb.parentKB.parentStar(7, 12));
			Assert::IsFalse(pkb.parentKB.parentStar(8, 12));
			Assert::IsFalse(pkb.parentKB.parentStar(9, 12));
			Assert::IsFalse(pkb.parentKB.parentStar(10, 12));
			Assert::IsFalse(pkb.parentKB.parentStar(7, 13));
			Assert::IsFalse(pkb.parentKB.parentStar(8, 13));
			Assert::IsFalse(pkb.parentKB.parentStar(9, 13));
			Assert::IsFalse(pkb.parentKB.parentStar(10, 13));
			Assert::IsFalse(pkb.parentKB.parentStar(7, 14));
			Assert::IsFalse(pkb.parentKB.parentStar(8, 14));
			Assert::IsFalse(pkb.parentKB.parentStar(9, 14));
			Assert::IsFalse(pkb.parentKB.parentStar(10, 14));
			Assert::IsFalse(pkb.parentKB.parentStar(7, 15));
			Assert::IsFalse(pkb.parentKB.parentStar(8, 15));
			Assert::IsFalse(pkb.parentKB.parentStar(9, 15));
			Assert::IsFalse(pkb.parentKB.parentStar(10, 15));

			Assert::IsFalse(pkb.parentKB.parentStar(11, 7));
			Assert::IsFalse(pkb.parentKB.parentStar(11, 8));
			Assert::IsFalse(pkb.parentKB.parentStar(11, 9));
			Assert::IsFalse(pkb.parentKB.parentStar(11, 10));
			Assert::IsFalse(pkb.parentKB.parentStar(12, 7));
			Assert::IsFalse(pkb.parentKB.parentStar(12, 8));
			Assert::IsFalse(pkb.parentKB.parentStar(12, 9));
			Assert::IsFalse(pkb.parentKB.parentStar(12, 10));
			Assert::IsFalse(pkb.parentKB.parentStar(13, 7));
			Assert::IsFalse(pkb.parentKB.parentStar(13, 8));
			Assert::IsFalse(pkb.parentKB.parentStar(13, 8));
			Assert::IsFalse(pkb.parentKB.parentStar(13, 10));
			Assert::IsFalse(pkb.parentKB.parentStar(14, 7));
			Assert::IsFalse(pkb.parentKB.parentStar(14, 8));
			Assert::IsFalse(pkb.parentKB.parentStar(14, 9));
			Assert::IsFalse(pkb.parentKB.parentStar(14, 10));
			Assert::IsFalse(pkb.parentKB.parentStar(15, 7));
			Assert::IsFalse(pkb.parentKB.parentStar(15, 8));
			Assert::IsFalse(pkb.parentKB.parentStar(15, 9));
			Assert::IsFalse(pkb.parentKB.parentStar(15, 10));

			Assert::IsTrue(pkb.parentKB.parentStar(11, 12));
			Assert::IsTrue(pkb.parentKB.parentStar(11, 14));
			Assert::IsTrue(pkb.parentKB.parentStar(12, 13));
			Assert::IsTrue(pkb.parentKB.parentStar(14, 15));
			Assert::IsTrue(pkb.parentKB.parentStar(11, 14));
			Assert::IsTrue(pkb.parentKB.parentStar(11, 15));

			Assert::IsFalse(pkb.parentKB.parentStar(12, 11));
			Assert::IsFalse(pkb.parentKB.parentStar(14, 11));
			Assert::IsFalse(pkb.parentKB.parentStar(13, 12));
			Assert::IsFalse(pkb.parentKB.parentStar(15, 14));
			Assert::IsFalse(pkb.parentKB.parentStar(14, 11));
			Assert::IsFalse(pkb.parentKB.parentStar(15, 11));
		}
	};
}