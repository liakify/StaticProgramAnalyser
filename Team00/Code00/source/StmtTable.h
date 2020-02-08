#include<stdio.h>
#include <string>
#include <unordered_map>

using namespace std;

typedef int StmtId;
typedef string StmtType;

class Statement {
public:
  StmtType type;

};


class StmtTable
{
  unordered_map<StmtId, Statement> idStmtTable;
  unordered_map<StmtType, list<StmtId>> typeIdTable;
  StmtId stmtIdGenerator;

public:

  StmtTable() {
    stmtIdGenerator = 0;
  }

  // Inserts stmt into the StmtTable. Returns the ID of the statement in the StmtTable.
  StmtId insertStmt(Statement stmt) {
    idStmtTable.insert(make_pair(stmtIdGenerator, stmt));
    StmtType thisType = stmt.type;
    StmtId thisId = stmtIdGenerator;
    stmtIdGenerator++;

    if (!typeIdTable.try_emplace(thisType, thisId).second) {
      typeIdTable.at(thisType).push_back(thisId);
    }

    return thisId;
  }

  // Returns the statement object at the given ID in the StmtTable. 
  // Throws an exception if the ID is not found in the table.
  Statement get(StmtId stmtId) {
    return idStmtTable.at(stmtId); // throws out_of_range exception
  }

  // Returns a list of statement IDs that match the specified statement type. An empty list is returned if no such statements exist.
  list<StmtId> getStmtsByType(StmtType stmtType) {
    return typeIdTable.at(stmtType);
  }

  // Returns the number of statements in the StmtTable.
  int size() {
    return idStmtTable.size();
  }

};
