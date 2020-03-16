#include <utility>

#include "StmtTable.h"

using std::out_of_range;
using std::shared_ptr;
using std::unordered_map;
using std::unordered_set;

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

    StmtId StmtTable::insertStmt(shared_ptr<Statement> stmt) {
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

    void StmtTable::insertStmtAtId(shared_ptr<Statement> stmt, StmtId id) {
        try {
            shared_ptr<Statement>& s = get(id);
        }
        catch (const out_of_range&) {
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

    shared_ptr<Statement>& StmtTable::get(StmtId stmtId) {
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