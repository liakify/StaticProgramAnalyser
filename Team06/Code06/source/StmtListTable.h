#pragma once

#include<stdio.h>
#include <string>
#include <unordered_map>

using namespace std;

typedef int StmtId;
typedef int StmtListId;
typedef string StmtType;

class StatementList {
public:
  bool operator== (const StatementList& other) {
    return true;
  }
};

class StmtListTable
{
  unordered_map<StmtListId, StatementList> idStmtListTable;
  StmtListId stmtListIdGenerator;

public: 
  //Inserts stmtLst into the StmtLstTable. Returns the ID of the statement list in the StmtLstTable.
  StmtListId insertStmtLst(StatementList stmtLst) {
    StmtListId thisId = stmtListIdGenerator;
    stmtListIdGenerator++;
    idStmtListTable.insert(make_pair(thisId, stmtLst));
    return thisId;
  }

  // Returns the statement list object at the given ID in the StmtLstTable. 
  // Throws an exception if the ID is not found in the table.
  StatementList get(StmtListId stmtListId) {
    return idStmtListTable.at(stmtListId);
  }


  //Returns the number of statements in the StmtTable.
  int size() {
    return idStmtListTable.size();
  }

};

