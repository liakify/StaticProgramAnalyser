#ifndef STMTTABLE_H
#define STMTTABLE_H

#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include "Types.h"
#include "Simple.h"

using std::out_of_range;
using std::unordered_map;
using std::unordered_set;
using SIMPLE::Statement;

namespace PKB {

  /**
   *  The StmtTable class stores all statements extracted from the SIMPLE source code
   *  and tags them with an ID.
   */
  class StmtTable
  {
    unordered_map<StmtId, Statement*> idStmtTable;
    unordered_map<StmtType, unordered_set<StmtId>> typeIdsTable;
    StmtId stmtIdGenerator;

  public:

    StmtTable();

    /**
     *  Inserts stmt into the StmtTable. Returns the ID of the statement in the StmtTable.
     */
    StmtId insertStmt(Statement* stmt);

    /*
    *   Returns a reserved ID to be used for future insertion.
    */
    StmtId reserveId();

    /*
    *   Decrements the StmtId generator value.
    *   Only call this if it is needed to directly undo a reserveId() call,
    *   i.e. no other statements were added between the latest reserveId() call
    *   and this call.
    */
    void unreserveId();

    /**
     *  Inserts stmt into the StmtTable at a specified ID.
     *  If the ID already exists, an error is thrown.
     */
    void insertStmtAtId(Statement* stmt, StmtId id);

    /**
     *  Returns the statement object at the given ID in the StmtTable. 
     *  Throws an exception if the ID is not found in the table.
     */
    Statement* get(StmtId stmtId);

    /**
     *  Returns a list of statement IDs that match the specified statement type.
     *  An empty list is returned if no such statements exist.
     */
    unordered_set<StmtId> getStmtsByType(StmtType stmtType);

    /**
     *  Returns the number of statements in the StmtTable.
     */
    int size();
  };
}

#endif