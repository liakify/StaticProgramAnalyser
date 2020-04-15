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

        std::unordered_set<StmtId> neighbours = pkb->nextGetDirectNodes(curr, type);

        for (const auto& n : neighbours) {
            if (pkb->nextStarProcessedAll(n, type)) {
                std::unordered_set<StmtId> nodes = pkb->nextStarGetAllNodes(n, type);
                for (StmtId id : nodes) {
                    visited.insert(id);
                    addBiDirEdge(root, id, type);
                }
                continue;
            }

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
        std::unordered_set<StmtId> visited;
        return affectsDFS(s1, modifiedId, s1, visited, s2);
    }

    void RuntimeDesignExtractor::processAffectsGetDirectNodes(StmtId s, NodeType type, PKB::PKB* pkb) {
        this->pkb = pkb;
        assert(pkb->stmtTable.get(s)->getType() == StmtType::ASSIGN);
        AssignStmt* a = dynamic_cast<AssignStmt*>(pkb->stmtTable.get(s).get());
        std::unordered_set<StmtId> visited;
        if (type == NodeType::SUCCESSOR) {
            VarId modifiedId = a->getVar();
            affectsDFS(s, modifiedId, s, visited);
            pkb->affectsSetProcessedDirect(s, NodeType::SUCCESSOR);
        } else {
            std::unordered_set<VarId> usedId = a->getExpr().getVarIds();
            affectedByDFS(s, usedId, s, visited);
            pkb->affectsSetProcessedDirect(s, NodeType::PREDECESSOR);
        }
    }

    bool RuntimeDesignExtractor::affectsDFS(StmtId root, VarId modifiedId, StmtId curr,
        std::unordered_set<StmtId>& visited, StmtId goal) {
        visited.insert(curr);

        std::unordered_set<StmtId> neighbours = pkb->nextGetDirectNodes(curr, NodeType::SUCCESSOR);
        for (StmtId next : neighbours) {
            std::shared_ptr<Statement> s = pkb->stmtTable.get(next);
            if (pkb->usesKB.stmtUses(next, modifiedId)) {
                if (s->getType() == StmtType::ASSIGN) {
                    pkb->addAffects(root, next);
                    if (next == goal) {
                        return true;
                    }
                }
            }
            if (pkb->modifiesKB.stmtModifies(next, modifiedId) &&
                s->getType() != StmtType::IF &&
                s->getType() != StmtType::WHILE) {
                    continue;
            }

            if (visited.find(next) == visited.end()) {
                if (affectsDFS(root, modifiedId, next, visited, goal)) {
                    return true;
                }
            }
        }
        return false;
    }

    void RuntimeDesignExtractor::affectedByDFS(StmtId root, std::unordered_set<VarId>& usedIds, StmtId curr, std::unordered_set<StmtId>& visited) {
        visited.insert(curr);

        std::unordered_set<StmtId> neighbours = pkb->nextGetDirectNodes(curr, NodeType::PREDECESSOR);

        for (StmtId prev : neighbours) {
            std::unordered_set<VarId> erasedIds;
            std::shared_ptr<Statement> s = pkb->stmtTable.get(prev);
            if (s->getType() == StmtType::ASSIGN || s->getType() == StmtType::READ || s->getType() == StmtType::CALL) {
                for (VarId id : pkb->modifiesKB.getAllVarsModifiedByStmt(prev)) {
                    if (usedIds.find(id) != usedIds.end()) {
                        erasedIds.insert(id);
                    }
                }
            }
            if (s->getType() == StmtType::ASSIGN && !erasedIds.empty()) {
                pkb->addAffects(prev, root);
            }
            if (usedIds.size() == erasedIds.size()) {
                continue;
            }
            if (visited.find(prev) == visited.end()) {
                for (VarId id : erasedIds) {
                    usedIds.erase(id);
                }
                affectedByDFS(root, usedIds, prev, visited);
                usedIds.insert(erasedIds.begin(), erasedIds.end());
            }
        }

        visited.erase(curr);
    }

    bool RuntimeDesignExtractor::processAffectsStar(StmtId s1, StmtId s2, PKB::PKB* pkb) {
        this->pkb = pkb;

        if (!pkb->allAffectsFullyComputed()) {
            populateAllAffects();
        }

        std::unordered_set<StmtId> visited;
        return affectsStarDFS(s1, s1, visited, NodeType::SUCCESSOR, s2);
    }

    void RuntimeDesignExtractor::processAffectsStarGetAllNodes(StmtId s, NodeType type, PKB::PKB* pkb) {
        this->pkb = pkb;

        if (!pkb->allAffectsFullyComputed()) {
            populateAllAffects();
        }

        std::unordered_set<StmtId> visited;
        affectsStarDFS(s, s, visited, type);
        pkb->affectsStarSetProcessedAll(s, type);
    }

    bool RuntimeDesignExtractor::affectsStarDFS(StmtId root, StmtId curr, std::unordered_set<StmtId>& visited, NodeType type, StmtId goal) {
        visited.insert(curr);

        if (pkb->affectsStarProcessedAll(curr, type)) {

            std::unordered_set<StmtId> nodes = pkb->affectsStarGetAllNodes(curr, type);
            for (StmtId id : nodes) {
                visited.insert(id);
                if (type == NodeType::SUCCESSOR) {
                    pkb->addAffectsStar(root, id);
                } else {
                    pkb->addAffectsStar(id, root);
                }
            }
            if (nodes.find(goal) != nodes.end()) {
                return true;
            }
            return false;
        }

        std::unordered_set<StmtId> neighbours = pkb->affectsGetDirectNodes(curr, type);

        for (StmtId n : neighbours) {
            // Add bi-directional edge first before cycle check for Affects*(s, s)
            if (type == NodeType::SUCCESSOR) {
                pkb->addAffectsStar(root, n);
            } else {
                pkb->addAffectsStar(n, root);
            }
            if (n == goal) {
                return true;
            }

            if (visited.find(n) == visited.end()) {
                if (affectsStarDFS(root, n, visited, type, goal)) {
                    return true;
                }
            }
        }
        return false;
    }

    void RuntimeDesignExtractor::populateAllAffects() {
        std::unordered_set<StmtId> assignStmts = pkb->stmtTable.getStmtsByType(StmtType::ASSIGN);

        for (StmtId id : assignStmts) {
            // Only need to populate forward as each edge added is bi-directional
            pkb->affectsGetDirectNodes(id, NodeType::SUCCESSOR);  // affectsSetProcessedDirect(i, NodeType::SUCCESSOR) is called here
            pkb->affectsSetProcessedDirect(id, NodeType::PREDECESSOR);
        }

        pkb->setAffectsFullyComputed();
    }
}
