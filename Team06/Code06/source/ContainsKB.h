#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Types.h"
#include "Constants.h"

namespace PKB {

    class ContainsKB {
     public:
        /*
            Adds Contains(procId, stmtId) relation to containsTable.
            Throws an exception if insertions are made in a way that results
            in a stmtId having multiple containing procIds, e.g.
            addContains(1, 1);
            addContains(2, 1);
        */
        void addContains(ProcId procId, StmtId stmtId);

        /*
            Returns TRUE if Contains(procId, stmtId) is true, FALSE otherwise.
        */
        bool contains(ProcId procId, StmtId stmtId);

        /*
            Returns the proc ID p for which Contains(p, stmtId) is true.
            If stmtId is not found, INVALID_PROC_ID 0 is returned.
        */
        ProcId getContainer(StmtId stmtId);

        /*
            Returns all statement IDs s for which Contains*(procId, s) is true.
            If procId is not found, EMPTY_RESULT is returned.
        */
        const std::unordered_set<StmtId>& getAllContains(ProcId procId);

     private:
        std::unordered_map<ProcId, std::unordered_set<StmtId>> procContainsTable;
        std::unordered_map<StmtId, ProcId> stmtContainsTable;

    };
}