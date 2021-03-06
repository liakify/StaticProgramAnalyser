#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Types.h"
#include "Constants.h"

namespace PKB {

    class PatternKB {
     public:
        void addAssignPattern(Pattern pattern, StmtId stmtId);

        void addIfPattern(VarId varId, StmtId stmtId);

        void addWhilePattern(VarId varId, StmtId stmtId);

        const std::unordered_set<StmtId>& getAssignPatternStmts(Pattern pattern);

        const std::unordered_set<StmtId>& getIfPatternStmts(VarId varId);

        const std::unordered_set<StmtId>& getWhilePatternStmts(VarId varId);

        const std::unordered_set<StmtId>& getAllIfStmtsWithCtrlVars();

        const std::unordered_set<StmtId>& getAllWhileStmtsWithCtrlVars();

     private:
        std::unordered_map<Pattern, std::unordered_set<StmtId>> assignTable;
        std::unordered_map<VarId, std::unordered_set<StmtId>> ifTable;
        std::unordered_map<VarId, std::unordered_set<StmtId>> whileTable;

        std::unordered_set<StmtId> allIfStmtsWithCtrlVars;
        std::unordered_set<StmtId> allWhileStmtsWithCtrlVars;
    };
}