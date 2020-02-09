#include<stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "Types.h"

using namespace std;

namespace PKB {

  class Statement {
  public:
    StmtType stmtType;

    Statement(StmtType stmtType);
  };


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
