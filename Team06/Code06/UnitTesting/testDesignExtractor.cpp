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
			pkb.stmtTable.insertStmt(ReadStmt(1));
			pkb.stmtTable.insertStmt(ReadStmt(2));
			pkb.stmtTable.insertStmt(ReadStmt(3));
			pkb.stmtTable.insertStmt(ReadStmt(4));
			pkb.stmtTable.insertStmt(ReadStmt(5));
			pkb.stmtTable.insertStmt(ReadStmt(6));
			pkb.followsKB.addFollows(1, 2);
			pkb.followsKB.addFollows(3, 4);
			pkb.followsKB.addFollows(4, 5);
			pkb.followsKB.addFollows(2, 6);

			pkb.stmtTable.insertStmt(ReadStmt(7));
			pkb.stmtTable.insertStmt(ReadStmt(8));
			pkb.stmtTable.insertStmt(ReadStmt(9));
			pkb.stmtTable.insertStmt(ReadStmt(10));
			pkb.stmtTable.insertStmt(ReadStmt(11));
			pkb.stmtTable.insertStmt(ReadStmt(12));
			pkb.stmtTable.insertStmt(ReadStmt(13));
			pkb.stmtTable.insertStmt(ReadStmt(14));
			pkb.stmtTable.insertStmt(ReadStmt(15));
			pkb.parentKB.addParent(7, 8);
			pkb.parentKB.addParent(8, 9);
			pkb.parentKB.addParent(8, 10);
			pkb.parentKB.addParent(11, 12);
			pkb.parentKB.addParent(11, 14);
			pkb.parentKB.addParent(12, 13);
			pkb.parentKB.addParent(14, 15);
			pkb = DE.run(pkb);
		}

		TEST_METHOD(populateFollowStar) {
			Assert::IsTrue(pkb.followsKB.followStar(1, 2));
			Assert::IsTrue(pkb.followsKB.followStar(3, 4));
			Assert::IsTrue(pkb.followsKB.followStar(4, 5));
			Assert::IsTrue(pkb.followsKB.followStar(2, 6));
			Assert::IsTrue(pkb.followsKB.followStar(1, 6));
			Assert::IsTrue(pkb.followsKB.followStar(3, 5));
		}

		TEST_METHOD(populateParentStar) {
			Assert::IsTrue(pkb.parentKB.parentStar(7, 8));
			Assert::IsTrue(pkb.parentKB.parentStar(8, 9));
			Assert::IsTrue(pkb.parentKB.parentStar(8, 10));
			Assert::IsTrue(pkb.parentKB.parentStar(7, 9));
			Assert::IsTrue(pkb.parentKB.parentStar(7, 10));

			Assert::IsFalse(pkb.parentKB.parentStar(7, 11));
			Assert::IsFalse(pkb.parentKB.parentStar(8, 11));
			Assert::IsFalse(pkb.parentKB.parentStar(9, 11));
			Assert::IsFalse(pkb.parentKB.parentStar(10, 11));

			Assert::IsTrue(pkb.parentKB.parentStar(11, 12));
			Assert::IsTrue(pkb.parentKB.parentStar(11, 14));
			Assert::IsTrue(pkb.parentKB.parentStar(12, 13));
			Assert::IsTrue(pkb.parentKB.parentStar(14, 15));
			Assert::IsTrue(pkb.parentKB.parentStar(11, 14));
			Assert::IsTrue(pkb.parentKB.parentStar(11, 15));
		}
	};
}