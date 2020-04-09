#include "FollowsKB.h"
#include <stdexcept>

namespace PKB {

    void FollowsKB::addFollows(StmtId stmtId1, StmtId stmtId2) {
        followsRS& fRS1 = followsTable[stmtId1];
        followsRS& fRS2 = followsTable[stmtId2];

        fRS1.follower = stmtId2;
        fRS2.following = stmtId1;
    }

    bool FollowsKB::follows(StmtId stmtId1, StmtId stmtId2) {
        try {
            return followsTable.at(stmtId1).follower == stmtId2;
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    bool FollowsKB::followStar(StmtId stmtId1, StmtId stmtId2) {
        try {
            std::unordered_set<StmtId> stmt1Followers = followsTable.at(stmtId1).allFollowers;
            return stmt1Followers.find(stmtId2) != stmt1Followers.end();
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    StmtId FollowsKB::getFollower(StmtId stmtId) {
        try {
            return followsTable.at(stmtId).follower;
        } catch (const std::out_of_range&) {
            return INVALID_STMT_ID;
        }
    }

    StmtId FollowsKB::getFollowing(StmtId stmtId) {
        try {
            return followsTable.at(stmtId).following;
        } catch (const std::out_of_range&) {
            return INVALID_STMT_ID;
        }
    }

    bool FollowsKB::hasFollower(StmtId stmtId) {
        try {
            return followsTable.at(stmtId).follower != 0;
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    bool FollowsKB::isFollowing(StmtId stmtId) {
        try {
            return followsTable.at(stmtId).following != 0;
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    const std::unordered_set<StmtId>& FollowsKB::getAllFollowers(StmtId stmtId) {
        try {
            return followsTable.at(stmtId).allFollowers;
        } catch (const std::out_of_range&) {
            return EMPTY_RESULT;
        }
    }

    const std::unordered_set<StmtId>& FollowsKB::getAllFollowing(StmtId stmtId) {
        try {
            return followsTable.at(stmtId).allFollowing;
        } catch (const std::out_of_range&) {
            return EMPTY_RESULT;
        }
    }

    void FollowsKB::setAllFollowers(StmtId stmtId, const std::unordered_set<StmtId>& followers) {
        try {
            followsTable.at(stmtId).allFollowers.insert(followers.begin(), followers.end());
        } catch (const std::out_of_range&) {
            return;
        }
    }

    void FollowsKB::setAllFollowing(StmtId stmtId, const std::unordered_set<StmtId>& following) {
        try {
            followsTable.at(stmtId).allFollowing.insert(following.begin(), following.end());
        } catch (const std::out_of_range&) {
            return;
        }
    }

    bool FollowsKB::hasFollowsRelation() {
        return followsTable.size() != 0;
    }
}


