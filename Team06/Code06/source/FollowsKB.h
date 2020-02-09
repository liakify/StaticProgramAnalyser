#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

struct followsRS
{
	int following = 0;
	int follower = 0;
	std::unordered_set<int> allFollowing;
	std::unordered_set<int> allFollowers;
};

class FollowsKB 
{
public:
	FollowsKB();

	/*
		Adds Follows(stmtId1, stmtId2) relation to followsTable.
		Also adds Follows*(s, stmtId2) for all s in s1.allFollowed.
	*/
	void addFollows(int stmtId1, int stmtId2);

	/*
		Returns TRUE if Follows(stmtId1, stmtId2) is true, FALSE otherwise.
	*/
	bool follows(int stmtId1, int stmtId2);

	/*
		Returns TRUE if Follows*(stmtId1, stmtId2) is true, FALSE otherwise.
	*/
	bool followStar(int stmtId1, int stmtId2);

	/*
		Returns statement ID s for which Follow(stmtId, s) is true.
	*/
	int getFollower(int stmtId);

	/*
		Returns statement ID s for which Follow(s, stmtId) is true.
	*/
	int getFollowing(int stmtId);

	/*
		Returns all statement IDs s for which Follows*(stmtId, s) is true.
	*/
	std::unordered_set<int> getAllFollowers(int stmtId);

	/*
		Returns all statement IDs s for which Follows*(s, stmtId) is true.
	*/
	std::unordered_set<int> getAllFollowing(int stmtId);

private:
	static std::unordered_map<int, followsRS> followsTable;
};
