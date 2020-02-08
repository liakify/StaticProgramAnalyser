#include "stdafx.h"
#include "CppUnitTest.h"
#include "ProcTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestProcTable)
	{
	public:
		ProcName PROC_NAME_A = "p";
		ProcName PROC_NAME_B = "X_WYS";
		Procedure PROC_A = Procedure(PROC_NAME_A);
		ProcId PROC_ID_A = 0;
		ProcId PROC_ID_B = 1;
		bool isExceptionThrown = false;
		TEST_METHOD(InitializingProcTable)
		{
			ProcTable procTable;

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

			ProcTable procTable;

			procTable.insertProc(PROC_A);
			Assert::AreEqual(procTable.size(), 1);

			ProcId a = procTable.getProcId(PROC_NAME_A);
			Assert::AreNotEqual(a, -1);
			Assert::IsTrue(procTable.get(a) == PROC_A);
		}
	};
}