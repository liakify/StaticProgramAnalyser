#include "StmtListTable.h"

namespace PKB {

    StmtListTable::StmtListTable() {
        stmtListIdGenerator = 1;
    }

    StmtListId StmtListTable::insertStmtLst(StatementList stmtLst) {
        StmtListId thisId = stmtListIdGenerator;
        stmtListIdGenerator++;
        idStmtListTable.insert(std::make_pair(thisId, stmtLst));
        return thisId;
    }

    StatementList StmtListTable::get(StmtListId stmtListId) {
        return idStmtListTable.at(stmtListId);
    }

    int StmtListTable::size() {
        return idStmtListTable.size();
    }
};
