#include "stdafx.h"
#include "CppUnitTest.h"
#include "StmtTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestStmtTable)
	{
	public:
		StmtType STMT_TYPE_ASSIGN = "assign";
		StmtType STMT_TYPE_WHILE = "while";
		Statement STMT_A = Statement(STMT_TYPE_ASSIGN);
		Statement STMT_B = Statement(STMT_TYPE_WHILE);
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
			Assert::IsTrue(stmtTable.getStmtsByType(STMT_TYPE_ASSIGN).empty());
		}

		TEST_METHOD(StmtTableFunctions) {

			PKB::StmtTable stmtTable;

			StmtId stmt_A_id = stmtTable.insertStmt(STMT_A);
			Assert::AreEqual(stmtTable.size(), 1);

			list<StmtId> list = stmtTable.getStmtsByType(STMT_TYPE_ASSIGN);

			Assert::IsFalse(list.empty());
			Assert::AreEqual(list.front(), stmt_A_id);
		}
	};
}