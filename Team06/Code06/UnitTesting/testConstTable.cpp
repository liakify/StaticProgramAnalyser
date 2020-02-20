#include "stdafx.h"
#include "CppUnitTest.h"
#include "ConstTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestConstTable)
	{
	public:
		ConstValue CONST_VALUE_A = "0";
		ConstValue CONST_VALUE_B = "123";
		ConstId CONST_ID_A = 0;
		ConstId CONST_ID_B = 1;
		bool isExceptionThrown = false;
		TEST_METHOD(InitializingConstTable)
		{
			PKB::ConstTable constTable;

			Assert::AreEqual(constTable.size(), 0);
			try {
				constTable.get(CONST_ID_A);
			}
			catch (out_of_range&) {
				isExceptionThrown = true;
			}
			Assert::IsTrue(isExceptionThrown);
			isExceptionThrown = false;
			Assert::AreEqual(constTable.getConstId(CONST_VALUE_A), -1);
		}

		TEST_METHOD(ConstTableFunctions) {

			PKB::ConstTable constTable;

			VarId insertConstResult = constTable.insertConst(CONST_VALUE_A);
			Assert::AreEqual(constTable.size(), 1);

			VarId a = constTable.getConstId(CONST_VALUE_A);
			Assert::AreNotEqual(a, -1);
			Assert::AreEqual(a, insertConstResult);

			// Checks for get()
			Assert::AreEqual(constTable.get(a), CONST_VALUE_A);
		}
	};
}