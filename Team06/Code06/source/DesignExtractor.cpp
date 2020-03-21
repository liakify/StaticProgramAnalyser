#include <memory>
#include <stdexcept>

#include "DesignExtractor.h"

using std::unordered_set;
using std::shared_ptr;

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
        updateStmtContainerId();
        return this->pkb;
    }

    void DesignExtractor::populateCalls() {
        for (ProcId i = 1; i <= pkb.procTable.size(); i++) {
            StmtListId sid = pkb.procTable.get(i).getStmtLstId();
            for (ProcId p : getAllCalls(sid)) {
                pkb.callsKB.addCalls(i, p);
            }
        }
    }

    unordered_set<ProcId> DesignExtractor::getAllCalls(StmtListId sid) {
        std::vector<StmtId> idList = pkb.stmtListTable.get(sid).getStmtIds();
        unordered_set<ProcId> result = unordered_set<ProcId>();
        for (StmtId id : idList) {
            std::shared_ptr<Statement>& s = pkb.stmtTable.get(id);
            if (s->getType() == StmtType::CALL) {
                CallStmt* cs = dynamic_cast<CallStmt*>(s.get());
                ProcId calledId = pkb.procTable.getProcId(cs->getProc());
                if (calledId == -1) {  // ProcId not found
                    throw std::invalid_argument("Invalid procedure call detected");
                }
                result.insert(calledId);
            } else if (s->getType() == StmtType::IF) {
                IfStmt* ifs = dynamic_cast<IfStmt*>(s.get());
                unordered_set<ProcId> thenSet = getAllCalls(ifs->getThenStmtLstId());
                for (ProcId pid : thenSet) {
                    result.insert(pid);
                }
                unordered_set<ProcId> elseSet = getAllCalls(ifs->getElseStmtLstId());
                for (ProcId pid : elseSet) {
                    result.insert(pid);
                }
            } else if (s->getType() == StmtType::WHILE) {
                WhileStmt* ws = dynamic_cast<WhileStmt*>(s.get());
                unordered_set<ProcId> pidSet = getAllCalls(ws->getStmtLstId());
                for (ProcId pid : pidSet) {
                    result.insert(pid);
                }
            }
        }
        return result;
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
            WhileStmt* ws = dynamic_cast<WhileStmt*>(pkb.stmtTable.get(id).get());
            populateParentKB(id, ws->getStmtLstId());
        }
        unordered_set<StmtId> ifSet = pkb.stmtTable.getStmtsByType(StmtType::IF);
        for (StmtId id : ifSet) {
            IfStmt* ifs = dynamic_cast<IfStmt*>(pkb.stmtTable.get(id).get());
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
        shared_ptr<Statement>& s = pkb.stmtTable.get(id);
        StmtType type = s->getType();
        if (type == StmtType::PRINT) {
            PrintStmt* ps = dynamic_cast<PrintStmt*>(s.get());
            pkb.usesKB.addStmtUses(id, ps->getVar());
        } else if (type == StmtType::WHILE) {
            WhileStmt* ws = dynamic_cast<WhileStmt*>(s.get());
            StmtListId stmtLstId = ws->getStmtLstId();
            populateStmtUsesKB(id, ws->getCondExpr().getVarIds());
            populateStmtUsesKB(id, getAllUses(stmtLstId));
        } else if (type == StmtType::IF) {
            IfStmt* ifs = dynamic_cast<IfStmt*>(s.get());
            StmtListId stmtLstId1 = ifs->getThenStmtLstId();
            StmtListId stmtLstId2 = ifs->getElseStmtLstId();
            populateStmtUsesKB(id, ifs->getCondExpr().getVarIds());
            populateStmtUsesKB(id, getAllUses(stmtLstId1));
            populateStmtUsesKB(id, getAllUses(stmtLstId2));
        } else if (type == StmtType::ASSIGN) {
            AssignStmt* as = dynamic_cast<AssignStmt*>(s.get());
            Expression exp = as->getExpr();
            populateStmtUsesKB(id, exp.getVarIds());
        } else if (type == StmtType::CALL) {
            CallStmt* cs = dynamic_cast<CallStmt*>(s.get());
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
        shared_ptr<Statement>& s = pkb.stmtTable.get(id);
        StmtType type = s->getType();
        if (type == StmtType::READ) {
            ReadStmt* rs = dynamic_cast<ReadStmt*>(s.get());
            pkb.modifiesKB.addStmtModifies(id, rs->getVar());
        } else if (type == StmtType::WHILE) {
            WhileStmt* ws = dynamic_cast<WhileStmt*>(s.get());
            StmtListId stmtLstId = ws->getStmtLstId();
            populateStmtModifiesKB(id, getAllModifies(stmtLstId));
        } else if (type == StmtType::IF) {
            IfStmt* ifs = dynamic_cast<IfStmt*>(s.get());
            StmtListId stmtLstId1 = ifs->getThenStmtLstId();
            StmtListId stmtLstId2 = ifs->getElseStmtLstId();
            populateStmtModifiesKB(id, getAllModifies(stmtLstId1));
            populateStmtModifiesKB(id, getAllModifies(stmtLstId2));
        } else if (type == StmtType::ASSIGN) {
            AssignStmt* as = dynamic_cast<AssignStmt*>(s.get());
            Expression exp = as->getExpr();
            pkb.modifiesKB.addStmtModifies(id, as->getVar());
        } else if (type == StmtType::CALL) {
            CallStmt* cs = dynamic_cast<CallStmt*>(s.get());
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
            AssignStmt* as = dynamic_cast<AssignStmt*>(pkb.stmtTable.get(id).get());
            Expression exp = as->getExpr();
            populateAssignPatternKB(id, exp);
        }
        unordered_set<StmtId> ifSet = pkb.stmtTable.getStmtsByType(StmtType::IF);
        for (StmtId id : ifSet) {
            IfStmt* ifs = dynamic_cast<IfStmt*>(pkb.stmtTable.get(id).get());
            CondExpr cond = ifs->getCondExpr();
            populateIfPatternKB(id, cond);
        }
        unordered_set<StmtId> whileSet = pkb.stmtTable.getStmtsByType(StmtType::WHILE);
        for (StmtId id : whileSet) {
            WhileStmt* ws = dynamic_cast<WhileStmt*>(pkb.stmtTable.get(id).get());
            CondExpr cond = ws->getCondExpr();
            populateWhilePatternKB(id, cond);
        }
    }

    void DesignExtractor::populateAssignPatternKB(StmtId stmtId, Expression exp) {
        pkb.patternKB.addAssignPattern(exp.getStr(), stmtId);
        unordered_set<Pattern> patterns = exp.getPatterns();
        for (Pattern p : patterns) {
            pkb.patternKB.addAssignPattern(p, stmtId);
        }
    }

    void DesignExtractor::populateIfPatternKB(StmtId stmtId, CondExpr cond) {
        unordered_set<VarId> varSet = cond.getVarIds();
        for (VarId id : varSet) {
            pkb.patternKB.addIfPattern(id, stmtId);
        }
    }

    void DesignExtractor::populateWhilePatternKB(StmtId stmtId, CondExpr cond) {
        unordered_set<VarId> varSet = cond.getVarIds();
        for (VarId id : varSet) {
            pkb.patternKB.addWhilePattern(id, stmtId);
        }
    }

    void DesignExtractor::populateNext() {
        for (ProcId pid = 1; pid <= pkb.procTable.size(); pid++) {
            StmtListId sid = pkb.procTable.get(pid).getStmtLstId();
            StatementList& sl = pkb.stmtListTable.get(sid);
            updateLastStmtId(sl);
            populateNextKB(sid);
        }
    }

    void DesignExtractor::populateNextKB(StmtListId sid) {
        StatementList sl = pkb.stmtListTable.get(sid);
        std::vector<StmtId> idList = sl.getStmtIds();
        for (size_t i = 0; i < idList.size(); i++) {
            shared_ptr<Statement> s = pkb.stmtTable.get(idList[i]);
            if (s->getType() == StmtType::IF) {
                IfStmt* ifs = dynamic_cast<IfStmt*>(s.get());
                StmtListId thenId = ifs->getThenStmtLstId();
                StatementList& thenSl = pkb.stmtListTable.get(thenId);
                updateLastStmtId(thenSl);
                populateNextKB(thenId);
                pkb.addNext(idList[i], thenSl.getFirst());
                StmtListId elseId = ifs->getElseStmtLstId();
                StatementList& elseSl = pkb.stmtListTable.get(elseId);
                updateLastStmtId(elseSl);
                populateNextKB(elseId);
                pkb.addNext(idList[i], elseSl.getFirst());
                if (i < idList.size() - 1) {
                    for (StmtId s : thenSl.getAllEnds()) {
                        pkb.addNext(s, idList[i + 1]);
                    }
                    for (StmtId s : elseSl.getAllEnds()) {
                        pkb.addNext(s, idList[i + 1]);
                    }
                }
            } else if (s->getType() == StmtType::WHILE) {
                WhileStmt* ws = dynamic_cast<WhileStmt*>(s.get());
                StmtListId loopId = ws->getStmtLstId();
                StatementList& whileSl = pkb.stmtListTable.get(loopId);
                updateLastStmtId(whileSl);
                populateNextKB(loopId);
                pkb.addNext(idList[i], whileSl.getFirst());
                for (StmtId s : whileSl.getAllEnds()) {
                    pkb.addNext(s, idList[i]);
                }
                if (i < idList.size() - 1) {
                    pkb.addNext(idList[i], idList[i + 1]);
                }
            } else {
                if (i < idList.size() - 1) {
                    pkb.addNext(idList[i], idList[i + 1]);
                }
            }
        }
    }

    void DesignExtractor::updateLastStmtId(StatementList& sl) {
        shared_ptr<Statement> lastStmt = pkb.stmtTable.get(sl.getStmtIds().back());
        if (lastStmt->getType() == StmtType::IF) {
            IfStmt* ifs = dynamic_cast<IfStmt*>(lastStmt.get());
            StatementList& thenSl = pkb.stmtListTable.get(ifs->getThenStmtLstId());
            StatementList& elseSl = pkb.stmtListTable.get(ifs->getElseStmtLstId());
            updateLastStmtId(thenSl);
            updateLastStmtId(elseSl);
            for (StmtId s : thenSl.getAllEnds()) {
                sl.addEnd(s);
            }
            for (StmtId s : elseSl.getAllEnds()) {
                sl.addEnd(s);
            }
            sl.setMaxLast(elseSl.getMaxLast());
        } else if (lastStmt->getType() == StmtType::WHILE) {
            WhileStmt* ws = dynamic_cast<WhileStmt*>(lastStmt.get());
            StatementList& newSl = pkb.stmtListTable.get(ws->getStmtLstId());
            updateLastStmtId(newSl);
            sl.addEnd(sl.getStmtIds().back());
            sl.setMaxLast(newSl.getMaxLast());
        } else {
            sl.addEnd(sl.getMaxLast());
        }
    }

    void DesignExtractor::updateStmtContainerId() {
        for (StmtListId sid = 1; sid <= pkb.stmtListTable.size(); sid++) {
            StatementList sl = pkb.stmtListTable.get(sid);
            for (StmtId id : sl.getStmtIds()) {
                pkb.stmtTable.get(id)->setContainerId(sid);
            }
        }
    }
}
