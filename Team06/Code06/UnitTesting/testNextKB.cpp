#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "NextKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    NextKB nKB = NextKB(&PKB::PKB());

    TEST_CLASS(TestNextKB) {
    public:
        std::unordered_set<StmtId> emptyResult;

        TEST_CLASS_INITIALIZE(setup) {
            nKB.addNext(1, 2);
            nKB.addNext(2, 3);
            nKB.addToAll(1, 2, NodeType::SUCCESSOR);
            nKB.addToAll(1, 3, NodeType::SUCCESSOR);
            nKB.addToAll(2, 3, NodeType::SUCCESSOR);
            nKB.addToAll(2, 1, NodeType::PREDECESSOR);
            nKB.addToAll(3, 2, NodeType::PREDECESSOR);
            nKB.addToAll(3, 1, NodeType::PREDECESSOR);
        }

        TEST_METHOD(addNext) {
            Assert::IsTrue(nKB.getDirectNodes(1, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{2});
            Assert::IsTrue(nKB.getDirectNodes(2, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{3});
            Assert::IsTrue(nKB.getDirectNodes(3, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(nKB.getDirectNodes(1, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(nKB.getDirectNodes(2, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1});
            Assert::IsTrue(nKB.getDirectNodes(3, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{2});
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
            Assert::IsFalse(nKB.next(4, -1));
        }

        //TEST_METHOD(nextStar) {
        //    Assert::IsTrue(nKB.nextStar(1, 2));
        //    Assert::IsTrue(nKB.nextStar(1, 3));
        //    Assert::IsTrue(nKB.nextStar(2, 3));

        //    Assert::IsFalse(nKB.nextStar(2, 1));
        //    Assert::IsFalse(nKB.nextStar(3, 1));
        //    Assert::IsFalse(nKB.nextStar(3, 2));

        //    Assert::IsFalse(nKB.nextStar(0, 1));
        //    Assert::IsFalse(nKB.nextStar(1, -1));
        //    Assert::IsFalse(nKB.nextStar(4, -1));
        //}

        TEST_METHOD(getDirectNodes) {
            Assert::IsTrue(nKB.getDirectNodes(0, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(nKB.getDirectNodes(4, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(nKB.getDirectNodes(-1, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(nKB.getDirectNodes(0, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(nKB.getDirectNodes(4, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(nKB.getDirectNodes(-1, NodeType::PREDECESSOR) == emptyResult);
        }

        //TEST_METHOD(getAllNodes) {
        //    Assert::IsTrue(nKB.getAllNodes(0, NodeType::SUCCESSOR) == emptyResult);
        //    Assert::IsTrue(nKB.getAllNodes(4, NodeType::SUCCESSOR) == emptyResult);
        //    Assert::IsTrue(nKB.getAllNodes(-1, NodeType::SUCCESSOR) == emptyResult);

        //    Assert::IsTrue(nKB.getAllNodes(0, NodeType::PREDECESSOR) == emptyResult);
        //    Assert::IsTrue(nKB.getAllNodes(4, NodeType::PREDECESSOR) == emptyResult);
        //    Assert::IsTrue(nKB.getAllNodes(-1, NodeType::PREDECESSOR) == emptyResult);
        //}

        //TEST_METHOD(addToAll_single) {
        //    Assert::IsTrue(nKB.getAllNodes(2, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{3});
        //    Assert::IsTrue(nKB.getAllNodes(2, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1});
        //}

        //TEST_METHOD(addToAll_many) {
        //    Assert::IsTrue(nKB.getAllNodes(1, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{2, 3});
        //    Assert::IsTrue(nKB.getAllNodes(3, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1, 2});
        //}

        TEST_METHOD(hasNext) {
            Assert::IsTrue(nKB.hasNext(1));
            Assert::IsTrue(nKB.hasNext(2));
            Assert::IsFalse(nKB.hasNext(3));

            Assert::IsFalse(nKB.hasNext(0));
            Assert::IsFalse(nKB.hasNext(-1));
            Assert::IsFalse(nKB.hasNext(4));
        }

        TEST_METHOD(hasPrev) {
            Assert::IsFalse(nKB.hasPrev(1));
            Assert::IsTrue(nKB.hasPrev(2));
            Assert::IsTrue(nKB.hasPrev(3));

            Assert::IsFalse(nKB.hasPrev(0));
            Assert::IsFalse(nKB.hasPrev(-1));
            Assert::IsFalse(nKB.hasPrev(4));
        }

        TEST_METHOD(hasNextRelation) {
            Assert::IsTrue(nKB.hasNextRelation());
        }

        //TEST_METHOD(getAllNodes) {
        //    Assert::IsTrue(nKB.getAllNodes() == std::unordered_set<StmtId>{1, 2});
        //}

        //TEST_METHOD(getAllNodes) {
        //    Assert::IsTrue(nKB.getAllNodes() == std::unordered_set<StmtId>{2, 3});
        //}
    };
}
