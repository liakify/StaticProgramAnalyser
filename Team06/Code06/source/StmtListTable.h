#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include "Types.h"
#include "Simple.h"

using std::out_of_range;
using std::unordered_map;
using SIMPLE::StatementList;

namespace PKB {

  class StmtListTable
  {
    unordered_map<StmtListId, StatementList> idStmtListTable;
    StmtListId stmtListIdGenerator;

  public:

    StmtListTable();
    StmtListId insertStmtLst(StatementList stmtLst);
    StatementList get(StmtListId stmtListId);
    int size();
  };
}