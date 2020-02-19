#include "DesignExtractor.h"

namespace FrontEnd {
	void DesignExtractor::run(PKB::PKB& pkb) {
		this->pkb = pkb;
		populateFollowStar();
		populateParentStar();
	}

	void DesignExtractor::populateFollowStar() {
		int numStmts = pkb.stmtTable.size();

		for (StmtId stmtId = numStmts; stmtId > 0; stmtId--) {
			if (pkb.followsKB.hasFollower(stmtId)) {
				StmtId followerId = pkb.followsKB.getFollower(stmtId);
				std::unordered_set<StmtId> allFollowers = pkb.followsKB.getAllFollowers(followerId);
				allFollowers.insert(followerId);
				pkb.followsKB.setAllFollowers(stmtId, allFollowers);
			}
		}

		for (StmtId stmtId = 1; stmtId <= numStmts; stmtId++) {
			if (pkb.followsKB.isFollowing(stmtId)) {
				StmtId followingId = pkb.followsKB.getFollowing(stmtId);
				std::unordered_set<StmtId> allFollowing = pkb.followsKB.getAllFollowing(followingId);
				allFollowing.insert(followingId);
				pkb.followsKB.setAllFollowing(stmtId, allFollowing);
			}
		}
	}

	void DesignExtractor::populateParentStar() {
		int numStmts = pkb.stmtTable.size();

		for (StmtId stmtId = numStmts; stmtId > 0; stmtId--) {
			if (pkb.parentKB.hasDirectChildren(stmtId)) {
				std::unordered_set<StmtId> allChildren;
				std::unordered_set<StmtId> dirChildren = pkb.parentKB.getDirectChildren(stmtId);
				for (auto& childId : dirChildren) {
					std::unordered_set<StmtId> indirChildren = pkb.parentKB.getAllChildren(childId);
					allChildren.insert(indirChildren.begin(), indirChildren.end());
				}
				allChildren.insert(dirChildren.begin(), dirChildren.end());
				pkb.parentKB.setAllChildren(stmtId, allChildren);
			}
		}

		for (StmtId stmtId = 1; stmtId <= numStmts; stmtId++) {
			if (pkb.parentKB.hasParent(stmtId)) {
				StmtId parentId = pkb.parentKB.getParent(stmtId);
				std::unordered_set<StmtId> parents = pkb.parentKB.getAllParents(parentId);
				parents.insert(parentId);
				pkb.parentKB.setAllParents(stmtId, parents);
			}
		}
	}
}