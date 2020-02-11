#include "stdafx.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting {
	TEST_CLASS(TestSimple) {
	public:
		VarName VAR_NAME_1 = "qwerty";
		VarName VAR_NAME_2 = "asdf";
		char op = '%';
		VarId VAR_ID_1 = 1;
		VarId VAR_ID_2 = 3;
		ConstValue CONST_VALUE = "999";

		TEST_METHOD(TestExpression) {
			std::unordered_set<VarId> varSet;
			std::unordered_set<ConstValue> constSet;

			Expression varExp = Expression(VAR_NAME_1, VAR_ID_1);
			Assert::AreEqual(varExp.getStr(), VAR_NAME_1);
			Assert::IsTrue(varExp.getConstValues() == std::unordered_set<ConstValue>());
			varSet.insert(VAR_ID_1);
			Assert::IsTrue(varExp.getVarIds() == varSet);

			Expression constExp = Expression(CONST_VALUE);
			Assert::AreEqual(constExp.getStr(), CONST_VALUE);
			Assert::IsTrue(constExp.getVarIds() == std::unordered_set<VarId>());
			constSet.insert(CONST_VALUE);
			Assert::IsTrue(constExp.getConstValues() == constSet);

			Expression exp = Expression(constExp, varExp, op);
			std::string expStr = "(" + CONST_VALUE + op + VAR_NAME_1 + ")";
			Assert::AreEqual(expStr, exp.getStr());
			Assert::IsTrue(exp.getVarIds() == varSet);
			Assert::IsTrue(exp.getConstValues() == constSet);

			Expression varExp2 = Expression(VAR_NAME_2, VAR_ID_2);
			Expression exp2 = Expression(varExp2, exp, op);
			std::string exp2Str =
				"(" + VAR_NAME_2 + op + "(" + CONST_VALUE + op + VAR_NAME_1 + "))";
			varSet.insert(VAR_ID_2);
			Assert::IsTrue(exp2.getVarIds() == varSet);
			Assert::IsTrue(exp2.getConstValues() == constSet);
		}
	};
}