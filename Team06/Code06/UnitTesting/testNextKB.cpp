#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

            pkb.nextKB.addNext(1, 2);
            pkb.nextKB.addNext(2, 3);
            pkb.nextKB.addToAll(1, 2, NodeType::SUCCESSOR);
            pkb.nextKB.addToAll(1, 3, NodeType::SUCCESSOR);
            pkb.nextKB.addToAll(2, 3, NodeType::SUCCESSOR);
            pkb.nextKB.addToAll(2, 1, NodeType::PREDECESSOR);
            pkb.nextKB.addToAll(3, 2, NodeType::PREDECESSOR);
            pkb.nextKB.addToAll(3, 1, NodeType::PREDECESSOR);
            pkb.nextKB.setProcessedAll(1, NodeType::SUCCESSOR);
            pkb.nextKB.setProcessedAll(2, NodeType::SUCCESSOR);
            pkb.nextKB.setProcessedAll(3, NodeType::SUCCESSOR);
            pkb.nextKB.setProcessedAll(3, NodeType::PREDECESSOR);
            pkb.nextKB.setProcessedAll(2, NodeType::PREDECESSOR);
            pkb.nextKB.setProcessedAll(1, NodeType::PREDECESSOR);
        }

        TEST_METHOD(addNext) {
            Assert::IsTrue(pkb.nextKB.getDirectNodes(1, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{2});
            Assert::IsTrue(pkb.nextKB.getDirectNodes(2, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{3});
            Assert::IsTrue(pkb.nextKB.getDirectNodes(3, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(pkb.nextKB.getDirectNodes(1, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextKB.getDirectNodes(2, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1});
            Assert::IsTrue(pkb.nextKB.getDirectNodes(3, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{2});
        }

        TEST_METHOD(next) {
            Assert::IsTrue(pkb.nextKB.next(1, 2));
            Assert::IsTrue(pkb.nextKB.next(2, 3));
            Assert::IsFalse(pkb.nextKB.next(1, 3));

            Assert::IsFalse(pkb.nextKB.next(2, 1));
            Assert::IsFalse(pkb.nextKB.next(3, 1));
            Assert::IsFalse(pkb.nextKB.next(3, 2));

            Assert::IsFalse(pkb.nextKB.next(0, 1));
            Assert::IsFalse(pkb.nextKB.next(1, -1));
            Assert::IsFalse(pkb.nextKB.next(5, -1));
        }

        TEST_METHOD(nextStar) {
            Assert::IsTrue(pkb.nextKB.nextStar(1, 2));
            Assert::IsTrue(pkb.nextKB.nextStar(1, 3));
            Assert::IsTrue(pkb.nextKB.nextStar(2, 3));

            Assert::IsFalse(pkb.nextKB.nextStar(2, 1));
            Assert::IsFalse(pkb.nextKB.nextStar(3, 1));
            Assert::IsFalse(pkb.nextKB.nextStar(3, 2));

            Assert::IsFalse(pkb.nextKB.nextStar(0, 1));
            Assert::IsFalse(pkb.nextKB.nextStar(1, -1));
            Assert::IsFalse(pkb.nextKB.nextStar(5, -1));
        }

        TEST_METHOD(getDirectNodes) {
            Assert::IsTrue(pkb.nextKB.getDirectNodes(0, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextKB.getDirectNodes(5, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextKB.getDirectNodes(-1, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(pkb.nextKB.getDirectNodes(0, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextKB.getDirectNodes(5, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextKB.getDirectNodes(-1, NodeType::PREDECESSOR) == emptyResult);
        }

        TEST_METHOD(getAllNodes) {
            Assert::IsTrue(pkb.nextKB.getAllNodes(0, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextKB.getAllNodes(5, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextKB.getAllNodes(-1, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(pkb.nextKB.getAllNodes(0, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextKB.getAllNodes(5, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(pkb.nextKB.getAllNodes(-1, NodeType::PREDECESSOR) == emptyResult);
        }

        TEST_METHOD(addToAll) {
            Assert::IsTrue(pkb.nextKB.getAllNodes(1, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{2, 3});
            Assert::IsTrue(pkb.nextKB.getAllNodes(1, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{});

            Assert::IsTrue(pkb.nextKB.getAllNodes(2, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{3});
            Assert::IsTrue(pkb.nextKB.getAllNodes(2, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1});

            Assert::IsTrue(pkb.nextKB.getAllNodes(3, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{});
            Assert::IsTrue(pkb.nextKB.getAllNodes(3, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1, 2});
        }

        TEST_METHOD(hasNext) {
            Assert::IsTrue(pkb.nextKB.hasNext(1));
            Assert::IsTrue(pkb.nextKB.hasNext(2));
            Assert::IsFalse(pkb.nextKB.hasNext(3));

            Assert::IsFalse(pkb.nextKB.hasNext(0));
            Assert::IsFalse(pkb.nextKB.hasNext(-1));
            Assert::IsFalse(pkb.nextKB.hasNext(5));
        }

        TEST_METHOD(hasPrev) {
            Assert::IsFalse(pkb.nextKB.hasPrev(1));
            Assert::IsTrue(pkb.nextKB.hasPrev(2));
            Assert::IsTrue(pkb.nextKB.hasPrev(3));

            Assert::IsFalse(pkb.nextKB.hasPrev(0));
            Assert::IsFalse(pkb.nextKB.hasPrev(-1));
            Assert::IsFalse(pkb.nextKB.hasPrev(5));
        }

        TEST_METHOD(hasNextRelation) {
            Assert::IsTrue(pkb.nextKB.hasNextRelation());
        }

        TEST_METHOD(setProcessedAll) {
            Assert::IsTrue(pkb.nextKB.processedAll(1, NodeType::SUCCESSOR));
            Assert::IsTrue(pkb.nextKB.processedAll(2, NodeType::SUCCESSOR));
            Assert::IsTrue(pkb.nextKB.processedAll(3, NodeType::SUCCESSOR));

            Assert::IsTrue(pkb.nextKB.processedAll(1, NodeType::PREDECESSOR));
            Assert::IsTrue(pkb.nextKB.processedAll(2, NodeType::PREDECESSOR));
            Assert::IsTrue(pkb.nextKB.processedAll(3, NodeType::PREDECESSOR));

            Assert::IsFalse(pkb.nextKB.processedAll(4, NodeType::SUCCESSOR));
            Assert::IsFalse(pkb.nextKB.processedAll(4, NodeType::PREDECESSOR));
        }
    };
}
