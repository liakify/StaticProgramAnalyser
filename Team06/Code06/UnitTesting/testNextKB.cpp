#include "stdafx.h"
#include "CppUnitTest.h"
#include "NextKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    NextKB nextKB;

    TEST_CLASS(TestNextKB)
    {
        TEST_CLASS_INITIALIZE(setup) {
            nextKB.addNext(1, 2);
            nextKB.addNext(1, 3);
            nextKB.addNext(2, 3);
            nextKB.addNext(3, 1);
            nextKB.addNext(3, 4);
        }

        TEST_METHOD(addNext) {

        }

        TEST_METHOD(next) {
            Assert::IsTrue(nextKB.next(1, 2));
            Assert::IsTrue(nextKB.next(1, 3));
            Assert::IsTrue(nextKB.next(2, 3));
            Assert::IsTrue(nextKB.next(3, 1));
            Assert::IsTrue(nextKB.next(3, 4));

            Assert::IsFalse(nextKB.next(1, 4));
            Assert::IsFalse(nextKB.next(2, 1));
            Assert::IsFalse(nextKB.next(2, 4));
            Assert::IsFalse(nextKB.next(3, 2));
            Assert::IsFalse(nextKB.next(4, 1));
            Assert::IsFalse(nextKB.next(4, 2));
            Assert::IsFalse(nextKB.next(4, 3));
        }

        TEST_METHOD(nextStar) {
            Assert::IsTrue(nextKB.nextStar(1, 1));
            Assert::IsTrue(nextKB.nextStar(1, 2));
            Assert::IsTrue(nextKB.nextStar(1, 3));
            Assert::IsTrue(nextKB.nextStar(1, 4));
            Assert::IsTrue(nextKB.nextStar(2, 1));
            Assert::IsTrue(nextKB.nextStar(2, 2));
            Assert::IsTrue(nextKB.nextStar(2, 3));
            Assert::IsTrue(nextKB.nextStar(2, 4));
            Assert::IsTrue(nextKB.nextStar(3, 1));
            Assert::IsTrue(nextKB.nextStar(3, 2));
            Assert::IsTrue(nextKB.nextStar(3, 3));
            Assert::IsTrue(nextKB.nextStar(3, 4));

            Assert::IsFalse(nextKB.nextStar(4, 1));
            Assert::IsFalse(nextKB.nextStar(4, 2));
            Assert::IsFalse(nextKB.nextStar(4, 3));
            Assert::IsFalse(nextKB.nextStar(4, 4));
        }

        TEST_METHOD(getAllStmtsPrevious) {
            Assert::IsTrue(nextKB.getAllStmtsPrevious(1) == std::vector<StmtId>({3}));
            Assert::IsTrue(nextKB.getAllStmtsPrevious(2) == std::vector<StmtId>({1}));
            Assert::IsTrue(nextKB.getAllStmtsPrevious(3) == std::vector<StmtId>({1, 2}));
            Assert::IsTrue(nextKB.getAllStmtsPrevious(4) == std::vector<StmtId>({3}));
        }

        TEST_METHOD(getAllStmtsNext) {
            Assert::IsTrue(nextKB.getAllStmtsNext(1) == std::vector<StmtId>({2, 3}));
            Assert::IsTrue(nextKB.getAllStmtsNext(2) == std::vector<StmtId>({3}));
            Assert::IsTrue(nextKB.getAllStmtsNext(3) == std::vector<StmtId>({1, 4}));
            Assert::IsTrue(nextKB.getAllStmtsNext(4) == std::vector<StmtId>({}));
        }

        TEST_METHOD(getAllStmtsPreviousStar) {
            Assert::IsTrue(nextKB.getAllStmtsPreviousStar(1) == std::vector<StmtId>({1, 2, 3}));
            Assert::IsTrue(nextKB.getAllStmtsPreviousStar(2) == std::vector<StmtId>({1, 2, 3}));
            Assert::IsTrue(nextKB.getAllStmtsPreviousStar(3) == std::vector<StmtId>({1, 2, 3}));
            Assert::IsTrue(nextKB.getAllStmtsPreviousStar(4) == std::vector<StmtId>({1, 2, 3}));    
        }

        TEST_METHOD(getAllStmtsNextStar) {
            Assert::IsTrue(nextKB.getAllStmtsNextStar(1) == std::vector<StmtId>({1, 2, 3, 4}));
            Assert::IsTrue(nextKB.getAllStmtsNextStar(2) == std::vector<StmtId>({1, 2, 3, 4}));
            Assert::IsTrue(nextKB.getAllStmtsNextStar(3) == std::vector<StmtId>({1, 2, 3, 4}));
            Assert::IsTrue(nextKB.getAllStmtsNextStar(4) == std::vector<StmtId>({}));
        }            
    }
}
