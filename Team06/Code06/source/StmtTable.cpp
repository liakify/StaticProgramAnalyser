#include "StmtTable.h"

namespace PKB {

  StmtTable::StmtTable() {
    stmtIdGenerator = 0;
  }

  // Inserts stmt into the StmtTable. Returns the ID of the statement in the StmtTable.
  StmtId StmtTable::insertStmt(Statement stmt) {
    idStmtTable.insert(std::make_pair(stmtIdGenerator, stmt));
    StmtType thisType = stmt.stmtType;
    StmtId thisId = stmtIdGenerator;
    stmtIdGenerator++;

    list<StmtId> newList{ thisId };
    if (!typeIdsTable.try_emplace(thisType, newList).second) {
      typeIdsTable.at(thisType).push_back(thisId);
    }

    return thisId;
  }

  // Returns the statement object at the given ID in the StmtTable. 
  // Throws an exception if the ID is not found in the table.
  Statement StmtTable::get(StmtId stmtId) {
    return idStmtTable.at(stmtId); // throws out_of_range exception
  }

  // Returns a list of statement IDs that match the specified statement type. 
  // An empty list is returned if no such statements exist.
  list<StmtId> StmtTable::getStmtsByType(StmtType stmtType) {
    try {
      return typeIdsTable.at(stmtType);
    }
    catch (const out_of_range&) {
      return {};
    }
  }

  // Returns the number of statements in the StmtTable.
  int StmtTable::size() {
    return idStmtTable.size();
  }
}