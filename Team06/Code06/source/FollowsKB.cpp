#include "FollowsKB.h"

/*
	TODO: Exception handling for invalid inputs
*/

FollowsKB::FollowsKB()
{

}

/*
	TODO: Move add Follows* logic out to Design Extractor
*/
void FollowsKB::addFollows(int stmtId1, int stmtId2)
{
	if (followsTable.find(stmtId1) == followsTable.end())
	{
		followsTable.insert(std::make_pair(stmtId1, followsRS()));
	}
	
	if (followsTable.find(stmtId2) == followsTable.end())
	{
		followsTable.insert(std::make_pair(stmtId2, followsRS()));
	}

	followsRS fRS1 = followsTable[stmtId1];
	followsRS fRS2 = followsTable[stmtId2];

	fRS1.follower = stmtId2;
	fRS2.following = stmtId1;

	for (auto following : fRS1.allFollowing)
	{
		followsTable[following].allFollowers.insert(stmtId2);
	}

	fRS2.allFollowing = fRS1.allFollowing;
	fRS2.allFollowing.insert(stmtId1);
}

bool FollowsKB::follows(int stmtId1, int stmtId2)
{
	return followsTable[stmtId1].follower == stmtId2;
}

bool FollowsKB::followStar(int stmtId1, int stmtId2)
{
	followsRS fRS1 = followsTable[stmtId1];
	std::unordered_set<int> stmt1Followers = fRS1.allFollowers;
	return stmt1Followers.find(stmtId2) != stmt1Followers.end();
}

int FollowsKB::getFollower(int stmtId)
{
	return followsTable[stmtId].follower;
}

int FollowsKB::getFollowing(int stmtId)
{
	return followsTable[stmtId].following;
}

std::unordered_set<int> FollowsKB::getAllFollowers(int stmtId)
{
	return followsTable[stmtId].allFollowers;
}

std::unordered_set<int> FollowsKB::getAllFollowing(int stmtId)
{
	return followsTable[stmtId].allFollowing;
}
