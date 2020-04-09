#include "stdafx.h"
#include "CppUnitTest.h"
#include "PatternKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    PKB::PatternKB patternKB;

    TEST_CLASS(TestPatternKB)
    {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            patternKB.addAssignPattern("a", 1);
            patternKB.addAssignPattern("b", 1);
            patternKB.addAssignPattern("a", 2);

            patternKB.addIfPattern(1, 1);
            patternKB.addIfPattern(2, 1);
            patternKB.addIfPattern(1, 2);

            patternKB.addWhilePattern(1, 1);
            patternKB.addWhilePattern(2, 1);
            patternKB.addWhilePattern(1, 2);
        }

        TEST_METHOD(getAssignPatternStmts) {
            Assert::IsTrue(patternKB.getAssignPatternStmts("a") == std::unordered_set<StmtId>({ 1, 2 }));
            Assert::IsTrue(patternKB.getAssignPatternStmts("b") == std::unordered_set<StmtId>({ 1 }));
            Assert::IsTrue(patternKB.getAssignPatternStmts("") == std::unordered_set<StmtId>());
            Assert::IsTrue(patternKB.getAssignPatternStmts("c") == std::unordered_set<StmtId>());
        }

        TEST_METHOD(getIfPatternStmts) {
            Assert::IsTrue(patternKB.getIfPatternStmts(1) == std::unordered_set<StmtId>({ 1, 2 }));
            Assert::IsTrue(patternKB.getIfPatternStmts(2) == std::unordered_set<StmtId>({ 1 }));
            Assert::IsTrue(patternKB.getIfPatternStmts(-1) == std::unordered_set<StmtId>());
            Assert::IsTrue(patternKB.getIfPatternStmts(0) == std::unordered_set<StmtId>());
            Assert::IsTrue(patternKB.getIfPatternStmts(3) == std::unordered_set<StmtId>());
        }

        TEST_METHOD(getWhilePatternStmts) {
            Assert::IsTrue(patternKB.getWhilePatternStmts(1) == std::unordered_set<StmtId>({ 1, 2 }));
            Assert::IsTrue(patternKB.getWhilePatternStmts(2) == std::unordered_set<StmtId>({ 1 }));
            Assert::IsTrue(patternKB.getIfPatternStmts(-1) == std::unordered_set<StmtId>());
            Assert::IsTrue(patternKB.getWhilePatternStmts(0) == std::unordered_set<StmtId>());
            Assert::IsTrue(patternKB.getWhilePatternStmts(3) == std::unordered_set<StmtId>());
        }
    };
}