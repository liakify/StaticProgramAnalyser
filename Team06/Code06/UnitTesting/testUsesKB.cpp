#include "stdafx.h"
#include "CppUnitTest.h"
#include "UsesKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    UsesKB usesKB;

    TEST_CLASS(TestUsesKB)
    {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            usesKB.addStmtUses(1, 1);
            usesKB.addStmtUses(1, 2);
            usesKB.addStmtUses(2, 1);
        }

        TEST_METHOD(stmtUses) {
            Assert::IsTrue(usesKB.stmtUses(1, 1));
            Assert::IsTrue(usesKB.stmtUses(1, 2));
            Assert::IsTrue(usesKB.stmtUses(2, 1));
            Assert::IsFalse(usesKB.stmtUses(-1, 1));
            Assert::IsFalse(usesKB.stmtUses(1, -1));
            Assert::IsFalse(usesKB.stmtUses(-1, -1));
        }

        TEST_METHOD(getAllVarsUsedByStmt) {
            Assert::IsTrue(usesKB.getAllVarsUsedByStmt(1) == std::unordered_set<VarId>({1, 2}));
            Assert::IsTrue(usesKB.getAllVarsUsedByStmt(2) == std::unordered_set<VarId>({ 1 }));
            Assert::IsTrue(usesKB.getAllVarsUsedByStmt(-1) == std::unordered_set<VarId>({}));
            Assert::IsTrue(usesKB.getAllVarsUsedByStmt(3) == std::unordered_set<VarId>({}));
        }

        TEST_METHOD(getAllStmtsUsingVar) {
            Assert::IsTrue(usesKB.getAllStmtsUsingVar(1) == std::unordered_set<StmtId>({ 1, 2 }));
            Assert::IsTrue(usesKB.getAllStmtsUsingVar(2) == std::unordered_set<StmtId>({ 1 }));
            Assert::IsTrue(usesKB.getAllStmtsUsingVar(-1) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(usesKB.getAllStmtsUsingVar(3) == std::unordered_set<StmtId>({}));
        }
    };
}