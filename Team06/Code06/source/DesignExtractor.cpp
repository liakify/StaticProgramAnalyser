#include "DesignExtractor.h"

using std::unordered_set;

namespace FrontEnd {
    PKB::PKB DesignExtractor::run(PKB::PKB& pkb) {
        this->pkb = pkb;
        populateCalls();
        populateCallStar();
        populateFollows();
        populateFollowStar();
        populateParent();
        populateParentStar();
        populateUses();
        populateModifies();
        populatePattern();
        populateNext();
        return this->pkb;
    }

    void DesignExtractor::populateCalls() {
        for (ProcId i = 1; i <= pkb.procTable.size(); i++) {
            StmtListId slid = pkb.procTable.get(i).getStmtLstId();
            std::vector<StmtId> idList = pkb.stmtListTable.get(slid).getStmtIds();
            for (StmtId id : idList) {
                Statement* s = pkb.stmtTable.get(id);
                if (s->getType() != StmtType::CALL) {
                    continue;
                }
                CallStmt* cs = reinterpret_cast<CallStmt*>(s);
                ProcId calledId = pkb.procTable.getProcId(cs->getProc());
                if (calledId == -1) {  // ProcId not found
                    throw std::invalid_argument("Invalid procedure call detected");
                }
                pkb.callsKB.addCalls(i, calledId);
            }
        }
    }

    void DesignExtractor::populateCallStar() {
        int numProc = pkb.procTable.size();

        /*
            0 indicates unvisited,
            -1 indicates visited in current dfs call path,
            1 indicates visited and fully processed
        */
        std::vector<ProcId> visited(numProc + 1);
        // Populate allCallees for every proc
        processCallStar(numProc, visited, NodeType::SUCCESSOR);

        std::fill(visited.begin(), visited.end(), 0);
        // Populate allCallers for every proc
        processCallStar(numProc, visited, NodeType::PREDECESSOR);
    }

    void DesignExtractor::processCallStar(int numProc, std::vector<int>& visited, NodeType type) {
        for (int p = 1; p <= numProc; p++) {
            if (visited[p] == 0) {
                callStarDFS(p, visited, type);
            }
        }
    }

