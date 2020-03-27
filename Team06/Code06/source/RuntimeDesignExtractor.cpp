#include <cassert>

#include "PKB.h"
#include "RuntimeDesignExtractor.h"

namespace FrontEnd {
    bool RuntimeDesignExtractor::processNextStar(StmtId s1, StmtId s2, PKB::PKB* pkb) {
        this->pkb = pkb;

        std::shared_ptr<Statement>& stmtS1 = pkb->stmtTable.get(s1);
        StmtListId s1StmtListId = stmtS1->getContainerId();
        StatementList& s1StmtList = pkb->stmtListTable.get(s1StmtListId);
        if (!pkb->parentKB.hasParent(s1)) {
            if ((s2 > s1 && s2 <= s1StmtList.getMaxLast()) || (s2 == s1 && stmtS1->getType() == StmtType::WHILE)) {
                addBiDirEdge(s1, s2, NodeType::SUCCESSOR);
                return true;
            }
            return false;
        } else {
            StmtId rootParent = pkb->parentKB.getRootParent(s1);
            std::shared_ptr<Statement>& rootParentStmt = pkb->stmtTable.get(rootParent);
            StmtListId rootParentStmtListId = rootParentStmt->getContainerId();
            StatementList& rootParentStmtList = pkb->stmtListTable.get(rootParentStmtListId);
            if (rootParentStmt->getType() == StmtType::WHILE) {
                if (s2 >= rootParent && s2 <= rootParentStmtList.getMaxLast()) {
                    addBiDirEdge(s1, s2, NodeType::SUCCESSOR);
                    return true;
                }
                return false;
            } else {  // rootParent is an IFStmt
                if (s2 > rootParentStmtList.getMaxLast() || s2 <= rootParent) {
                    return false;
                }
                if (nextStarRecurse(s1, s1, s2)) {
                    addBiDirEdge(s1, s2, NodeType::SUCCESSOR);
                    return true;
                }
                return false;
            }
        }
    }

    void RuntimeDesignExtractor::processNextStarGetAllNodes(StmtId s, NodeType type, PKB::PKB* pkb) {
        this->pkb = pkb;

        std::unordered_set<StmtId> visited;
        nextStarDFS(s, s, visited, type);
        pkb->nextStarSetProcessedAll(s, type);
    }

    void RuntimeDesignExtractor::addBiDirEdge(StmtId s1, StmtId s2, NodeType type) {
        NodeType reverseType = type == NodeType::SUCCESSOR ? NodeType::PREDECESSOR : NodeType::SUCCESSOR;
        pkb->nextStarAddToAll(s1, s2, type);
        pkb->nextStarAddToAll(s2, s1, reverseType);
    }

    bool RuntimeDesignExtractor::nextStarRecurse(StmtId curr, StmtId last, StmtId s2) {
        std::shared_ptr<Statement>& currStmt = pkb->stmtTable.get(curr);
        StmtListId currStmtListId = currStmt->getContainerId();
        StatementList& currStmtList = pkb->stmtListTable.get(currStmtListId);

        if (!pkb->parentKB.hasParent(curr)) {  // can only be an IfStmt
            if (!pkb->followsKB.hasFollower(curr)) {
                return false;
            }
            return s2 >= pkb->followsKB.getFollower(curr) && s2 <= currStmtList.getMaxLast();
        }

        StmtId parentId = pkb->parentKB.getParent(curr);
        std::shared_ptr<Statement>& parentStmt = pkb->stmtTable.get(parentId);
        if (parentStmt->getType() == StmtType::WHILE) {
            WhileStmt* ws = dynamic_cast<WhileStmt*>(parentStmt.get());
            StatementList& whileSl = pkb->stmtListTable.get(ws->getStmtLstId());
            if (s2 >= parentId && s2 <= whileSl.getMaxLast()) {
                return true;
            }
            return nextStarRecurse(parentId, whileSl.getMaxLast(), s2);
        } else {  // parentStmt is IfStmt
            if ((s2 > last && s2 <= currStmtList.getMaxLast()) ||
                (currStmt->getType() == StmtType::WHILE && s2 == last)) {
                return true;
            }

            IfStmt* ifs = dynamic_cast<IfStmt*>(parentStmt.get());
            StatementList& elseSl = pkb->stmtListTable.get(ifs->getElseStmtLstId());
            return nextStarRecurse(parentId, elseSl.getMaxLast(), s2);
        }
    }

    void RuntimeDesignExtractor::nextStarDFS(StmtId root, StmtId curr, std::unordered_set<StmtId>& visited, NodeType type) {
        visited.insert(curr);

        std::unordered_set<StmtId> neighbours = pkb->nextStarGetDirectNodes(curr, type);

        for (const auto& n : neighbours) {
            // Add bi-directional edge first before cycle check for Next*(s, s)
            addBiDirEdge(root, n, type);
            if (visited.find(n) == visited.end()) {
                nextStarDFS(root, n, visited, type);
            }
        }
    }

