#include <string>
#include <unordered_map>
#include <unordered_set>

#include "IfPatternEvaluator.h"
#include "LoggingUtils.h"

namespace PQL {
    namespace IfPatternEvaluator {

        /**
        * Evaluates a single if pattern clause on the given PKB where the inputs is one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateIfPatternClauseWild(PKB::PKB& database, PatternClause& clause, ClauseResult& intResult) {
            
            std::unordered_set<StmtId> stmts = database.patternKB.getAllIfStmtsWithCtrlVars();

            Synonym arg0 = clause.getSynonym().value;

            if (std::find(intResult.syns.begin(), intResult.syns.end(), arg0) == intResult.syns.end()) {
                intResult.syns.emplace_back(arg0);
                for (StmtId stmt : stmts) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(std::to_string(stmt));
                    intResult.rows.emplace_back(resultEntry);
                }
            } else {
                int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    if (stmts.find(std::stoi(resultEntry[index])) != stmts.end()) {
                        updatedResult.emplace_back(resultEntry);
                    }
                }
                intResult.rows = updatedResult;
            }
        }

        /**
        * Evaluates a single if pattern clause on the given PKB where the input is one identifier.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateIfPatternClauseId(PKB::PKB& database, PatternClause& clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            Synonym arg0 = clause.getSynonym().value;
            VarId arg1 = database.varTable.getVarId(clause.getArgs().first.value);

            std::unordered_set<StmtId> stmts = database.patternKB.getIfPatternStmts(arg1);

            if (std::find(intResult.syns.begin(), intResult.syns.end(), arg0) == intResult.syns.end()) {
                intResult.syns.emplace_back(arg0);
                for (StmtId stmt : stmts) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(std::to_string(stmt));
                    intResult.rows.emplace_back(resultEntry);
                }
            } else {
                int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    if (stmts.find(std::stoi(resultEntry[index])) != stmts.end()) {
                        updatedResult.emplace_back(resultEntry);
                    }
                }
                intResult.rows = updatedResult;
            }
        }

        /**
        * Evaluates a single if pattern clause on the given PKB where the input is one synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateIfPatternClauseSyn(PKB::PKB& database, PatternClause& clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            Synonym arg0 = clause.getSynonym().value;
            Synonym arg1 = clause.getArgs().first.value;

            bool foundSyn0 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg0) == intResult.syns.end());
            bool foundSyn1 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end());

            if (!foundSyn0 && !foundSyn1) {
                if (arg0 < arg1) {
                    intResult.syns.emplace_back(arg0);
                    intResult.syns.emplace_back(arg1);
                } else {
                    intResult.syns.emplace_back(arg1);
                    intResult.syns.emplace_back(arg0);
                }

                std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(StmtType::IF);
                for (StmtId stmt : stmts) {
                    SIMPLE::IfStmt* ifStmt = dynamic_cast<SIMPLE::IfStmt*>(database.stmtTable.get(stmt).get());
                    std::unordered_set<VarId> vars = ifStmt->getCondExpr().getVarIds();
                    for (VarId var : vars) {
                        ClauseResultEntry resultEntry;
                        if (arg0 < arg1) {
                            resultEntry.emplace_back(std::to_string(stmt));
                            resultEntry.emplace_back(database.varTable.get(var));
                        } else {
                            resultEntry.emplace_back(database.varTable.get(var));
                            resultEntry.emplace_back(std::to_string(stmt));
                        }
                        intResult.rows.emplace_back(resultEntry);
                    }
                }
            } else if (foundSyn0 && !foundSyn1) {
                int index0 = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                intResult.syns.emplace_back(arg1);
                std::sort(intResult.syns.begin(), intResult.syns.end());
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    StmtId stmt = std::stoi(resultEntry[index0]);
                    if (database.stmtTable.get(stmt).get()->getType() == StmtType::IF) {
                        SIMPLE::IfStmt* ifStmt = dynamic_cast<SIMPLE::IfStmt*>(database.stmtTable.get(stmt).get());
                        std::unordered_set<VarId> vars = ifStmt->getCondExpr().getVarIds();
                        for (VarId var : vars) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index1, database.varTable.get(var));
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            } else if (!foundSyn0 && foundSyn1) {
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                intResult.syns.emplace_back(arg0);
                std::sort(intResult.syns.begin(), intResult.syns.end());
                int index0 = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                std::unordered_set<StmtId> ifStmts = database.patternKB.getAllIfStmtsWithCtrlVars();
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    VarId var = database.varTable.getVarId(resultEntry[index1]);
                    for (StmtId stmt : ifStmts) {
                        SIMPLE::IfStmt* ifStmt = dynamic_cast<SIMPLE::IfStmt*>(database.stmtTable.get(stmt).get());
                        if (ifStmt->getCondExpr().getVarIds().count(var) > 0) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index0, std::to_string(stmt));
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            } else if (foundSyn0 && foundSyn1) {
                int index0 = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    StmtId stmt = std::stoi(resultEntry[index0]);
                    VarId var = database.varTable.getVarId(resultEntry[index1]);
                    if (database.stmtTable.get(stmt).get()->getType() == StmtType::IF) {
                        SIMPLE::IfStmt* ifStmt = dynamic_cast<SIMPLE::IfStmt*>(database.stmtTable.get(stmt).get());
                        if (ifStmt->getCondExpr().getVarIds().count(var) > 0) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            }

        }

        void evaluateIfPatternClause(PKB::PKB& database, PatternClause clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            ArgType argType1 = clause.getArgs().first.type;

            if (argType1 == ArgType::WILDCARD) {
                // 1 wildcard
                evaluateIfPatternClauseWild(database, clause, intResult);
            } else if (argType1 == ArgType::IDENTIFIER) {
                // 1 identifier
                evaluateIfPatternClauseId(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM) {
                // 1 synonym
                evaluateIfPatternClauseSyn(database, clause, synonymTable, intResult);
            } else {
                SPA::LoggingUtils::LogErrorMessage("IfPatternEvaluator::evaluateIfPatternClause: Invalid ArgTypes for If Pattern clause. argType1 = %d\n", argType1);

            }
        }

    }
}