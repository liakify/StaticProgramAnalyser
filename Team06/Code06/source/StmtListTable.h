#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include "Types.h"
#include "Simple.h"

using std::out_of_range;
using std::unordered_map;
using SIMPLE::StatementList;

namespace PKB {

    /**
     *  The StmtListTable class stores all StmtLists extracted from the SIMPLE source code
     *  and tags them with an ID.
     */
    class StmtListTable
    {
        unordered_map<StmtListId, StatementList> idStmtListTable;
        StmtListId stmtListIdGenerator;

    public:

        StmtListTable();
        /**
         *  Inserts stmtLst into the StmtLstTable. Returns the ID of the statement list in the StmtLstTable.
         */
        StmtListId insertStmtLst(StatementList stmtLst);

        /**
         *  Returns the statement list object at the given ID in the StmtLstTable. 
         *  Throws an exception if the ID is not found in the table.
         */
        StatementList get(StmtListId stmtListId);

        /**
         *  Returns the number of statements in the StmtTable.
         */
        int size();
    };
}