#include "StmtListTable.h"

namespace PKB {

  bool StatementList::operator== (const StatementList& other) {
    return true;
  }

  //Inserts stmtLst into the StmtLstTable. Returns the ID of the statement list in the StmtLstTable.
  StmtListId StmtListTable::insertStmtLst(StatementList stmtLst) {
    StmtListId thisId = stmtListIdGenerator;
    stmtListIdGenerator++;
    idStmtListTable.insert(make_pair(thisId, stmtLst));
    return thisId;
  }

  // Returns the statement list object at the given ID in the StmtLstTable. 
  // Throws an exception if the ID is not found in the table.
  StatementList StmtListTable::get(StmtListId stmtListId) {
    return idStmtListTable.at(stmtListId);
  }


  //Returns the number of statements in the StmtTable.
  int StmtListTable::size() {
    return idStmtListTable.size();
  }
};
