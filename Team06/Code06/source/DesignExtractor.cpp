#include "DesignExtractor.h"

using std::unordered_set;

namespace FrontEnd {
	PKB::PKB DesignExtractor::run(PKB::PKB& pkb) {
		this->pkb = pkb;
		populateFollows();
		populateFollowStar();
		populateParent();
		populateParentStar();
		return this->pkb;
	}

	void DesignExtractor::populateFollows() {
		for (int i = 1; i <= pkb.stmtListTable.size(); i++) {
			StatementList sl = pkb.stmtListTable.get(i);
			std::vector<StmtId> sid = sl.getStmtIds();
			for (size_t j = 0; j < sid.size() - 1; j++) {
				pkb.followsKB.addFollows(sid[j], sid[j + 1]);
			}
		}
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

	void DesignExtractor::populateParent() {
		unordered_set<StmtId> whileSet = pkb.stmtTable.getStmtsByType(StmtType::WHILE);
		for (StmtId id : whileSet) {
			WhileStmt* ws = (WhileStmt*)pkb.stmtTable.get(id);
			populateParentKB(id, ws->getStmtLstId());
		}
		unordered_set<StmtId> ifSet = pkb.stmtTable.getStmtsByType(StmtType::IF);
		for (StmtId id : ifSet) {
			IfStmt* ifs = (IfStmt*)pkb.stmtTable.get(id);
			populateParentKB(id, ifs->getThenStmtLstId());
			populateParentKB(id, ifs->getElseStmtLstId());
		}
	}

	void DesignExtractor::populateParentKB(StmtId stmtId, StmtListId stmtLstId) {
		StatementList sl = pkb.stmtListTable.get(stmtLstId);
		std::vector<StmtId> idList = sl.getStmtIds();
		for (StmtId id : idList) {
			pkb.parentKB.addParent(stmtId, id);
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

	void DesignExtractor::populateUses() {
		for (StmtId i = 1; i < pkb.stmtTable.size(); i++) {
			Statement* s = pkb.stmtTable.get(i);
			StmtType type = s->getType();
			if (type == StmtType::PRINT) {
				PrintStmt* ps = (PrintStmt*)s;
				pkb.usesKB.addStmtUses(i, ps->getVar());
			}
			else if (type == StmtType::WHILE) {
				WhileStmt* ws = (WhileStmt*)s;
				StmtListId stmtLstId = ws->getStmtLstId();
				populateUsesKB(i, ws->getCondExpr().getVarIds());
				populateUsesKB(i, getAllUses(stmtLstId));
			}
			else if (type == StmtType::IF) {
				IfStmt* ifs = (IfStmt*)s;
				StmtListId stmtLstId1 = ifs->getThenStmtLstId();
				StmtListId stmtLstId2 = ifs->getElseStmtLstId();
				populateUsesKB(i, ifs->getCondExpr().getVarIds());
				populateUsesKB(i, getAllUses(stmtLstId1));
				populateUsesKB(i, getAllUses(stmtLstId2));
			}
			else if (type == StmtType::ASSIGN) {
				AssignStmt* as = (AssignStmt*)s;
				Expression exp = as->getExpr();
				populateUsesKB(i, exp.getVarIds());
			}
		}
	}

	unordered_set<VarId> DesignExtractor::getAllUses(StmtListId sid) {
		std::unordered_set<VarId> result;
		StatementList sl = pkb.stmtListTable.get(sid);
		std::vector<StmtId> idList = sl.getStmtIds();
		for (StmtId id : idList) {
			std::unordered_set<VarId> set = pkb.usesKB.getAllVarsUsedByStmt(id);
			result.insert(set.begin(), set.end());
		}
		return result;
	}

	void DesignExtractor::populateUsesKB(StmtId stmtId, std::unordered_set<VarId> varSet) {
		for (VarId id : varSet) {
			pkb.usesKB.addStmtUses(stmtId, id);
		}
	}

	void DesignExtractor::populateModifies() {
		for (StmtId i = 1; i < pkb.stmtTable.size(); i++) {
			Statement* s = pkb.stmtTable.get(i);
			StmtType type = s->getType();
			if (type == StmtType::READ) {
				ReadStmt* rs = (ReadStmt*)s;
				pkb.modifiesKB.addStmtModifies(i, rs->getVar());
			}
			else if (type == StmtType::WHILE) {
				WhileStmt* ws = (WhileStmt*)s;
				StmtListId stmtLstId = ws->getStmtLstId();
				populateModifiesKB(i, getAllModifies(stmtLstId));
			}
			else if (type == StmtType::IF) {
				IfStmt* ifs = (IfStmt*)s;
				StmtListId stmtLstId1 = ifs->getThenStmtLstId();
				StmtListId stmtLstId2 = ifs->getElseStmtLstId();
				populateModifiesKB(i, getAllModifies(stmtLstId1));
				populateModifiesKB(i, getAllModifies(stmtLstId2));
			}
			else if (type == StmtType::ASSIGN) {
				AssignStmt* as = (AssignStmt*)s;
				Expression exp = as->getExpr();
				pkb.modifiesKB.addStmtModifies(i, as->getVar());
			}
		}
	}

	std::unordered_set<VarId> DesignExtractor::getAllModifies(StmtListId sid) {
		std::unordered_set<VarId> result;
		StatementList sl = pkb.stmtListTable.get(sid);
		std::vector<StmtId> idList = sl.getStmtIds();
		for (StmtId id : idList) {
			std::unordered_set<VarId> set = pkb.modifiesKB.getAllVarsModifiedByStmt(id);
			result.insert(set.begin(), set.end());
		}
		return result;
	}

	void DesignExtractor::populateModifiesKB(StmtId stmtId, std::unordered_set<VarId> varSet) {
		for (VarId id : varSet) {
			pkb.modifiesKB.addStmtModifies(stmtId, id);
		}
	}

	void DesignExtractor::populatePattern() {
		unordered_set<StmtId> assignSet = pkb.stmtTable.getStmtsByType(StmtType::ASSIGN);
		for (StmtId id : assignSet) {
			AssignStmt* as = (AssignStmt*)pkb.stmtTable.get(id);
			Expression exp = as->getExpr();
			populatePatternKB(id, exp);
			VarName vn = pkb.varTable.get(as->getVar());
			pkb.patternKB.addLHSPattern(vn, id);
		}
	}

	void DesignExtractor::populatePatternKB(StmtId stmtId, Expression exp) {
		pkb.patternKB.addRHSPattern(exp.getStr(), stmtId);
		std::unordered_set<Pattern> patterns = exp.getPatterns();
		for (Pattern p : patterns) {
			pkb.patternKB.addRHSPattern(p, stmtId);
		}
	}
}