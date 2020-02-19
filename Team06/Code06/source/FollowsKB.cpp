#include "FollowsKB.h"

/*
	TODO: Exception handling for invalid inputs
*/

void FollowsKB::addFollows(StmtId stmtId1, StmtId stmtId2)
{
	if (followsTable.find(stmtId1) == followsTable.end())
	{
		followsTable.insert(std::make_pair(stmtId1, followsRS()));
	}

	if (followsTable.find(stmtId2) == followsTable.end())
	{
		followsTable.insert(std::make_pair(stmtId2, followsRS()));
	}

	followsRS& fRS1 = followsTable[stmtId1];
	followsRS& fRS2 = followsTable[stmtId2];

	fRS1.follower = stmtId2;
	fRS2.following = stmtId1;
}

bool FollowsKB::follows(StmtId stmtId1, StmtId stmtId2)
{
	return followsTable[stmtId1].follower == stmtId2;
}

bool FollowsKB::followStar(StmtId stmtId1, StmtId stmtId2)
{
	std::unordered_set<StmtId> stmt1Followers = followsTable[stmtId1].allFollowers;
	return stmt1Followers.find(stmtId2) != stmt1Followers.end();
}

StmtId FollowsKB::getFollower(StmtId stmtId)
{
	return followsTable[stmtId].follower;
}

StmtId FollowsKB::getFollowing(StmtId stmtId)
{
	return followsTable[stmtId].following;
}

bool FollowsKB::hasFollower(StmtId stmtId)
{
	return followsTable[stmtId].follower != 0;
}

bool FollowsKB::isFollowing(StmtId stmtId)
{
	return followsTable[stmtId].following != 0;
}

std::unordered_set<StmtId> FollowsKB::getAllFollowers(StmtId stmtId)
{
	return followsTable[stmtId].allFollowers;
}

std::unordered_set<StmtId> FollowsKB::getAllFollowing(StmtId stmtId)
{
	return followsTable[stmtId].allFollowing;
}

void FollowsKB::setAllFollowers(StmtId stmtId, std::unordered_set<StmtId> followers)
{
	followsTable[stmtId].allFollowers = followers;
}

void FollowsKB::setAllFollowing(StmtId stmtId, std::unordered_set<StmtId> following)
{
	followsTable[stmtId].allFollowing = following;
}
