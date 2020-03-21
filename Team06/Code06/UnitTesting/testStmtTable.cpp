#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting
{
    TEST_CLASS(TestStmtTable)
    {
    public:
        VarId VAR_ID = 1;
        std::shared_ptr<Statement> STMT_A = std::shared_ptr<Statement>(new PrintStmt(VAR_ID));
        std::shared_ptr<Statement> STMT_B = std::shared_ptr<Statement>(new ReadStmt(VAR_ID));
        StmtId STMT_ID_A = 0;
        StmtId STMT_ID_B = 1;
        bool isExceptionThrown = false;

        TEST_METHOD(InitializingStmtTable)
        {
            PKB::StmtTable stmtTable;

            Assert::AreEqual(stmtTable.size(), 0);
            try {
                stmtTable.get(STMT_ID_A);
            }
            catch (out_of_range &) {
                isExceptionThrown = true;
            }
            Assert::IsTrue(isExceptionThrown);
            isExceptionThrown = false;
            Assert::IsTrue(stmtTable.getStmtsByType(StmtType::PRINT).empty());
        }

        TEST_METHOD(StmtTableFunctions) {

            PKB::StmtTable stmtTable;

            StmtId stmt_A_id = stmtTable.insertStmt(STMT_A);
            Assert::AreEqual(stmtTable.size(), 1);

            std::unordered_set<StmtId> set = stmtTable.getStmtsByType(StmtType::PRINT);

            Assert::IsFalse(set.empty());
            Assert::IsFalse(set.find(stmt_A_id) == set.end());

            Assert::ExpectException<std::invalid_argument>([&stmtTable, this] {stmtTable.insertStmtAtId(STMT_B, 1); });
            StmtId reserved = stmtTable.reserveId();
            StmtId stmt_B_id = stmtTable.insertStmt(STMT_B);
            Assert::IsTrue(set.find(reserved) == set.end());
            try {
                stmtTable.insertStmtAtId(STMT_B, reserved);
            }
            catch (std::invalid_argument&) {
                Assert::IsTrue(false);
            }
        }
    };
}