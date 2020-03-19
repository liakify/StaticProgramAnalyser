#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting
{
    PKB::PKB pkbFollowsParent;
    FrontEnd::DesignExtractor DE_followsParent;

    TEST_CLASS(TestDesignExtractor)
    {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            pkbFollowsParent = PKB::PKB();
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(2)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(3)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(4)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(5)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(6)));

            std::vector<StmtId> v1 = { 1,2,6 };
            pkbFollowsParent.stmtListTable.insertStmtLst(StatementList(v1));
            std::vector<StmtId> v2 = { 3,4,5 };
            pkbFollowsParent.stmtListTable.insertStmtLst(StatementList(v2));


            CondExpr cond = CondExpr(Expression("1", 1, ExprType::CONST), Expression("2", 2, ExprType::CONST));
            std::vector<StmtId> v3 = { 8 };
            pkbFollowsParent.stmtListTable.insertStmtLst(StatementList(v3));
            std::vector<StmtId> v4 = { 9, 10 };
            pkbFollowsParent.stmtListTable.insertStmtLst(StatementList(v4));

            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 3)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 4)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(9)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(10)));
            
            std::vector<StmtId> v5 = { 12 };
            pkbFollowsParent.stmtListTable.insertStmtLst(StatementList(v5));
            std::vector<StmtId> v6 = { 13 };
            pkbFollowsParent.stmtListTable.insertStmtLst(StatementList(v6));
            std::vector<StmtId> v7 = { 14 };
            pkbFollowsParent.stmtListTable.insertStmtLst(StatementList(v7));
            std::vector<StmtId> v8 = { 15 };
            pkbFollowsParent.stmtListTable.insertStmtLst(StatementList(v8));

            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond, 5, 7)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 6)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(13)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 8)));
            pkbFollowsParent.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(15)));

            pkbFollowsParent = DE_followsParent.run(pkbFollowsParent);
        }

        TEST_METHOD(populateFollows) {
            Assert::IsTrue(pkbFollowsParent.followsKB.follows(1, 2));
            Assert::IsTrue(pkbFollowsParent.followsKB.follows(3, 4));
            Assert::IsTrue(pkbFollowsParent.followsKB.follows(4, 5));
            Assert::IsTrue(pkbFollowsParent.followsKB.follows(2, 6));

            Assert::IsFalse(pkbFollowsParent.followsKB.follows(1, 6));
            Assert::IsFalse(pkbFollowsParent.followsKB.follows(3, 5));
        }

        TEST_METHOD(populateFollowStar) {
            Assert::IsTrue(pkbFollowsParent.followsKB.followStar(1, 2));
            Assert::IsTrue(pkbFollowsParent.followsKB.followStar(3, 4));
            Assert::IsTrue(pkbFollowsParent.followsKB.followStar(4, 5));
            Assert::IsTrue(pkbFollowsParent.followsKB.followStar(2, 6));
            Assert::IsTrue(pkbFollowsParent.followsKB.followStar(1, 6));
            Assert::IsTrue(pkbFollowsParent.followsKB.followStar(3, 5));

            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(1, 3));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(1, 4));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(1, 5));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(3, 1));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(4, 1));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(5, 1));

            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(2, 3));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(2, 4));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(2, 5));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(3, 2));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(4, 2));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(5, 2));

            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(3, 6));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(4, 6));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(5, 6));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(6, 3));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(6, 4));
            Assert::IsFalse(pkbFollowsParent.followsKB.followStar(6, 5));
        }

        TEST_METHOD(populateParentStar) {
            Assert::IsTrue(pkbFollowsParent.parentKB.parentStar(7, 8));
            Assert::IsTrue(pkbFollowsParent.parentKB.parentStar(8, 9));
            Assert::IsTrue(pkbFollowsParent.parentKB.parentStar(8, 10));
            Assert::IsTrue(pkbFollowsParent.parentKB.parentStar(7, 9));
            Assert::IsTrue(pkbFollowsParent.parentKB.parentStar(7, 10));

            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(8, 7));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(9, 8));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(10, 8));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(9, 7));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(10, 7));

            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(7, 11));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(8, 11));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(9, 11));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(10, 11));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(7, 12));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(8, 12));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(9, 12));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(10, 12));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(7, 13));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(8, 13));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(9, 13));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(10, 13));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(7, 14));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(8, 14));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(9, 14));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(10, 14));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(7, 15));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(8, 15));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(9, 15));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(10, 15));

            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(11, 7));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(11, 8));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(11, 9));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(11, 10));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(12, 7));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(12, 8));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(12, 9));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(12, 10));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(13, 7));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(13, 8));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(13, 8));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(13, 10));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(14, 7));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(14, 8));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(14, 9));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(14, 10));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(15, 7));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(15, 8));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(15, 9));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(15, 10));

            Assert::IsTrue(pkbFollowsParent.parentKB.parentStar(11, 12));
            Assert::IsTrue(pkbFollowsParent.parentKB.parentStar(11, 14));
            Assert::IsTrue(pkbFollowsParent.parentKB.parentStar(12, 13));
            Assert::IsTrue(pkbFollowsParent.parentKB.parentStar(14, 15));
            Assert::IsTrue(pkbFollowsParent.parentKB.parentStar(11, 14));
            Assert::IsTrue(pkbFollowsParent.parentKB.parentStar(11, 15));

            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(12, 11));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(14, 11));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(13, 12));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(15, 14));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(14, 11));
            Assert::IsFalse(pkbFollowsParent.parentKB.parentStar(15, 11));
        }
    };
}