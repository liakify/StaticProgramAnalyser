#include "stdafx.h"
#include "CppUnitTest.h"
#include "CallsKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    PKB::CallsKB cKB;
    PKB::CallsKB ckbEmpty;

    TEST_CLASS(TestCallsKB)
    {
    public:
        std::unordered_set<ProcId> emptyResult;
        
        TEST_CLASS_INITIALIZE(setup) {
            cKB.addCalls(1, 2);
            cKB.addCalls(2, 3);
            cKB.addToAll(1, std::unordered_set<ProcId>{2, 3}, NodeType::SUCCESSOR);
            cKB.addToAll(2, 3, NodeType::SUCCESSOR);
            cKB.addToAll(2, 1, NodeType::PREDECESSOR);
            cKB.addToAll(3, std::unordered_set<ProcId>{1, 2}, NodeType::PREDECESSOR);
        }

        TEST_METHOD(addCalls) {
            Assert::IsTrue(cKB.getDirectNodes(1, NodeType::SUCCESSOR) == std::unordered_set<ProcId>{2});
            Assert::IsTrue(cKB.getDirectNodes(2, NodeType::SUCCESSOR) == std::unordered_set<ProcId>{3});
            Assert::IsTrue(cKB.getDirectNodes(3, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(cKB.getDirectNodes(1, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(cKB.getDirectNodes(2, NodeType::PREDECESSOR) == std::unordered_set<ProcId>{1});
            Assert::IsTrue(cKB.getDirectNodes(3, NodeType::PREDECESSOR) == std::unordered_set<ProcId>{2});
        }

        TEST_METHOD(calls) {
            Assert::IsTrue(cKB.calls(1, 2));
            Assert::IsTrue(cKB.calls(2, 3));
            Assert::IsFalse(cKB.calls(1, 3));

            Assert::IsFalse(cKB.calls(2, 1));
            Assert::IsFalse(cKB.calls(3, 1));
            Assert::IsFalse(cKB.calls(3, 2));

            Assert::IsFalse(cKB.calls(0, 1));
            Assert::IsFalse(cKB.calls(1, -1));
            Assert::IsFalse(cKB.calls(4, -1));
        }

        TEST_METHOD(callStar) {
            Assert::IsTrue(cKB.callStar(1, 2));
            Assert::IsTrue(cKB.callStar(1, 3));
            Assert::IsTrue(cKB.callStar(2, 3));

            Assert::IsFalse(cKB.callStar(2, 1));
            Assert::IsFalse(cKB.callStar(3, 1));
            Assert::IsFalse(cKB.callStar(3, 2));

            Assert::IsFalse(cKB.callStar(0, 1));
            Assert::IsFalse(cKB.callStar(1, -1));
            Assert::IsFalse(cKB.callStar(4, -1));
        }

        TEST_METHOD(getDirectNodes) {
            Assert::IsTrue(cKB.getDirectNodes(0, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(cKB.getDirectNodes(4, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(cKB.getDirectNodes(-1, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(cKB.getDirectNodes(0, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(cKB.getDirectNodes(4, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(cKB.getDirectNodes(-1, NodeType::PREDECESSOR) == emptyResult);
        }

        TEST_METHOD(getAllNodes) {
            Assert::IsTrue(cKB.getAllNodes(0, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(cKB.getAllNodes(4, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(cKB.getAllNodes(-1, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(cKB.getAllNodes(0, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(cKB.getAllNodes(4, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(cKB.getAllNodes(-1, NodeType::PREDECESSOR) == emptyResult);
        }

        TEST_METHOD(addToAll_single) {
            Assert::IsTrue(cKB.getAllNodes(2, NodeType::SUCCESSOR) == std::unordered_set<ProcId>{3});
            Assert::IsTrue(cKB.getAllNodes(2, NodeType::PREDECESSOR) == std::unordered_set<ProcId>{1});
        }

        TEST_METHOD(addToAll_many) {
            Assert::IsTrue(cKB.getAllNodes(1, NodeType::SUCCESSOR) == std::unordered_set<ProcId>{2, 3});
            Assert::IsTrue(cKB.getAllNodes(3, NodeType::PREDECESSOR) == std::unordered_set<ProcId>{1, 2});
        }

        TEST_METHOD(hasCallee) {
            Assert::IsTrue(cKB.hasCallee(1));
            Assert::IsTrue(cKB.hasCallee(2));
            Assert::IsFalse(cKB.hasCallee(3));

            Assert::IsFalse(cKB.hasCallee(0));
            Assert::IsFalse(cKB.hasCallee(-1));
            Assert::IsFalse(cKB.hasCallee(4));
        }

        TEST_METHOD(hasCaller) {
            Assert::IsFalse(cKB.hasCaller(1));
            Assert::IsTrue(cKB.hasCaller(2));
            Assert::IsTrue(cKB.hasCaller(3));

            Assert::IsFalse(cKB.hasCaller(0));
            Assert::IsFalse(cKB.hasCaller(-1));
            Assert::IsFalse(cKB.hasCaller(4));
        }

        TEST_METHOD(hasCallsRelation) {
            Assert::IsTrue(cKB.hasCallsRelation());

            Assert::IsFalse(ckbEmpty.hasCallsRelation());
        }

        TEST_METHOD(getAllCallers) {
            Assert::IsTrue(cKB.getAllCallers() == std::unordered_set<ProcId>{1, 2});
        }

        TEST_METHOD(getAllCallees) {
            Assert::IsTrue(cKB.getAllCallees() == std::unordered_set<ProcId>{2, 3});
        }
    };
}
