#include "ContainsKB.h"
#include <stdexcept>

namespace PKB {

    void ContainsKB::addContains(ProcId procId, StmtId stmtId) {
        if (stmtContainsTable.find(stmtId) != stmtContainsTable.end()) {
            if (stmtContainsTable.at(stmtId) != procId) {
                throw std::invalid_argument("Statement already has container procedure");
            }
        } else {
            stmtContainsTable.emplace(stmtId, procId);
        }
        if (procContainsTable.find(procId) != procContainsTable.end()) {
            procContainsTable.at(procId).insert(stmtId);
        } else {
            procContainsTable.emplace(procId, std::unordered_set<StmtId>({stmtId}));
        }
    }

    bool ContainsKB::contains(ProcId procId, StmtId stmtId) {
        try {
            return stmtContainsTable.at(stmtId) == procId;
        } catch (const std::out_of_range&) {
            return false;
        }
    }

    ProcId ContainsKB::getContainer(StmtId stmtId) {
        try {
            return stmtContainsTable.at(stmtId);
        } catch (const std::out_of_range&) {
            return INVALID_PROC_ID;
        }
    }

    const std::unordered_set<StmtId>& ContainsKB::getAllContains(ProcId procId) {
        try {
            return procContainsTable.at(procId);
        } catch (const std::out_of_range&) {
            return EMPTY_RESULT;
        }
    }
}


