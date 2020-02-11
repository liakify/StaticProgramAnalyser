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

		Expression varExp = Expression(VAR_NAME_1, VAR_ID_1);
		Expression varExp2 = Expression(VAR_NAME_2, VAR_ID_2);
		Expression constExp = Expression(CONST_VALUE);
		Expression exp = Expression(constExp, varExp, op);
		Expression exp2 = Expression(varExp2, exp, op);

		CondExpr cond1 = CondExpr(varExp2, varExp2);
		CondExpr cond2 = CondExpr(constExp, exp);
		CondExpr cond3 = CondExpr(cond1, cond2);

		TEST_METHOD(TestExpression) {
			std::unordered_set<VarId> varSet;
			std::unordered_set<ConstValue> constSet;

			Assert::AreEqual(varExp.getStr(), VAR_NAME_1);
			Assert::IsTrue(varExp.getConstValues() == std::unordered_set<ConstValue>());
			varSet.insert(VAR_ID_1);
			Assert::IsTrue(varExp.getVarIds() == varSet);

			Assert::AreEqual(constExp.getStr(), CONST_VALUE);
			Assert::IsTrue(constExp.getVarIds() == std::unordered_set<VarId>());
			constSet.insert(CONST_VALUE);
			Assert::IsTrue(constExp.getConstValues() == constSet);

			std::string expStr = "(" + CONST_VALUE + op + VAR_NAME_1 + ")";
			Assert::AreEqual(expStr, exp.getStr());
			Assert::IsTrue(exp.getVarIds() == varSet);
			Assert::IsTrue(exp.getConstValues() == constSet);

			std::string exp2Str =
				"(" + VAR_NAME_2 + op + "(" + CONST_VALUE + op + VAR_NAME_1 + "))";
			varSet.insert(VAR_ID_2);
			Assert::IsTrue(exp2.getVarIds() == varSet);
			Assert::IsTrue(exp2.getConstValues() == constSet);
		}

		TEST_METHOD(TestCondExpr) {
			std::unordered_set<VarId> varSet, varSet2;
			std::unordered_set<ConstValue> constSet;

			varSet.insert(VAR_ID_2);
			Assert::IsTrue(cond1.getVarIds() == varSet);
			Assert::IsTrue(cond1.getConstValues() == constSet);

			varSet2.insert(VAR_ID_1);
			constSet.insert(CONST_VALUE);
			Assert::IsTrue(cond2.getVarIds() == varSet2);
			Assert::IsTrue(cond2.getConstValues() == constSet);

			varSet.insert(VAR_ID_1);
			Assert::IsTrue(cond3.getVarIds() == varSet);
			Assert::IsTrue(cond3.getConstValues() == constSet);
		}
	};
}