#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowsKB.h"
#include "Constants.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    FollowsKB fKB;

    TEST_CLASS(TestFollowsKB)
    {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            fKB.addFollows(1, 2);
            fKB.addFollows(2, 4);
            fKB.setAllFollowers(1, std::unordered_set<StmtId>({2, 4}));
            fKB.setAllFollowers(2, std::unordered_set<StmtId>({4}));
            fKB.setAllFollowing(2, std::unordered_set<StmtId>({1}));
            fKB.setAllFollowing(4, std::unordered_set<StmtId>({1, 2}));
        }

        TEST_METHOD(addFollows)
        {
            Assert::IsTrue(fKB.getFollower(1) == 2);
            Assert::IsTrue(fKB.getFollowing(1) == INVALID_STMT_ID);

            Assert::IsTrue(fKB.getFollower(2) == 4);
            Assert::IsTrue(fKB.getFollowing(2) == 1);

            Assert::IsTrue(fKB.getFollower(4) == INVALID_STMT_ID);
            Assert::IsTrue(fKB.getFollowing(4) == 2);
        }

        TEST_METHOD(follows_True)
        {
            Assert::IsTrue(fKB.follows(1, 2));
            Assert::IsTrue(fKB.follows(2, 4));
        }

        TEST_METHOD(follows_False)
        {
            Assert::IsFalse(fKB.follows(2, 1));
            Assert::IsFalse(fKB.follows(2, 3));
            Assert::IsFalse(fKB.follows(1, 4));
            Assert::IsFalse(fKB.follows(3, 4));
            Assert::IsFalse(fKB.follows(-1, 1));
            Assert::IsFalse(fKB.follows(1, -1));
            Assert::IsFalse(fKB.follows(-1, -1));
            Assert::IsFalse(fKB.follows(4, 5));
            Assert::IsFalse(fKB.follows(5, 4));
            Assert::IsFalse(fKB.follows(5, 5));
        }

        TEST_METHOD(followStar_True)
        {
            Assert::IsTrue(fKB.followStar(1, 2));
            Assert::IsTrue(fKB.followStar(1, 4));
            Assert::IsTrue(fKB.followStar(2, 4));
        }

        TEST_METHOD(followStar_False)
        {
            Assert::IsFalse(fKB.followStar(2, 3));
            Assert::IsFalse(fKB.followStar(3, 4));
            Assert::IsFalse(fKB.followStar(-1, 1));
            Assert::IsFalse(fKB.followStar(1, -1));
            Assert::IsFalse(fKB.followStar(-1, -1));
            Assert::IsFalse(fKB.followStar(4, 5));
            Assert::IsFalse(fKB.followStar(5, 4));
            Assert::IsFalse(fKB.followStar(5, 5));
        }

        TEST_METHOD(getFollower)
        {
            Assert::IsTrue(fKB.getFollower(3) == INVALID_STMT_ID);
            Assert::IsTrue(fKB.getFollower(-1) == INVALID_STMT_ID);
            Assert::IsTrue(fKB.getFollower(5) == INVALID_STMT_ID);
        }

        TEST_METHOD(getFollowing)
        {
            Assert::IsTrue(fKB.getFollowing(3) == INVALID_STMT_ID);
            Assert::IsTrue(fKB.getFollowing(-1) == INVALID_STMT_ID);
            Assert::IsTrue(fKB.getFollowing(5) == INVALID_STMT_ID);
        }

        TEST_METHOD(hasFollower)
        {
            Assert::IsTrue(fKB.hasFollower(1));
            Assert::IsTrue(fKB.hasFollower(2));
            Assert::IsFalse(fKB.hasFollower(4));
            Assert::IsFalse(fKB.hasFollower(-1));
            Assert::IsFalse(fKB.hasFollower(5));
            Assert::IsFalse(fKB.hasFollower(3));
        }

        TEST_METHOD(isFollowing)
        {
            Assert::IsFalse(fKB.isFollowing(1));
            Assert::IsTrue(fKB.isFollowing(2));
            Assert::IsTrue(fKB.isFollowing(4));
            Assert::IsFalse(fKB.isFollowing(-1));
            Assert::IsFalse(fKB.isFollowing(5));
            Assert::IsFalse(fKB.isFollowing(3));
        }

        TEST_METHOD(getAllFollowers)
        {
            Assert::IsTrue(fKB.getAllFollowers(1) == std::unordered_set<StmtId>({2, 4}));
            Assert::IsTrue(fKB.getAllFollowers(2) == std::unordered_set<StmtId>({ 4 }));
            Assert::IsTrue(fKB.getAllFollowers(4) == std::unordered_set<StmtId>());
            Assert::IsTrue(fKB.getAllFollowers(-1) == std::unordered_set<StmtId>());
            Assert::IsTrue(fKB.getAllFollowers(3) == std::unordered_set<StmtId>());
            Assert::IsTrue(fKB.getAllFollowers(5) == std::unordered_set<StmtId>());
        }

        TEST_METHOD(getAllFollowing)
        {
            Assert::IsTrue(fKB.getAllFollowing(1) == std::unordered_set<StmtId>());
            Assert::IsTrue(fKB.getAllFollowing(2) == std::unordered_set<StmtId>({ 1 }));
            Assert::IsTrue(fKB.getAllFollowing(4) == std::unordered_set<StmtId>({1, 2}));
            Assert::IsTrue(fKB.getAllFollowing(-1) == std::unordered_set<StmtId>());
            Assert::IsTrue(fKB.getAllFollowing(3) == std::unordered_set<StmtId>());
            Assert::IsTrue(fKB.getAllFollowing(5) == std::unordered_set<StmtId>());
        }
    };
}