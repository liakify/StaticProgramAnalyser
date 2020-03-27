#include "stdafx.h"

#include "CppUnitTest.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting {
    PKB::PKB aKB;

    TEST_CLASS(TestAffectsKB) {
    public:
        std::unordered_set<StmtId> emptyResult;

        TEST_CLASS_INITIALIZE(setup) {
            aKB.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, Expression("x", 1, ExprType::VAR))));
            aKB.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, Expression("y", 2, ExprType::VAR))));
            aKB.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, Expression("z", 3, ExprType::VAR))));
            aKB.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(4)));

            aKB.addAffects(1, 2);
            aKB.addAffects(2, 3);
            aKB.affectsSetProcessedAll(1, NodeType::SUCCESSOR);
            aKB.affectsSetProcessedAll(2, NodeType::SUCCESSOR);
            aKB.affectsSetProcessedAll(3, NodeType::SUCCESSOR);
            aKB.affectsSetProcessedAll(3, NodeType::PREDECESSOR);
            aKB.affectsSetProcessedAll(2, NodeType::PREDECESSOR);
            aKB.affectsSetProcessedAll(1, NodeType::PREDECESSOR);
        }

        TEST_METHOD(addAffects) {
            Assert::IsTrue(aKB.affectsGetDirectNodes(1, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{2});
            Assert::IsTrue(aKB.affectsGetDirectNodes(2, NodeType::SUCCESSOR) == std::unordered_set<StmtId>{3});
            Assert::IsTrue(aKB.affectsGetDirectNodes(3, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(aKB.affectsGetDirectNodes(1, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(aKB.affectsGetDirectNodes(2, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{1});
            Assert::IsTrue(aKB.affectsGetDirectNodes(3, NodeType::PREDECESSOR) == std::unordered_set<StmtId>{2});
        }

        TEST_METHOD(affects) {
            Assert::IsTrue(aKB.affects(1, 2));
            Assert::IsTrue(aKB.affects(2, 3));

            Assert::IsFalse(aKB.affects(1, 3));
            Assert::IsFalse(aKB.affects(2, 1));
            Assert::IsFalse(aKB.affects(3, 1));
            Assert::IsFalse(aKB.affects(3, 2));

            Assert::IsFalse(aKB.affects(0, 1));
            Assert::IsFalse(aKB.affects(1, -1));
            Assert::IsFalse(aKB.affects(5, -1));
        }

        TEST_METHOD(affectsGetDirectNodes) {
            Assert::IsTrue(aKB.affectsGetDirectNodes(0, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(aKB.affectsGetDirectNodes(5, NodeType::SUCCESSOR) == emptyResult);
            Assert::IsTrue(aKB.affectsGetDirectNodes(-1, NodeType::SUCCESSOR) == emptyResult);

            Assert::IsTrue(aKB.affectsGetDirectNodes(0, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(aKB.affectsGetDirectNodes(5, NodeType::PREDECESSOR) == emptyResult);
            Assert::IsTrue(aKB.affectsGetDirectNodes(-1, NodeType::PREDECESSOR) == emptyResult);
        }

        TEST_METHOD(affectsSetProcessedAll) {
            Assert::IsTrue(aKB.affectsProcessedAll(1, NodeType::SUCCESSOR));
            Assert::IsTrue(aKB.affectsProcessedAll(2, NodeType::SUCCESSOR));
            Assert::IsTrue(aKB.affectsProcessedAll(3, NodeType::SUCCESSOR));

            Assert::IsTrue(aKB.affectsProcessedAll(1, NodeType::PREDECESSOR));
            Assert::IsTrue(aKB.affectsProcessedAll(2, NodeType::PREDECESSOR));
            Assert::IsTrue(aKB.affectsProcessedAll(3, NodeType::PREDECESSOR));

            Assert::IsFalse(aKB.affectsProcessedAll(4, NodeType::SUCCESSOR));
            Assert::IsFalse(aKB.affectsProcessedAll(4, NodeType::PREDECESSOR));
        }
    };
}
