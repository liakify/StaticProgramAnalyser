#include "stdafx.h"
#include "CppUnitTest.h"
#include "StmtListTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestStmtListTable)
	{
	public:
		StmtId STMT_ID = 1;
		std::vector<StmtId> stmt_vector = std::vector<StmtId>(1, STMT_ID);
		StatementList STMTLST_A = StatementList(stmt_vector);
		StatementList STMTLST_B = StatementList(stmt_vector);
		StmtListId STMTLST_ID_A = 0;
		StmtListId STMTLST_ID_B = 1;
		bool isExceptionThrown = false;

		TEST_METHOD(InitializingStmtListTable)
		{
			PKB::StmtListTable stmtListTable;

			Assert::AreEqual(stmtListTable.size(), 0);
			try {
				stmtListTable.get(STMTLST_ID_A);
			}
			catch (out_of_range &) {
				isExceptionThrown = true;
			}
			Assert::IsTrue(isExceptionThrown);
			isExceptionThrown = false;
		}

		TEST_METHOD(StmtListTableFunctions) {

			PKB::StmtListTable stmtListTable;

			StmtListId stmtList_A_id = stmtListTable.insertStmtLst(STMTLST_A);
			Assert::AreEqual(stmtListTable.size(), 1);

			StatementList stmtlist = stmtListTable.get(stmtList_A_id);
			Assert::IsTrue(stmtlist == STMTLST_A);
		}
	};
}