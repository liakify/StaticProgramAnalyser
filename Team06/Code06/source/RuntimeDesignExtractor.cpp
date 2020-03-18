#include "PKB.h"
#include "RuntimeDesignExtractor.h"

namespace FrontEnd {
    RuntimeDesignExtractor::RuntimeDesignExtractor(PKB::PKB* pkb)
        : pkb(pkb) {
    }

    bool RuntimeDesignExtractor::nextStar(StmtId s1, StmtId s2) {
        std::shared_ptr<Statement>& stmtS1 = pkb->stmtTable.get(s1);
        StmtListId s1StmtListId = stmtS1->getContainerId();
        StatementList& s1StmtList = pkb->stmtListTable.get(s1StmtListId);
        if (!pkb->parentKB.hasParent(s1)) {
            if ((s2 > s1 && s2 <= s1StmtList.getLast()) || (s2 == s1 && stmtS1->getType() == StmtType::WHILE)) {
                addBiDirEdge(s1, s2);
                return true;
            }
            return false;
        } else {
            StmtId rootParent = pkb->parentKB.getRootParent(s1);
            std::shared_ptr<Statement>& rootParentStmt = pkb->stmtTable.get(rootParent);
            StmtListId rootParentStmtListId = rootParentStmt->getContainerId();
            StatementList& rootParentStmtList = pkb->stmtListTable.get(rootParentStmtListId);
            if (rootParentStmt->getType() == StmtType::WHILE) {
                if (s2 >= rootParent && s2 <= rootParentStmtList.getLast()) {
                    addBiDirEdge(s1, s2);
                    return true;
                }
                return false;
            } else {
                if (nextStarRecurse(s1, s1, s2)) {
                    addBiDirEdge(s1, s2);
                    return true;
                }
                return false;
            }
        }
    }

    void RuntimeDesignExtractor::processStmtAllNodes(StmtId s, NodeType type) {
        std::unordered_set<StmtId> visited;
        NodeType reverseType = type == NodeType::SUCCESSOR ? NodeType::PREDECESSOR : NodeType::SUCCESSOR;
        nextStarDFS(s, s, visited, type);
        pkb->nextKB.setProcessedAll(s, type);
    }

    void RuntimeDesignExtractor::addBiDirEdge(StmtId s1, StmtId s2) {
        pkb->nextKB.addToAll(s1, s2, NodeType::SUCCESSOR);
        pkb->nextKB.addToAll(s2, s1, NodeType::PREDECESSOR);
    }

    bool RuntimeDesignExtractor::nextStarRecurse(StmtId curr, StmtId last, StmtId s2) {
        if (!pkb->parentKB.hasParent(curr)) {  // can only be an IfStmt
            if (!pkb->followsKB.hasFollower(curr)) {
                return false;
            }
            return s2 >= pkb->followsKB.getFollower(curr);
        }

        StmtId parentId = pkb->parentKB.getParent(curr);
        std::shared_ptr<Statement>& parentStmt = pkb->stmtTable.get(parentId);
        if (parentStmt->getType() == StmtType::WHILE) {
            WhileStmt* ws = dynamic_cast<WhileStmt*>(parentStmt.get());
            StatementList& whileSl = pkb->stmtListTable.get(ws->getStmtLstId());
            if (s2 >= parentId && s2 <= whileSl.getLast()) {
                return true;
            }
            return nextStarRecurse(parentId, whileSl.getLast(), s2);
        } else {  // parentStmt is IfStmt
            std::shared_ptr<Statement>& currStmt = pkb->stmtTable.get(curr);
            StmtListId currStmtListId = currStmt->getContainerId();
            StatementList& currStmtList = pkb->stmtListTable.get(currStmtListId);
            if (s2 > last && s2 <= currStmtList.getLast()) {
                return true;
            }
            IfStmt* ifs = dynamic_cast<IfStmt*>(parentStmt.get());
            StatementList& elseSl = pkb->stmtListTable.get(ifs->getElseStmtLstId());
            return nextStarRecurse(parentId, elseSl.getLast(), s2);
        }
    }

    void RuntimeDesignExtractor::nextStarDFS(StmtId root, StmtId curr, std::unordered_set<StmtId>& visited, NodeType type) {
        visited.insert(curr);

        std::unordered_set<StmtId> neighbours = pkb->nextKB.getDirectNodes(curr, type);

        for (const auto& n : neighbours) {
            // Add bi-directional edge first before cycle check for Next*(s, s)
            addBiDirEdge(root, n);
            if (visited.find(n) != visited.end()) {
                // Cycle Detected
                return;
            }
            nextStarDFS(root, n, visited, type);
        }
    }
}
