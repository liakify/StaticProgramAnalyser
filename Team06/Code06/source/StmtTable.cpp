#include "StmtTable.h"

namespace PKB {

    StmtTable::StmtTable() {
        stmtIdGenerator = 1;
    }

    StmtId StmtTable::reserveId() {
            return stmtIdGenerator++;
    }

    void StmtTable::unreserveId() {
            stmtIdGenerator--;
    }

    StmtId StmtTable::insertStmt(Statement* stmt) {
        idStmtTable.insert(std::make_pair(stmtIdGenerator, stmt));
        StmtType thisType = stmt->getType();
        StmtId thisId = stmtIdGenerator;
        stmtIdGenerator++;

        unordered_set<StmtId> newSet{ thisId };
        if (!typeIdsTable.try_emplace(thisType, newSet).second) {
            typeIdsTable.at(thisType).insert(thisId);
        }

        return thisId;
    }

    void StmtTable::insertStmtAtId(Statement* stmt, StmtId id) {
            try {
                    Statement* s = get(id);
            }
            catch (std::out_of_range&) {
                    idStmtTable.insert(std::make_pair(id, stmt));
                    StmtType thisType = stmt->getType();

                    unordered_set<StmtId> newSet{ id };
                    if (!typeIdsTable.try_emplace(thisType, newSet).second) {
                            typeIdsTable.at(thisType).insert(id);
                    }
                    return;
            }
            throw std::invalid_argument("ID already exists in StmtTable.");
    }

    Statement* StmtTable::get(StmtId stmtId) {
        return idStmtTable.at(stmtId);  // throws out_of_range exception
    }

    unordered_set<StmtId> StmtTable::getStmtsByType(StmtType stmtType) {
        try {
            return typeIdsTable.at(stmtType);
        }
        catch (const out_of_range&) {
            return {};
        }
    }

    int StmtTable::size() {
        return idStmtTable.size();
    }
}