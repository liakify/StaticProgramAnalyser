#pragma once

#include <vector>
#include <unordered_map>

#include "Types.h"

struct nextRS
{
    //Using map for O(1) lookup, but the boolean is unnecessary
    std::unordered_map<StmtId, bool> allDirectPrevious;
    std::unordered_map<StmtId, bool> allDirectNext;
};

class NextKB
{
public:
    /*
		Adds Next(stmtId1, stmtId2) relation to controlFlowGraph.
	*/
	void addStmtControlFlow(StmtId stmtId1, StmtId stmtId2);

    /*
		Returns TRUE if Next(stmtId1, stmtId2) is true, FALSE otherwise.
	*/
	bool next(StmtId stmtId1, StmtId stmtId2);

    /*
		Returns TRUE if Next*(stmtId1, stmtId2) is true, FALSE otherwise.
	*/
	bool nextStar(StmtId stmtId1, StmtId stmtId2);

    /*
		Returns all statement IDs for which Next(s, stmtId) is true.
	*/
    std::vector<StmtId> getAllStmtsPrevious(StmtId stmtId);

    /*
		Returns all statement IDs for which Next(stmtId, s) is true.
	*/
    std::vector<StmtId> getAllStmtsNext(StmtId stmtId);

    /*
		Returns all statement IDs for which Next*(stmtId, s) is true.
	*/
	std::vector<StmtId> getAllStmtsNextStar(StmtId stmtId);

private:
	std::unordered_map<StmtId, nextRS> controlFlowGraph;
};
