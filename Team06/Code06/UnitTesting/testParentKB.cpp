#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentKB.h"
#include "Constants.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    PKB::ParentKB parentKB;
    PKB::ParentKB parentKBEmpty;

    TEST_CLASS(TestParentKB)
    {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            parentKB.addParent(1, 2);
            parentKB.addParent(2, 4);
            parentKB.setAllChildren(1, std::unordered_set<StmtId>({ 2, 4 }));
            parentKB.setAllChildren(2, std::unordered_set<StmtId>({ 4 }));
            parentKB.setAllParents(2, std::unordered_set<StmtId>({ 1 }));
            parentKB.setAllParents(4, std::unordered_set<StmtId>({ 1, 2 }));
        }

        TEST_METHOD(addParent) {
            Assert::IsTrue(parentKB.getDirectChildren(1) == std::unordered_set<StmtId>({ 2 }));
            Assert::IsTrue(parentKB.getParent(1) == INVALID_STMT_ID);

            Assert::IsTrue(parentKB.getDirectChildren(2) == std::unordered_set<StmtId>({ 4 }));
            Assert::IsTrue(parentKB.getParent(2) == 1);

            Assert::IsTrue(parentKB.getDirectChildren(4) == std::unordered_set<StmtId>());
            Assert::IsTrue(parentKB.getParent(4) == 2);
        }

        TEST_METHOD(parent_True) {
            Assert::IsTrue(parentKB.parent(1, 2));
            Assert::IsTrue(parentKB.parent(2, 4));
        }

        TEST_METHOD(parent_False) {
            Assert::IsFalse(parentKB.parent(2, 1));
            Assert::IsFalse(parentKB.parent(2, 3));
            Assert::IsFalse(parentKB.parent(1, 4));
            Assert::IsFalse(parentKB.parent(3, 4));
            Assert::IsFalse(parentKB.parent(-1, 1));
            Assert::IsFalse(parentKB.parent(1, -1));
            Assert::IsFalse(parentKB.parent(-1, -1));
            Assert::IsFalse(parentKB.parent(4, 5));
            Assert::IsFalse(parentKB.parent(5, 4));
            Assert::IsFalse(parentKB.parent(5, 5));
        }

        TEST_METHOD(parentStar_True) {
            Assert::IsTrue(parentKB.parentStar(1, 2));
            Assert::IsTrue(parentKB.parentStar(1, 4));
            Assert::IsTrue(parentKB.parentStar(2, 4));
        }

        TEST_METHOD(parentStar_False) {
            Assert::IsFalse(parentKB.parentStar(2, 3));
            Assert::IsFalse(parentKB.parentStar(3, 4));
            Assert::IsFalse(parentKB.parentStar(-1, 1));
            Assert::IsFalse(parentKB.parentStar(1, -1));
            Assert::IsFalse(parentKB.parentStar(-1, -1));
            Assert::IsFalse(parentKB.parentStar(4, 5));
            Assert::IsFalse(parentKB.parentStar(5, 4));
            Assert::IsFalse(parentKB.parentStar(5, 5));
        }

        TEST_METHOD(getDirectChildren) {
            Assert::IsTrue(parentKB.getDirectChildren(3) == std::unordered_set<StmtId>());
            Assert::IsTrue(parentKB.getDirectChildren(-1) == std::unordered_set<StmtId>());
            Assert::IsTrue(parentKB.getDirectChildren(5) == std::unordered_set<StmtId>());
        }

        TEST_METHOD(getParent) {
            Assert::IsTrue(parentKB.getParent(3) == INVALID_STMT_ID);
            Assert::IsTrue(parentKB.getParent(-1) == INVALID_STMT_ID);
            Assert::IsTrue(parentKB.getParent(5) == INVALID_STMT_ID);
        }

        TEST_METHOD(hasDirectChildren) {
            Assert::IsTrue(parentKB.hasDirectChildren(1));
            Assert::IsTrue(parentKB.hasDirectChildren(2));
            Assert::IsFalse(parentKB.hasDirectChildren(4));
            Assert::IsFalse(parentKB.hasDirectChildren(-1));
            Assert::IsFalse(parentKB.hasDirectChildren(5));
            Assert::IsFalse(parentKB.hasDirectChildren(3));
        }

        TEST_METHOD(hasParent) {
            Assert::IsFalse(parentKB.hasParent(1));
            Assert::IsTrue(parentKB.hasParent(2));
            Assert::IsTrue(parentKB.hasParent(4));
            Assert::IsFalse(parentKB.hasParent(-1));
            Assert::IsFalse(parentKB.hasParent(5));
            Assert::IsFalse(parentKB.hasParent(3));
        }

        TEST_METHOD(getAllChildren) {
            Assert::IsTrue(parentKB.getAllChildren(1) == std::unordered_set<StmtId>({ 2, 4 }));
            Assert::IsTrue(parentKB.getAllChildren(2) == std::unordered_set<StmtId>({ 4 }));
            Assert::IsTrue(parentKB.getAllChildren(4) == std::unordered_set<StmtId>());
            Assert::IsTrue(parentKB.getAllChildren(-1) == std::unordered_set<StmtId>());
            Assert::IsTrue(parentKB.getAllChildren(3) == std::unordered_set<StmtId>());
            Assert::IsTrue(parentKB.getAllChildren(5) == std::unordered_set<StmtId>());
        }

        TEST_METHOD(getAllParents) {
            Assert::IsTrue(parentKB.getAllParents(1) == std::unordered_set<StmtId>());
            Assert::IsTrue(parentKB.getAllParents(2) == std::unordered_set<StmtId>({ 1 }));
            Assert::IsTrue(parentKB.getAllParents(4) == std::unordered_set<StmtId>({ 1, 2 }));
            Assert::IsTrue(parentKB.getAllParents(-1) == std::unordered_set<StmtId>());
            Assert::IsTrue(parentKB.getAllParents(3) == std::unordered_set<StmtId>());
            Assert::IsTrue(parentKB.getAllParents(5) == std::unordered_set<StmtId>());
        }

        TEST_METHOD(hasParentRelation) {
            Assert::IsTrue(parentKB.hasParentRelation());
            Assert::IsFalse(parentKBEmpty.hasParentRelation());
        }
    };
}