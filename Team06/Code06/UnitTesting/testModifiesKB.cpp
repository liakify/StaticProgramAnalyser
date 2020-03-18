#include "stdafx.h"
#include "CppUnitTest.h"
#include "ModifiesKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    ModifiesKB mKB;

    TEST_CLASS(TestModifiesKB)
    {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            mKB.addStmtModifies(1, 1);
            mKB.addStmtModifies(1, 2);
            mKB.addStmtModifies(2, 1);
        }

        TEST_METHOD(stmtModifies) {
            Assert::IsTrue(mKB.stmtModifies(1, 1));
            Assert::IsTrue(mKB.stmtModifies(1, 2));
            Assert::IsTrue(mKB.stmtModifies(2, 1));
            Assert::IsFalse(mKB.stmtModifies(-1, 1));
            Assert::IsFalse(mKB.stmtModifies(1, -1));
            Assert::IsFalse(mKB.stmtModifies(-1, -1));
        }

        TEST_METHOD(getAllVarsModifiedByStmt) {
            Assert::IsTrue(mKB.getAllVarsModifiedByStmt(1) == std::unordered_set<VarId>({ 1, 2 }));
            Assert::IsTrue(mKB.getAllVarsModifiedByStmt(2) == std::unordered_set<VarId>({ 1 }));
            Assert::IsTrue(mKB.getAllVarsModifiedByStmt(-1) == std::unordered_set<VarId>({}));
            Assert::IsTrue(mKB.getAllVarsModifiedByStmt(3) == std::unordered_set<VarId>({}));
        }

        TEST_METHOD(getAllStmtsModifyVar) {
            Assert::IsTrue(mKB.getAllStmtsModifyVar(1) == std::unordered_set<StmtId>({ 1, 2 }));
            Assert::IsTrue(mKB.getAllStmtsModifyVar(2) == std::unordered_set<StmtId>({ 1 }));
            Assert::IsTrue(mKB.getAllStmtsModifyVar(-1) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(mKB.getAllStmtsModifyVar(3) == std::unordered_set<StmtId>({}));
        }
    };
}