    void DesignExtractor::callStarDFS(ProcId root, std::vector<ProcId>& visited, NodeType type) {
        visited[root] = -1;

        std::unordered_set<ProcId> directSet = pkb.callsKB.getDirectNodes(root, type);

        for (const auto& dirNode : directSet) {
            if (visited[dirNode] == -1) {
                throw std::invalid_argument("Cycle Detected");
            }
            if (visited[dirNode] == 0) {
                callStarDFS(dirNode, visited, type);
            }

            pkb.callsKB.addToAll(root, dirNode, type);
            pkb.callsKB.addToAll(root, pkb.callsKB.getAllNodes(dirNode, type), type);
        }

        visited[root] = 1;
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
            WhileStmt* ws = reinterpret_cast<WhileStmt*>(pkb.stmtTable.get(id));
            populateParentKB(id, ws->getStmtLstId());
        }
        unordered_set<StmtId> ifSet = pkb.stmtTable.getStmtsByType(StmtType::IF);
        for (StmtId id : ifSet) {
            IfStmt* ifs = reinterpret_cast<IfStmt*>(pkb.stmtTable.get(id));
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
            PrintStmt* ps = reinterpret_cast<PrintStmt*>(s);
            pkb.usesKB.addStmtUses(id, ps->getVar());
        } else if (type == StmtType::WHILE) {
            WhileStmt* ws = reinterpret_cast<WhileStmt*>(s);
            StmtListId stmtLstId = ws->getStmtLstId();
            populateStmtUsesKB(id, ws->getCondExpr().getVarIds());
            populateStmtUsesKB(id, getAllUses(stmtLstId));
        } else if (type == StmtType::IF) {
            IfStmt* ifs = reinterpret_cast<IfStmt*>(s);
            StmtListId stmtLstId1 = ifs->getThenStmtLstId();
            StmtListId stmtLstId2 = ifs->getElseStmtLstId();
            populateStmtUsesKB(id, ifs->getCondExpr().getVarIds());
            populateStmtUsesKB(id, getAllUses(stmtLstId1));
            populateStmtUsesKB(id, getAllUses(stmtLstId2));
        } else if (type == StmtType::ASSIGN) {
            AssignStmt* as = reinterpret_cast<AssignStmt*>(s);
            Expression exp = as->getExpr();
            populateStmtUsesKB(id, exp.getVarIds());
        } else if (type == StmtType::CALL) {
            CallStmt* cs = reinterpret_cast<CallStmt*>(s);
            ProcId pid = pkb.procTable.getProcId(cs->getProc());
            StmtListId stmtLstId = pkb.procTable.get(pid).getStmtLstId();

            // Depending on the DAG of the procedure calls, can potentially be very inefficient
            // Possible to optimize performance of this by using DP
            // i.e. cache result of getAllUses(StmtLstId)
            populateStmtUsesKB(id, getAllUses(stmtLstId));
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
            ReadStmt* rs = reinterpret_cast<ReadStmt*>(s);
            pkb.modifiesKB.addStmtModifies(id, rs->getVar());
        } else if (type == StmtType::WHILE) {
            WhileStmt* ws = reinterpret_cast<WhileStmt*>(s);
            StmtListId stmtLstId = ws->getStmtLstId();
            populateStmtModifiesKB(id, getAllModifies(stmtLstId));
        } else if (type == StmtType::IF) {
            IfStmt* ifs = reinterpret_cast<IfStmt*>(s);
            StmtListId stmtLstId1 = ifs->getThenStmtLstId();
            StmtListId stmtLstId2 = ifs->getElseStmtLstId();
            populateStmtModifiesKB(id, getAllModifies(stmtLstId1));
            populateStmtModifiesKB(id, getAllModifies(stmtLstId2));
        } else if (type == StmtType::ASSIGN) {
            AssignStmt* as = reinterpret_cast<AssignStmt*>(s);
            Expression exp = as->getExpr();
            pkb.modifiesKB.addStmtModifies(id, as->getVar());
        } else if (type == StmtType::CALL) {
            CallStmt* cs = reinterpret_cast<CallStmt*>(s);
            ProcId pid = pkb.procTable.getProcId(cs->getProc());
            StmtListId stmtLstId = pkb.procTable.get(pid).getStmtLstId();

            // Depending on the DAG of the procedure calls, can potentially be very inefficient
            // Possible to optimize performance of this by using DP
            // i.e. cache result of getAllModifies(StmtLstId)
            populateStmtModifiesKB(id, getAllModifies(stmtLstId));
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
            AssignStmt* as = reinterpret_cast<AssignStmt*>(pkb.stmtTable.get(id));
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

    void DesignExtractor::populateNext() {
        for (ProcId pid = 1; pid < pkb.procTable.size(); pid++) {
            populateNextKB(pkb.procTable.get(pid).getStmtLstId());
        }
    }

    void DesignExtractor::populateNextKB(StmtListId sid) {
        StatementList sl = pkb.stmtListTable.get(sid);
        std::vector<StmtId> idList = sl.getStmtIds();
        for (int i = 0; i < idList.size(); i++) {
            Statement* s = pkb.stmtTable.get(idList[i]);
            if (s->getType() == StmtType::IF) {
                IfStmt* ifs = reinterpret_cast<IfStmt*>(s);
                StmtListId thenId = ifs->getThenStmtLstId();
                StatementList& thenSl = pkb.stmtListTable.get(thenId);
                updateLastStmtId(thenSl);
                populateNextKB(thenId);
                // pkb.nextKB.addNext(idList[i], thenSl.getFirst());
                StmtListId elseId = ifs->getElseStmtLstId();
                StatementList& elseSl = pkb.stmtListTable.get(elseId);
                updateLastStmtId(elseSl);
                populateNextKB(elseId);
                // pkb.nextKB.addNext(idList[i], elseSl.getFirst());
                if (i < idList.size() - 1) {
                    // pkb.nextKB.addNext(thenSl.getLast(), idList[i+1]);
                    // pkb.nextKB.addNext(elseSl.getLast(), idList[i+1]);
                }
            } else if (s->getType() == StmtType::WHILE) {
                WhileStmt* ws = reinterpret_cast<WhileStmt*>(s);
                StmtListId loopId = ws->getStmtLstId();
                StatementList& whileSl = pkb.stmtListTable.get(loopId);
                updateLastStmtId(whileSl);
                populateNextKB(loopId);
                // pkb.nextKB.addNext(idList[i], whileSl.getFirst());
                // pkb.nextKB.addNext(whileSl.getLast(), idList[i]);
                if (i < idList.size() - 1) {
                    // pkb.nextKB.addNext(idList[i], idList[i+1]);
                }
            } else {
                if (i < idList.size() - 1) {
                    // pkb.nextKB.addNext(idList[i], idList[i+1]);
                }
            }
        }
    }

    void DesignExtractor::updateLastStmtId(StatementList& sl) {
        Statement* lastStmt = pkb.stmtTable.get(sl.getLast());
        if (lastStmt->getType() == StmtType::IF) {
            IfStmt* ifs = reinterpret_cast<IfStmt*>(lastStmt);
            StatementList& newSl = pkb.stmtListTable.get(ifs->getElseStmtLstId());
            updateLastStmtId(newSl);
            sl.setLast(newSl.getLast());
        } else if (lastStmt->getType() == StmtType::WHILE) {
            WhileStmt* ws = reinterpret_cast<WhileStmt*>(lastStmt);
            StatementList& newSl = pkb.stmtListTable.get(ws->getStmtLstId());
            updateLastStmtId(newSl);
            sl.setLast(newSl.getLast());
        }
    }
}
