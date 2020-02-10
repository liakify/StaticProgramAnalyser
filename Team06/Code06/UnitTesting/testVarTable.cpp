#include "stdafx.h"
#include "CppUnitTest.h"
#include "VarTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestVarTable)
	{
	public:
		VarName VAR_NAME_A = "p";
		VarName VAR_NAME_B = "X_WYS";
		VarId VAR_ID_A = 0;
		VarId VAR_ID_B = 1;
		bool isExceptionThrown = false;
		TEST_METHOD(InitializingVarTable)
		{
			PKB::VarTable varTable;
			
			Assert::AreEqual(varTable.size(), 0);
			try {
				varTable.get(VAR_ID_A);
			}
			catch (out_of_range &) {
				isExceptionThrown = true;
				}
			Assert::IsTrue(isExceptionThrown);
			isExceptionThrown = false;
			Assert::AreEqual(varTable.getVarId(VAR_NAME_A), -1);
		}

		TEST_METHOD(VarTableFunctions) {

			PKB::VarTable varTable;

			varTable.insertVar(VAR_NAME_A);
			Assert::AreEqual(varTable.size(), 1);
			
			VarId a = varTable.getVarId(VAR_NAME_A);
			Assert::AreNotEqual(a, -1);
			Assert::AreEqual(varTable.get(a), VAR_NAME_A);
		}
	};
}