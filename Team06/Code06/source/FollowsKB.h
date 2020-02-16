#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Types.h"

struct followsRS
{
	StmtId following = 0;
	StmtId follower = 0;
	std::unordered_set<StmtId> allFollowing;
	std::unordered_set<StmtId> allFollowers;
};

class FollowsKB
{
public:
	/*
		Adds Follows(stmtId1, stmtId2) relation to followsTable.
		Also adds Follows*(s, stmtId2) for all s in s1.allFollowed.
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
	*/
	StmtId getFollower(StmtId stmtId);

	/*
		Returns statement ID s for which Follow(s, stmtId) is true.
	*/
	StmtId getFollowing(StmtId stmtId);

	/*
		Returns all statement IDs s for which Follows*(stmtId, s) is true.
	*/
	std::unordered_set<StmtId> getAllFollowers(StmtId stmtId);

	/*
		Returns all statement IDs s for which Follows*(s, stmtId) is true.
	*/
	std::unordered_set<StmtId> getAllFollowing(StmtId stmtId);

private:
	std::unordered_map<StmtId, followsRS> followsTable;
};
