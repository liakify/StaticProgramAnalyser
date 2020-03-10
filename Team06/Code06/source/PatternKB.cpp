#include "PatternKB.h"
#include <stdexcept>

void PatternKB::addAssignPattern(Pattern pattern, StmtId stmtId)
{
    assignTable[pattern].insert(stmtId);
}

void PatternKB::addIfPattern(VarId varId, StmtId stmtId)
{
    ifTable[varId].insert(stmtId);
}

void PatternKB::addWhilePattern(VarId varId, StmtId stmtId)
{
    whileTable[varId].insert(stmtId);
}

const std::unordered_set<StmtId>& PatternKB::getAssignPatternStmts(Pattern pattern)
{
    try {
        return assignTable.at(pattern);
    }
    catch (const std::out_of_range &) {
        return EMPTY_RESULT;
    }
}

const std::unordered_set<StmtId>& PatternKB::getIfPatternStmts(VarId varId)
{
    try {
        return ifTable.at(varId);
    }
    catch (const std::out_of_range&) {
        return EMPTY_RESULT;
    }
}

const std::unordered_set<StmtId>& PatternKB::getWhilePatternStmts(VarId varId)
{
    try {
        return whileTable.at(varId);
    }
    catch (const std::out_of_range&) {
        return EMPTY_RESULT;
    }
}
