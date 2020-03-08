#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Types.h"

class NextKB
{
public:
    /*
		Adds Next(stmtId1, stmtId2) relation to stmtControlFlowTable.
	*/
	void addStmtControlFlow(StmtId stmtId1, StmtId stmtId2);

    /*
		Returns TRUE if Next(stmtId1, stmtId2) is true, FALSE otherwise.
	*/
	bool stmtNext(StmtId stmtId1, StmtId stmtId2);

    /*
		Returns all statement IDs for which Next*(stmtId, s) is true.
	*/
	unordered_set<StmtId> getAllStmtsNextStmt(StmtId stmtId);

private:
	unordered_map<StmtId, std::unordered_set<StmtId>> stmtControlFlowTable;
};
