#ifndef STMTTABLE_H
#define STMTTABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Types.h"
#include "Simple.h"

using std::out_of_range;
using std::unordered_map;
using std::list;
using SIMPLE::Statement;

namespace PKB {
  class StmtTable
  {
    unordered_map<StmtId, Statement> idStmtTable;
    unordered_map<StmtType, list<StmtId>> typeIdsTable;
    StmtId stmtIdGenerator;

  public:

    StmtTable();
    StmtId insertStmt(Statement stmt);
    Statement get(StmtId stmtId);
    list<StmtId> getStmtsByType(StmtType stmtType);
    int size();
  };
}

#endif