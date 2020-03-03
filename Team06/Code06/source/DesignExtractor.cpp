#include "DesignExtractor.h"

using std::unordered_set;

namespace FrontEnd {
	PKB::PKB DesignExtractor::run(PKB::PKB& pkb) {
		this->pkb = pkb;
		populateFollows();
		populateFollowStar();
		populateParent();
		populateParentStar();
		populateUses();
		populateModifies();
		populatePattern();
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
				unordered_set<StmtId> allFollowers = pkb.followsKB.getAllFollowers(followerId);
				allFollowers.insert(followerId);
				pkb.followsKB.setAllFollowers(stmtId, allFollowers);
			}
		}

		for (StmtId stmtId = 1; stmtId <= numStmts; stmtId++) {
			if (pkb.followsKB.isFollowing(stmtId)) {
				StmtId followingId = pkb.followsKB.getFollowing(stmtId);
				unordered_set<StmtId> allFollowing = pkb.followsKB.getAllFollowing(followingId);
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
				unordered_set<StmtId> allChildren;
				unordered_set<StmtId> dirChildren = pkb.parentKB.getDirectChildren(stmtId);
				for (auto& childId : dirChildren) {
					unordered_set<StmtId> indirChildren = pkb.parentKB.getAllChildren(childId);
					allChildren.insert(indirChildren.begin(), indirChildren.end());
				}
				allChildren.insert(dirChildren.begin(), dirChildren.end());
				pkb.parentKB.setAllChildren(stmtId, allChildren);
			}
		}

		for (StmtId stmtId = 1; stmtId <= numStmts; stmtId++) {
			if (pkb.parentKB.hasParent(stmtId)) {
				StmtId parentId = pkb.parentKB.getParent(stmtId);
				unordered_set<StmtId> parents = pkb.parentKB.getAllParents(parentId);
				parents.insert(parentId);
				pkb.parentKB.setAllParents(stmtId, parents);
			}
		}
	}

	void DesignExtractor::populateCallStar() {
		int numProc = pkb.procTable.size();
		
		std::vector<ProcId> visited(numProc + 1);
		std::unordered_set<ProcId> roots = pkb.callsKB.getRoots();
		for (const auto& root : roots) {
			dfsFromRoot(root, visited);
		}

		std::vector<ProcId> visited(numProc + 1);
		std::unordered_set<ProcId> leaves = pkb.callsKB.getLeaves();
		for (const auto& leaf : leaves) {
			dfsFromLeaf(leaf, visited);
		}
	}

	void DesignExtractor::populateUses() {
		for (ProcId i = 1; i <= pkb.procTable.size(); i++) {
			Procedure p = pkb.procTable.get(i);
			StmtListId sid = p.getStmtLstId();
			populateProcUsesKB(i, getAllUses(sid));
		}
	}

	void DesignExtractor::populateStmtUses(StmtId id) {
		Statement* s = pkb.stmtTable.get(id);
		StmtType type = s->getType();
		if (type == StmtType::PRINT) {
			PrintStmt* ps = (PrintStmt*)s;
			pkb.usesKB.addStmtUses(id, ps->getVar());
		}
		else if (type == StmtType::WHILE) {
			WhileStmt* ws = (WhileStmt*)s;
			StmtListId stmtLstId = ws->getStmtLstId();
			populateStmtUsesKB(id, ws->getCondExpr().getVarIds());
			populateStmtUsesKB(id, getAllUses(stmtLstId));
		}
		else if (type == StmtType::IF) {
			IfStmt* ifs = (IfStmt*)s;
			StmtListId stmtLstId1 = ifs->getThenStmtLstId();
			StmtListId stmtLstId2 = ifs->getElseStmtLstId();
			populateStmtUsesKB(id, ifs->getCondExpr().getVarIds());
			populateStmtUsesKB(id, getAllUses(stmtLstId1));
			populateStmtUsesKB(id, getAllUses(stmtLstId2));
		}
		else if (type == StmtType::ASSIGN) {
			AssignStmt* as = (AssignStmt*)s;
			Expression exp = as->getExpr();
			populateStmtUsesKB(id, exp.getVarIds());
		}
	}

	unordered_set<VarId> DesignExtractor::getAllUses(StmtListId sid) {
		unordered_set<VarId> result;
		StatementList sl = pkb.stmtListTable.get(sid);
		std::vector<StmtId> idList = sl.getStmtIds();
		for (StmtId id : idList) {
			populateStmtUses(id);
			unordered_set<VarId> set = pkb.usesKB.getAllVarsUsedByStmt(id);
			result.insert(set.begin(), set.end());
		}
		return result;
	}

	void DesignExtractor::populateStmtUsesKB(StmtId stmtId, unordered_set<VarId>& varSet) {
		for (VarId id : varSet) {
			pkb.usesKB.addStmtUses(stmtId, id);
		}
	}

	void DesignExtractor::populateProcUsesKB(ProcId procId, unordered_set<VarId>& varSet) {
		for (VarId id : varSet) {
			pkb.usesKB.addProcUses(procId, id);
		}
	}

	void DesignExtractor::populateModifies() {
		for (ProcId i = 1; i <= pkb.procTable.size(); i++) {
			Procedure p = pkb.procTable.get(i);
			StmtListId sid = p.getStmtLstId();
			populateProcModifiesKB(i, getAllModifies(sid));
		}
	}

	void DesignExtractor::populateStmtModifies(StmtId id) {
		Statement* s = pkb.stmtTable.get(id);
		StmtType type = s->getType();
		if (type == StmtType::READ) {
			ReadStmt* rs = (ReadStmt*)s;
			pkb.modifiesKB.addStmtModifies(id, rs->getVar());
		}
		else if (type == StmtType::WHILE) {
			WhileStmt* ws = (WhileStmt*)s;
			StmtListId stmtLstId = ws->getStmtLstId();
			populateStmtModifiesKB(id, getAllModifies(stmtLstId));
		}
		else if (type == StmtType::IF) {
			IfStmt* ifs = (IfStmt*)s;
			StmtListId stmtLstId1 = ifs->getThenStmtLstId();
			StmtListId stmtLstId2 = ifs->getElseStmtLstId();
			populateStmtModifiesKB(id, getAllModifies(stmtLstId1));
			populateStmtModifiesKB(id, getAllModifies(stmtLstId2));
		}
		else if (type == StmtType::ASSIGN) {
			AssignStmt* as = (AssignStmt*)s;
			Expression exp = as->getExpr();
			pkb.modifiesKB.addStmtModifies(id, as->getVar());
		}
	}

	unordered_set<VarId> DesignExtractor::getAllModifies(StmtListId sid) {
		unordered_set<VarId> result;
		StatementList sl = pkb.stmtListTable.get(sid);
		std::vector<StmtId> idList = sl.getStmtIds();
		for (StmtId id : idList) {
			populateStmtModifies(id);
			unordered_set<VarId> set = pkb.modifiesKB.getAllVarsModifiedByStmt(id);
			result.insert(set.begin(), set.end());
		}
		return result;
	}

	void DesignExtractor::populateStmtModifiesKB(StmtId stmtId, unordered_set<VarId>& varSet) {
		for (VarId id : varSet) {
			pkb.modifiesKB.addStmtModifies(stmtId, id);
		}
	}

	void DesignExtractor::populateProcModifiesKB(ProcId procId, unordered_set<VarId>& varSet) {
		for (VarId id : varSet) {
			pkb.modifiesKB.addProcModifies(procId, id);
		}
	}

	void DesignExtractor::populatePattern() {
		unordered_set<StmtId> assignSet = pkb.stmtTable.getStmtsByType(StmtType::ASSIGN);
		for (StmtId id : assignSet) {
			AssignStmt* as = (AssignStmt*)pkb.stmtTable.get(id);
			Expression exp = as->getExpr();
			populatePatternKB(id, exp);
		}
	}

	void DesignExtractor::populatePatternKB(StmtId stmtId, Expression exp) {
		pkb.patternKB.addAssignPattern(exp.getStr(), stmtId);
		unordered_set<Pattern> patterns = exp.getPatterns();
		for (Pattern p : patterns) {
			pkb.patternKB.addAssignPattern(p, stmtId);
		}
	}

	void DesignExtractor::dfsFromRoot(ProcId root, std::vector<ProcId>& visited) {
		if (visited[root] == -1) {
			throw std::domain_error("Cycle Detected");
		}
		
		visited[root] = -1;

		for (const auto& dirCallee : pkb.callsKB.getDirectCallees(root)) {
			if (visited[dirCallee] == 0) {
				dfsFromRoot(dirCallee, visited);
			}
			pkb.callsKB.setAllCallees(root, pkb.callsKB.getAllCallees(dirCallee));
		}

		visited[root] = 1;
	}

	void DesignExtractor::dfsFromLeaf(ProcId leaf, std::vector<ProcId>& visited) {
		if (visited[leaf] == -1) {
			throw std::domain_error("Cycle Detected");
		}

		visited[leaf] = -1;

		for (const auto& dirCaller : pkb.callsKB.getDirectCallers(leaf)) {
			if (visited[dirCaller] == 0) {
				dfsFromRoot(dirCaller, visited);
			}
			pkb.callsKB.setAllCallers(leaf, pkb.callsKB.getAllCallers(dirCaller));
		}

		visited[leaf] = 1;
	}
}
