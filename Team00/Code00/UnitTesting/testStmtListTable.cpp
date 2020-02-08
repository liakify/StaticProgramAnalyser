#include "stdafx.h"
#include "CppUnitTest.h"
#include "StmtListTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestStmtListTable)
	{
	public:
		StatementList STMTLST_A = StatementList();
		StatementList STMTLST_B = StatementList();
		StmtId STMTLST_ID_A = 0;
		StmtId STMTLST_ID_B = 1;
		bool isExceptionThrown = false;

		TEST_METHOD(InitializingStmtListTable)
		{
			StmtListTable stmtListTable;

			Assert::AreEqual(stmtListTable.size(), 0);
			try {
				stmtListTable.get(STMTLST_ID_A);
			}
			catch (out_of_range & e) {
				isExceptionThrown = true;
			}
			Assert::IsTrue(isExceptionThrown);
			isExceptionThrown = false;
		}

		TEST_METHOD(StmtListTableFunctions) {

			StmtListTable stmtListTable;

			StmtListId stmtList_A_id = stmtListTable.insertStmtLst(STMTLST_A);
			Assert::AreEqual(stmtListTable.size(), 1);

			StatementList stmtlist = stmtListTable.get(stmtList_A_id);
			Assert::IsTrue(stmtlist == STMTLST_A);
		}
	};
}