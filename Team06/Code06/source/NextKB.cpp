#include "NextKB.h"
#include <stdexcept>

void NextKB::addStmtControlFlow(StmtId stmtId1, StmtId stmtId2)
{
    nextRS& nRS1 = controlFlowGraph[stmtId1];
    nextRS& nRS2 = controlFlowGraph[stmtId2];

    nRS1.allDirectNext.at(stmtId2) = true;
    nRS2.allDirectPrevious.at(stmtId1) = true;
}

bool NextKB::next(StmtId stmtId1, StmtId stmtId2)
{
    try {
        return controlFlowGraph.at[stmtId1].allDirectNext.contains(stmtId2);
    }
    catch (const std::out_of_range &) {
		return false;
	}
}

bool NextKB::nextStar(StmtId stmtId1, StmtId stmtId2)
{
    try {
        if(next(stmtId1, stmtId2)) {
            return true;
        }
        for(auto itr = controlFlowGraph.at[stmtId1].allDirectNext.begin(); itr != controlFlowGraph.at[stmtId1].allDirectNext.end(); itr ++) {
            if(nextStar(itr -> first, stmtId2)) {
                return true;
            }
        }
        return false;
    }
    catch (const std::out_of_range &) {
		return false;
	}
}

std::vector<StmtId> NextKB::getAllStmtsPrevious(StmtId stmtId)
{
    try {
        std::vector<StmtId> prevStmt;
        for(auto itr = controlFlowGraph.at[stmtId].allDirectPrevious.begin(); itr != controlFlowGraph.at[stmtId].allDirectPrevious.end(); itr ++) {
            prevStmt.emplace_back(itr -> first);
        }
        return prevStmt;
    }
    catch (const std::out_of_range &) {
		return {};
	}
}

std::vector<StmtId> NextKB::getAllStmtsNext(StmtId stmtId)
{
    try {
        std::vector<StmtId> prevStmt;
        for(auto itr = controlFlowGraph.at[stmtId].allDirectNext.begin(); itr != controlFlowGraph.at[stmtId].allDirectNext.end(); itr ++) {
            prevStmt.emplace_back(itr -> first);
        }
        return prevStmt;
    }
    catch (const std::out_of_range &) {
		return {};
	}
}

std::vector<StmtId> NextKB::getAllStmtsNextStar(StmtId stmtId)
{
    try {
        std::vector<StmtId> result;
        for(auto itr = controlFlowGraph.at[stmtId].allDirectNext.begin(); itr != controlFlowGraph.at[stmtId].allDirectNext.end(); itr ++) {
            result.emplace_back(itr -> first);
            std::vector<StmtId> subresult = getAllStmtsNextStar(itr -> first);
            result.insert(result.end(), subresult.begin(), subresult.end());
        }
        return result;
    }
    catch (const std::out_of_range &) {
		return {};
	}
}