    bool RuntimeDesignExtractor::processAffects(StmtId s1, StmtId s2, PKB::PKB* pkb) {
        this->pkb = pkb;
        assert(pkb->stmtTable.get(s1)->getType() == StmtType::ASSIGN);
        assert(pkb->stmtTable.get(s2)->getType() == StmtType::ASSIGN);
        if (!pkb->nextStar(s1, s2)) {
            return false;
        }

        AssignStmt* a1 = dynamic_cast<AssignStmt*>(pkb->stmtTable.get(s1).get());
        std::unordered_set<VarId> usedVars = pkb->usesKB.getAllVarsUsedByStmt(s2);
        VarId modifiedId = a1->getVar();
        if (usedVars.find(modifiedId) == usedVars.end()) {
            return false;
        }
        std::unordered_set<StmtId> visited, result;
        affectsDFS(s1, modifiedId, s1, visited, result, s2);
        for (StmtId id : result) {
            pkb->addAffects(s1, id);
        }
        return result.find(s2) != result.end();
    }

    void RuntimeDesignExtractor::processAffectsGetAllNodes(StmtId s, NodeType type, PKB::PKB* pkb) {
        this->pkb = pkb;
        assert(pkb->stmtTable.get(s)->getType() == StmtType::ASSIGN);
        AssignStmt* a = dynamic_cast<AssignStmt*>(pkb->stmtTable.get(s).get());
        std::unordered_set<StmtId> visited, result;
        if (type == NodeType::SUCCESSOR) {
            VarId modifiedId = a->getVar();
            affectsDFS(s, modifiedId, s, visited, result);
            for (StmtId id : result) {
                pkb->addAffects(s, id);
            }
            pkb->affectsSetProcessedAll(s, NodeType::SUCCESSOR);
        } else {
            std::unordered_set<VarId> usedId = a->getExpr().getVarIds();
            affectedByDFS(s, usedId, s, visited, result);
            for (StmtId id : result) {
                pkb->addAffects(id, s);
            }
            pkb->affectsSetProcessedAll(s, NodeType::PREDECESSOR);
        }
    }

    void RuntimeDesignExtractor::affectsDFS(StmtId root, VarId modifiedId, StmtId curr,
            std::unordered_set<StmtId>& visited, std::unordered_set<StmtId>& result, StmtId goal) {
        if (curr != root || visited.size() != 0) {
            visited.insert(curr);
            std::shared_ptr<Statement> s = pkb->stmtTable.get(curr);
            if (s->getType() != StmtType::IF && s->getType() != StmtType::WHILE) {
                if (s->getType() == StmtType::ASSIGN && pkb->usesKB.stmtUses(curr, modifiedId)) {
                    result.insert(curr);
                }
                if (pkb->modifiesKB.stmtModifies(curr, modifiedId)) {
                    return;
                }
            }
        }

        std::unordered_set<StmtId> neighbours = pkb->nextStarGetDirectNodes(curr, NodeType::SUCCESSOR);
        for (StmtId next : neighbours) {
            if (visited.find(next) == visited.end()) {
                affectsDFS(root, modifiedId, next, visited, result, goal);
                if (next == goal) {
                    break;
                }
            }
        }
    }


    void RuntimeDesignExtractor::affectedByDFS(StmtId root, std::unordered_set<VarId> usedId, StmtId curr,
            std::unordered_set<StmtId>& visited, std::unordered_set<StmtId>& result) {
        if (curr != root || visited.size() != 0) {
            visited.insert(curr);
            std::shared_ptr<Statement> s = pkb->stmtTable.get(curr);
            if (s->getType() != StmtType::IF && s->getType() != StmtType::WHILE) {
                bool isModified = false;
                for (VarId id : pkb->modifiesKB.getAllVarsModifiedByStmt(curr)) {
                    if (usedId.find(id) != usedId.end()) {
                        isModified = true;
                        usedId.erase(id);
                    }
                }
                if (s->getType() == StmtType::ASSIGN && isModified) {
                    result.insert(curr);
                }
                if (usedId.size() == 0 || curr == root) {
                    // Since the reverse DFS traverses all possible reverse paths, 
                    // there will be no need to search beyond a single cycle back to the root
                    return;
                }
            }
        }

        std::unordered_set<StmtId> neighbours = pkb->nextStarGetDirectNodes(curr, NodeType::PREDECESSOR);
        for (StmtId prev : neighbours) {
            // Cannot check visited here because the DFS needs to search all possible paths backwards
            affectedByDFS(root, usedId, prev, visited, result);
        }
    }
}
