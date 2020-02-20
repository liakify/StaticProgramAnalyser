#include "stdafx.h"
#include "CppUnitTest.h"
#include "PatternKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	PatternKB patternKB;

	TEST_CLASS(TestPatternKB)
	{
	public:
		TEST_CLASS_INITIALIZE(setup) {
			patternKB.addLHSPattern("a", 1);
			patternKB.addRHSPattern("b", 1);
		}

		TEST_METHOD(getLHSPatternStmts) {
			Assert::IsTrue(patternKB.getLHSPatternStmts("a") == std::unordered_set<StmtId>({ 1 }));
			Assert::IsTrue(patternKB.getLHSPatternStmts("") == std::unordered_set<StmtId>());
			Assert::IsTrue(patternKB.getLHSPatternStmts("c") == std::unordered_set<StmtId>());
		}

		TEST_METHOD(getRHSPatternStmts) {
			Assert::IsTrue(patternKB.getRHSPatternStmts("b") == std::unordered_set<StmtId>({ 1 }));
			Assert::IsTrue(patternKB.getRHSPatternStmts("") == std::unordered_set<StmtId>());
			Assert::IsTrue(patternKB.getRHSPatternStmts("c") == std::unordered_set<StmtId>());
		}
	};
}