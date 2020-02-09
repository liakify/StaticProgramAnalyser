#pragma once

#include<stdio.h>
#include <string>
#include <unordered_map>
#include "Types.h"

using namespace std;

namespace PKB {

  class StatementList {
  public:
    bool operator== (const StatementList& other);
  };

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