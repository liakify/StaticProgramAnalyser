#include<stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "Types.h"

using namespace std;

class Statement {
public:

  StmtType stmtType;

  Statement(StmtType stmtType) {
    this->stmtType = stmtType;
  }
};


class StmtTable
{
  unordered_map<StmtId, Statement> idStmtTable;
  unordered_map<StmtType, list<StmtId>> typeIdsTable;
  StmtId stmtIdGenerator;

public:

  StmtTable() {
    stmtIdGenerator = 0;
  }

  // Inserts stmt into the StmtTable. Returns the ID of the statement in the StmtTable.
  StmtId insertStmt(Statement stmt) {
    idStmtTable.insert(make_pair(stmtIdGenerator, stmt));
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
  Statement get(StmtId stmtId) {
    return idStmtTable.at(stmtId); // throws out_of_range exception
  }

  // Returns a list of statement IDs that match the specified statement type. 
  // An empty list is returned if no such statements exist.
  list<StmtId> getStmtsByType(StmtType stmtType) {
    try {
      return typeIdsTable.at(stmtType);
    }
    catch (const std::out_of_range&) {
      return {};
    }
  }

  // Returns the number of statements in the StmtTable.
  int size() {
    return idStmtTable.size();
  }
};
