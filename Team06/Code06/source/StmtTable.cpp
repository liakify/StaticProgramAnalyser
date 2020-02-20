#include "StmtTable.h"

namespace PKB {

  StmtTable::StmtTable() {
    stmtIdGenerator = 1;
  }

  StmtId StmtTable::insertStmt(Statement stmt) {
    idStmtTable.insert(std::make_pair(stmtIdGenerator, stmt));
    StmtType thisType = stmt.getType();
    StmtId thisId = stmtIdGenerator;
    stmtIdGenerator++;

    list<StmtId> newList{ thisId };
    if (!typeIdsTable.try_emplace(thisType, newList).second) {
      typeIdsTable.at(thisType).push_back(thisId);
    }

    return thisId;
  }

  Statement StmtTable::get(StmtId stmtId) {
    return idStmtTable.at(stmtId); // throws out_of_range exception
  }

  list<StmtId> StmtTable::getStmtsByType(StmtType stmtType) {
    try {
      return typeIdsTable.at(stmtType);
    }
    catch (const out_of_range&) {
      return {};
    }
  }

  int StmtTable::size() {
    return idStmtTable.size();
  }
}