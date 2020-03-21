#include "stdafx.h"

#include "CppUnitTest.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting {
    PKB::PKB pkb;

    TEST_CLASS(TestNextKB) {
    public:
        std::unordered_set<StmtId> emptyResult;

        TEST_CLASS_INITIALIZE(setup) {
            pkb.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));
            pkb.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(2)));
            pkb.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(3)));
            pkb.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(4)));

            pkb.addNext(1, 2);
            pkb.addNext(2, 3);
            pkb.nextStarAddToAll(1, 2, NodeType::SUCCESSOR);
            pkb.nextStarAddToAll(1, 3, NodeType::SUCCESSOR);
            pkb.nextStarAddToAll(2, 3, NodeType::SUCCESSOR);
            pkb.nextStarAddToAll(2, 1, NodeType::PREDECESSOR);
            pkb.nextStarAddToAll(3, 2, NodeType::PREDECESSOR);
            pkb.nextStarAddToAll(3, 1, NodeType::PREDECESSOR);
            pkb.nextStarSetProcessedAll(1, NodeType::SUCCESSOR);
            pkb.nextStarSetProcessedAll(2, NodeType::SUCCESSOR);
            pkb.nextStarSetProcessedAll(3, NodeType::SUCCESSOR);
            pkb.nextStarSetProcessedAll(3, NodeType::PREDECESSOR);
            pkb.nextStarSetProcessedAll(2, NodeType::PREDECESSOR);
            pkb.nextStarSetProcessedAll(1, NodeType::PREDECESSOR);
        }

        TEST_METHOD(addNext) {
            Assert::IsTrue(pkb.nextStarGetDirectNodes(1, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{2});
            Assert::IsTrue(pkb.nextStarGetDirectNodes(2, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{3});
            Assert::IsTrue(pkb.nextStarGetDirectNodes(3, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(pkb.nextStarGetDirectNodes(1, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextStarGetDirectNodes(2, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1});
            Assert::IsTrue(pkb.nextStarGetDirectNodes(3, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{2});
        }

        TEST_METHOD(next) {
            Assert::IsTrue(pkb.next(1, 2));
            Assert::IsTrue(pkb.next(2, 3));
            Assert::IsFalse(pkb.next(1, 3));

            Assert::IsFalse(pkb.next(2, 1));
            Assert::IsFalse(pkb.next(3, 1));
            Assert::IsFalse(pkb.next(3, 2));

            Assert::IsFalse(pkb.next(0, 1));
            Assert::IsFalse(pkb.next(1, -1));
            Assert::IsFalse(pkb.next(5, -1));
        }

        TEST_METHOD(nextStar) {
            Assert::IsTrue(pkb.nextStar(1, 2));
            Assert::IsTrue(pkb.nextStar(1, 3));
            Assert::IsTrue(pkb.nextStar(2, 3));

            Assert::IsFalse(pkb.nextStar(2, 1));
            Assert::IsFalse(pkb.nextStar(3, 1));
            Assert::IsFalse(pkb.nextStar(3, 2));

            Assert::IsFalse(pkb.nextStar(0, 1));
            Assert::IsFalse(pkb.nextStar(1, -1));
            Assert::IsFalse(pkb.nextStar(5, -1));
        }

        TEST_METHOD(nextStarGetDirectNodes) {
            Assert::IsTrue(pkb.nextStarGetDirectNodes(0, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextStarGetDirectNodes(5, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextStarGetDirectNodes(-1, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(pkb.nextStarGetDirectNodes(0, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextStarGetDirectNodes(5, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextStarGetDirectNodes(-1, NodeType::PREDECESSOR) == emptyResult);
        }

        TEST_METHOD(nextStarGetAllNodes) {
            Assert::IsTrue(pkb.nextStarGetAllNodes(0, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextStarGetAllNodes(5, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextStarGetAllNodes(-1, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(pkb.nextStarGetAllNodes(0, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextStarGetAllNodes(5, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextStarGetAllNodes(-1, NodeType::PREDECESSOR) == emptyResult);
        }

        TEST_METHOD(nextStarAddToAll) {
            Assert::IsTrue(pkb.nextStarGetAllNodes(1, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{2, 3});
            Assert::IsTrue(pkb.nextStarGetAllNodes(1, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{});

            Assert::IsTrue(pkb.nextStarGetAllNodes(2, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{3});
            Assert::IsTrue(pkb.nextStarGetAllNodes(2, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1});

            Assert::IsTrue(pkb.nextStarGetAllNodes(3, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{});
            Assert::IsTrue(pkb.nextStarGetAllNodes(3, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1, 2});
        }

        TEST_METHOD(hasNext) {
            Assert::IsTrue(pkb.hasNext(1));
            Assert::IsTrue(pkb.hasNext(2));
            Assert::IsFalse(pkb.hasNext(3));

            Assert::IsFalse(pkb.hasNext(0));
            Assert::IsFalse(pkb.hasNext(-1));
            Assert::IsFalse(pkb.hasNext(5));
        }

        TEST_METHOD(hasPrev) {
            Assert::IsFalse(pkb.hasPrev(1));
            Assert::IsTrue(pkb.hasPrev(2));
            Assert::IsTrue(pkb.hasPrev(3));

            Assert::IsFalse(pkb.hasPrev(0));
            Assert::IsFalse(pkb.hasPrev(-1));
            Assert::IsFalse(pkb.hasPrev(5));
        }

        TEST_METHOD(hasNextRelation) {
            Assert::IsTrue(pkb.hasNextRelation());
        }

        TEST_METHOD(nextStarSetProcessedAll) {
            Assert::IsTrue(pkb.nextStarProcessedAll(1, NodeType::SUCCESSOR));
            Assert::IsTrue(pkb.nextStarProcessedAll(2, NodeType::SUCCESSOR));
            Assert::IsTrue(pkb.nextStarProcessedAll(3, NodeType::SUCCESSOR));

            Assert::IsTrue(pkb.nextStarProcessedAll(1, NodeType::PREDECESSOR));
            Assert::IsTrue(pkb.nextStarProcessedAll(2, NodeType::PREDECESSOR));
            Assert::IsTrue(pkb.nextStarProcessedAll(3, NodeType::PREDECESSOR));

            Assert::IsFalse(pkb.nextStarProcessedAll(4, NodeType::SUCCESSOR));
            Assert::IsFalse(pkb.nextStarProcessedAll(4, NodeType::PREDECESSOR));
        }
    };
}
