#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Types.h"
#include "Constants.h"

namespace PKB {

    struct followsRS {
        StmtId following = 0;
        StmtId follower = 0;
        std::unordered_set<StmtId> allFollowing;
        std::unordered_set<StmtId> allFollowers;
    };

    class FollowsKB {
     public:
        /*
            Adds Follows(stmtId1, stmtId2) relation to followsTable.
        */
        void addFollows(StmtId stmtId1, StmtId stmtId2);

        /*
            Returns TRUE if Follows(stmtId1, stmtId2) is true, FALSE otherwise.
        */
        bool follows(StmtId stmtId1, StmtId stmtId2);

        /*
            Returns TRUE if Follows*(stmtId1, stmtId2) is true, FALSE otherwise.
        */
        bool followStar(StmtId stmtId1, StmtId stmtId2);

        /*
            Returns statement ID s for which Follow(stmtId, s) is true.
            If stmtId is not found or stmtId has no follower, INVALID_STMT_ID 0 is returned.
        */
        StmtId getFollower(StmtId stmtId);

        /*
            Returns statement ID s for which Follow(s, stmtId) is true.
            If stmtId is not found or stmtId is not following another statement, NVALID_STMT_ID 0 is returned.
        */
        StmtId getFollowing(StmtId stmtId);

        /*
            Returns TRUE if stmtId has a follower, FALSE otherwise.
        */
        bool hasFollower(StmtId stmtId);

        /*
            Return TRUE if stmtId is following another statement, FALSE otherwise.
        */
        bool isFollowing(StmtId stmtId);

        /*
            Returns all statement IDs s for which Follows*(stmtId, s) is true.
        */
        const std::unordered_set<StmtId>& getAllFollowers(StmtId stmtId);

        /*
            Returns all statement IDs s for which Follows*(s, stmtId) is true.
        */
        const std::unordered_set<StmtId>& getAllFollowing(StmtId stmtId);

        /*
            Sets allFollowers of stmtId to followers.
        */
        void setAllFollowers(StmtId stmtId, const std::unordered_set<StmtId>& followers);

        /*
            Sets allFollowing of stmtId to following.
        */
        void setAllFollowing(StmtId stmtId, const std::unordered_set<StmtId>& following);

        /*
            Returns TRUE if any Follows relation is present, FALSE otherwise.
        */
        bool hasFollowsRelation();

     private:
        std::unordered_map<StmtId, followsRS> followsTable;
    };
}