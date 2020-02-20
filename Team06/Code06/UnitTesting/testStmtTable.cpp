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
		PrintStmt PRINT_STMT = PrintStmt(VAR_ID);
		ReadStmt READ_STMT = ReadStmt(VAR_ID);
		Statement STMT_A = PRINT_STMT;
		Statement STMT_B = READ_STMT;
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
			Assert::IsTrue(stmtTable.getStmtsByType(PRINT).empty());
		}

		TEST_METHOD(StmtTableFunctions) {

			PKB::StmtTable stmtTable;

			StmtId stmt_A_id = stmtTable.insertStmt(STMT_A);
			Assert::AreEqual(stmtTable.size(), 1);

			unordered_set<StmtId> set = stmtTable.getStmtsByType(PRINT);

			Assert::IsFalse(set.empty());
			Assert::IsFalse(set.find(stmt_A_id) == set.end());

			stmtTable.insertStmtAtId(STMT_B, 1);
			Assert::IsTrue(stmtTable.get(1).getType() == READ);
			Assert::IsTrue(stmtTable.get(2).getType() == PRINT);
			stmtTable.insertStmtAtId(STMT_A, 1);
			Assert::IsTrue(stmtTable.get(1).getType() == PRINT);
			Assert::IsTrue(stmtTable.get(2).getType() == READ);
			Assert::IsTrue(stmtTable.get(3).getType() == PRINT);
		}
	};
}