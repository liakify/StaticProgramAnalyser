#include "stdafx.h"
#include "CppUnitTest.h"
#include "ContainsKB.h"
#include "Constants.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    PKB::ContainsKB containsKB;

    TEST_CLASS(TestContainsKB)
    {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            containsKB.addContains(1, 1);
            containsKB.addContains(1, 2);
            containsKB.addContains(2, 3);
        }

        TEST_METHOD(addContains) {
            Assert::ExpectException<std::invalid_argument>([] {containsKB.addContains(2, 1); });
            Assert::ExpectException<std::invalid_argument>([] {containsKB.addContains(2, 2); });
            Assert::ExpectException<std::invalid_argument>([] {containsKB.addContains(3, 3); });
            try {
                containsKB.addContains(1, 1);
                containsKB.addContains(1, 2);
                containsKB.addContains(2, 3);
            } catch (std::invalid_argument&) {
                Assert::IsTrue(false);
            }
        }

        TEST_METHOD(contains_True) {
            Assert::IsTrue(containsKB.contains(1, 1));
            Assert::IsTrue(containsKB.contains(1, 2));
            Assert::IsTrue(containsKB.contains(2, 3));
        }

        TEST_METHOD(contains_False) {
            Assert::IsFalse(containsKB.contains(2, 1));
            Assert::IsFalse(containsKB.contains(1, 4));
            Assert::IsFalse(containsKB.contains(3, 4));
            Assert::IsFalse(containsKB.contains(-1, 1));
            Assert::IsFalse(containsKB.contains(1, -1));
            Assert::IsFalse(containsKB.contains(-1, -1));
            Assert::IsFalse(containsKB.contains(4, 5));
            Assert::IsFalse(containsKB.contains(5, 4));
            Assert::IsFalse(containsKB.contains(5, 5));
        }


        TEST_METHOD(getContainer) {
            Assert::IsTrue(containsKB.getContainer(1) == 1);
            Assert::IsTrue(containsKB.getContainer(2) == 1);
            Assert::IsTrue(containsKB.getContainer(3) == 2);
            Assert::IsTrue(containsKB.getContainer(4) == INVALID_STMT_ID);
        }

        TEST_METHOD(getAllContains) {
            Assert::IsTrue(containsKB.getAllContains(1) == std::unordered_set<StmtId>({ 1, 2 }));
            Assert::IsTrue(containsKB.getAllContains(2) == std::unordered_set<StmtId>({ 3 }));
            Assert::IsTrue(containsKB.getAllContains(3) == EMPTY_RESULT);
        }
    };
}