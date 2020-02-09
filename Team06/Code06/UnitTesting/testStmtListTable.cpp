#include "stdafx.h"
#include "CppUnitTest.h"
#include "StmtListTable.h"
#include "Simple.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::vector;
using SIMPLE::Statement;

namespace UnitTesting
{
	TEST_CLASS(TestStmtListTable)
	{
	public:
		StatementList STMTLST_A = StatementList(vector<Statement>());;
		StatementList STMTLST_B = StatementList(vector<Statement>());
		StmtId STMTLST_ID_A = 0;
		StmtId STMTLST_ID_B = 1;
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