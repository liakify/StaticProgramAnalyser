#include "stdafx.h"
#include "CppUnitTest.h"
#include "ProcTable.h"
#include "ProcTable.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestProcTable)
	{
	public:
		ProcName PROC_NAME_A = "p";
		ProcName PROC_NAME_B = "X_WYS";
		StmtListId STMTLST_ID = 2;
		Procedure PROC_A = Procedure(PROC_NAME_A, STMTLST_ID);
		ProcId PROC_ID_A = 0;
		ProcId PROC_ID_B = 1;
		bool isExceptionThrown = false;
		TEST_METHOD(InitializingProcTable)
		{
			PKB::ProcTable procTable;

			Assert::AreEqual(procTable.size(), 0);
			try {
				procTable.get(PROC_ID_A);
			}
			catch (out_of_range &) {
				isExceptionThrown = true;
			}
			Assert::IsTrue(isExceptionThrown);
			isExceptionThrown = false;
			Assert::AreEqual(procTable.getProcId(PROC_NAME_A), -1);
		}

		TEST_METHOD(ProcTableFunctions) {

			PKB::ProcTable procTable;

			ProcId insertProcResult = procTable.insertProc(PROC_A);
			Assert::AreEqual(procTable.size(), 1);

			ProcId a = procTable.getProcId(PROC_NAME_A);
			Assert::AreNotEqual(a, -1);
			Assert::AreEqual(insertProcResult, a);

			Assert::IsTrue(procTable.get(a) == PROC_A);
		}
	};
}