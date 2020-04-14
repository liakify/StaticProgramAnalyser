#include "stdafx.h"

#include "CppUnitTest.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting {
    PKB::PKB nKB;
    PKB::PKB nkbEmpty;

    TEST_CLASS(TestNextKB) {
    public:
        std::unordered_set<StmtId> emptyResult;

        TEST_CLASS_INITIALIZE(setup) {
            nKB.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));
            nKB.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(2)));
            nKB.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(3)));
            nKB.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(4)));

            nKB.addNext(1, 2);
            nKB.addNext(2, 3);
            nKB.nextStarAddToAll(1, 2, NodeType::SUCCESSOR);
            nKB.nextStarAddToAll(1, 3, NodeType::SUCCESSOR);
            nKB.nextStarAddToAll(2, 3, NodeType::SUCCESSOR);
            nKB.nextStarAddToAll(2, 1, NodeType::PREDECESSOR);
            nKB.nextStarAddToAll(3, 2, NodeType::PREDECESSOR);
            nKB.nextStarAddToAll(3, 1, NodeType::PREDECESSOR);
            nKB.nextStarSetProcessedAll(1, NodeType::SUCCESSOR);
            nKB.nextStarSetProcessedAll(2, NodeType::SUCCESSOR);
            nKB.nextStarSetProcessedAll(3, NodeType::SUCCESSOR);
            nKB.nextStarSetProcessedAll(3, NodeType::PREDECESSOR);
            nKB.nextStarSetProcessedAll(2, NodeType::PREDECESSOR);
            nKB.nextStarSetProcessedAll(1, NodeType::PREDECESSOR);
        }

        TEST_METHOD(addNext) {
            Assert::IsTrue(nKB.nextGetDirectNodes(1, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{2});
            Assert::IsTrue(nKB.nextGetDirectNodes(2, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{3});
            Assert::IsTrue(nKB.nextGetDirectNodes(3, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(nKB.nextGetDirectNodes(1, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(nKB.nextGetDirectNodes(2, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1});
            Assert::IsTrue(nKB.nextGetDirectNodes(3, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{2});
        }

        TEST_METHOD(next) {
            Assert::IsTrue(nKB.next(1, 2));
            Assert::IsTrue(nKB.next(2, 3));
            Assert::IsFalse(nKB.next(1, 3));

            Assert::IsFalse(nKB.next(2, 1));
            Assert::IsFalse(nKB.next(3, 1));
            Assert::IsFalse(nKB.next(3, 2));

            Assert::IsFalse(nKB.next(0, 1));
            Assert::IsFalse(nKB.next(1, -1));
            Assert::IsFalse(nKB.next(5, -1));
        }

        TEST_METHOD(nextStar) {
            Assert::IsTrue(nKB.nextStar(1, 2));
            Assert::IsTrue(nKB.nextStar(1, 3));
            Assert::IsTrue(nKB.nextStar(2, 3));

            Assert::IsFalse(nKB.nextStar(2, 1));
            Assert::IsFalse(nKB.nextStar(3, 1));
            Assert::IsFalse(nKB.nextStar(3, 2));

            Assert::IsFalse(nKB.nextStar(0, 1));
            Assert::IsFalse(nKB.nextStar(1, -1));
            Assert::IsFalse(nKB.nextStar(5, -1));
        }

        TEST_METHOD(nextGetDirectNodes) {
            Assert::IsTrue(nKB.nextGetDirectNodes(0, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(nKB.nextGetDirectNodes(5, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(nKB.nextGetDirectNodes(-1, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(nKB.nextGetDirectNodes(0, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(nKB.nextGetDirectNodes(5, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(nKB.nextGetDirectNodes(-1, NodeType::PREDECESSOR) == emptyResult);
        }

        TEST_METHOD(nextStarGetAllNodes) {
            Assert::IsTrue(nKB.nextStarGetAllNodes(0, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(nKB.nextStarGetAllNodes(5, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(nKB.nextStarGetAllNodes(-1, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(nKB.nextStarGetAllNodes(0, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(nKB.nextStarGetAllNodes(5, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(nKB.nextStarGetAllNodes(-1, NodeType::PREDECESSOR) == emptyResult);
        }

        TEST_METHOD(nextStarAddToAll) {
            Assert::IsTrue(nKB.nextStarGetAllNodes(1, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{2, 3});
            Assert::IsTrue(nKB.nextStarGetAllNodes(1, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{});

            Assert::IsTrue(nKB.nextStarGetAllNodes(2, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{3});
            Assert::IsTrue(nKB.nextStarGetAllNodes(2, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1});

            Assert::IsTrue(nKB.nextStarGetAllNodes(3, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{});
            Assert::IsTrue(nKB.nextStarGetAllNodes(3, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1, 2});
        }

        TEST_METHOD(hasNext) {
            Assert::IsTrue(nKB.hasNext(1));
            Assert::IsTrue(nKB.hasNext(2));
            Assert::IsFalse(nKB.hasNext(3));

            Assert::IsFalse(nKB.hasNext(0));
            Assert::IsFalse(nKB.hasNext(-1));
            Assert::IsFalse(nKB.hasNext(5));
        }

        TEST_METHOD(hasPrev) {
            Assert::IsFalse(nKB.hasPrev(1));
            Assert::IsTrue(nKB.hasPrev(2));
            Assert::IsTrue(nKB.hasPrev(3));

            Assert::IsFalse(nKB.hasPrev(0));
            Assert::IsFalse(nKB.hasPrev(-1));
            Assert::IsFalse(nKB.hasPrev(5));
        }

        TEST_METHOD(hasNextRelation) {
            Assert::IsTrue(nKB.hasNextRelation());
            Assert::IsFalse(nkbEmpty.hasNextRelation());
        }

        TEST_METHOD(nextStarSetProcessedAll) {
            Assert::IsTrue(nKB.nextStarProcessedAll(1, NodeType::SUCCESSOR));
            Assert::IsTrue(nKB.nextStarProcessedAll(2, NodeType::SUCCESSOR));
            Assert::IsTrue(nKB.nextStarProcessedAll(3, NodeType::SUCCESSOR));

            Assert::IsTrue(nKB.nextStarProcessedAll(1, NodeType::PREDECESSOR));
            Assert::IsTrue(nKB.nextStarProcessedAll(2, NodeType::PREDECESSOR));
            Assert::IsTrue(nKB.nextStarProcessedAll(3, NodeType::PREDECESSOR));

            Assert::IsFalse(nKB.nextStarProcessedAll(4, NodeType::SUCCESSOR));
            Assert::IsFalse(nKB.nextStarProcessedAll(4, NodeType::PREDECESSOR));
        }
    };
}
