#include <string>
#include <unordered_map>
#include <unordered_set>

#include "AssignPatternEvaluator.h"
#include "LoggingUtils.h"

namespace PQL {
    namespace AssignPatternEvaluator {

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAssignPatternClauseWildWild(PKB::PKB& database, PatternClause &clause) {
            
            Synonym arg0 = clause.synonym;
            
            std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(StmtType::ASSIGN);
            
            ClauseResult clauseResult;
            for (StmtId stmt : stmts) {
                ClauseResultEntry resultEntry;
                resultEntry[arg0] = std::to_string(stmt);
                clauseResult.emplace_back(resultEntry);
            }

            return clauseResult;
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one wildcard and one pattern.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAssignPatternClauseWildPtn(PKB::PKB& database, PatternClause &clause) {
            Synonym arg0 = clause.synonym;
            Pattern arg2 = clause.getArgs().second.second;

            std::unordered_set<StmtId> stmts = database.patternKB.getAssignPatternStmts(arg2);

            ClauseResult clauseResult;
            for (StmtId stmt : stmts) {
                if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                    ClauseResultEntry resultEntry;
                    resultEntry[arg0] = std::to_string(stmt);
                    clauseResult.emplace_back(resultEntry);
                }
            }
            return clauseResult;
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one identifier and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAssignPatternClauseIdWild(PKB::PKB& database, PatternClause& clause) {
            Synonym arg0 = clause.synonym;
            VarName arg1 = clause.getArgs().first.second;
            VarId arg1Id = database.varTable.getVarId(arg1);

            std::unordered_set<StmtId> stmts = database.modifiesKB.getAllStmtsModifyVar(arg1Id);
            ClauseResult clauseResult;
            for (StmtId stmt : stmts) {
                if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                    ClauseResultEntry resultEntry;
                    resultEntry[arg0] = std::to_string(stmt);
                    clauseResult.emplace_back(resultEntry);
                }
            }

            return clauseResult;
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one identifier and one pattern.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAssignPatternClauseIdPtn(PKB::PKB& database, PatternClause& clause) {
            Synonym arg0 = clause.synonym;
            VarName arg1 = clause.getArgs().first.second;
            VarId arg1Id = database.varTable.getVarId(arg1);
            Pattern arg2 = clause.getArgs().second.second;

            std::unordered_set<StmtId> stmts1 = database.modifiesKB.getAllStmtsModifyVar(arg1Id);
            std::unordered_set<StmtId> stmts2 = database.patternKB.getAssignPatternStmts(arg2);

            // Find the intersection
            std::unordered_set<StmtId> stmts;
            for (StmtId stmt : stmts1) {
                if (stmts2.find(stmt) != stmts2.end()) {
                    stmts.insert(stmt);
                }
            }

            ClauseResult clauseResult;
            for (StmtId stmt : stmts) {
                if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                    ClauseResultEntry resultEntry;
                    resultEntry[arg0] = std::to_string(stmt);
                    clauseResult.emplace_back(resultEntry);
                }
            }

            return clauseResult;
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one synonym and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAssignPatternClauseSynWild(PKB::PKB& database, PatternClause& clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg0 = clause.synonym;
            Synonym arg1 = clause.getArgs().first.second;

            std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(StmtType::ASSIGN);

            // If synonym 'arg1' appeared on LHS, then it must have been modified by the assignment
            ClauseResult clauseResult;
            for (StmtId stmt : stmts) {
                if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                    std::unordered_set<VarId> vars = database.modifiesKB.getAllVarsModifiedByStmt(stmt);
                    for (VarId var : vars) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg0] = std::to_string(stmt);
                        resultEntry[arg1] = database.varTable.get(var);
                        clauseResult.emplace_back(resultEntry);
                    }
                }
            }

            return clauseResult;
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one synonym and one pattern.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAssignPatternClauseSynPtn(PKB::PKB& database, PatternClause& clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg0 = clause.synonym;
            Synonym arg1 = clause.getArgs().first.second;
            Pattern arg2 = clause.getArgs().second.second;

            std::unordered_set<StmtId> stmts1 = database.stmtTable.getStmtsByType(StmtType::ASSIGN);
            std::unordered_set<StmtId> stmts2 = database.patternKB.getAssignPatternStmts(arg2);

            // Find the intersection
            std::unordered_set<StmtId> stmts;
            for (StmtId stmt : stmts1) {
                if (stmts2.find(stmt) != stmts2.end()) {
                    stmts.insert(stmt);
                }
            }

            ClauseResult clauseResult;
            for (StmtId stmt : stmts) {
                if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                    std::unordered_set<VarId> vars = database.modifiesKB.getAllVarsModifiedByStmt(stmt);
                    for (VarId var : vars) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg0] = std::to_string(stmt);
                        resultEntry[arg1] = database.varTable.get(var);
                        clauseResult.emplace_back(resultEntry);
                    }
                }
            }

            return clauseResult;
        }

        ClauseResult evaluateAssignPatternClause(PKB::PKB& database, PatternClause clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {

            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;


            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // 2 wildcards
                return evaluateAssignPatternClauseWildWild(database, clause);
            } else if (argType1 == ArgType::WILDCARD &&
                (argType2 == ArgType::EXACT_PATTERN || argType2 == ArgType::INCLUSIVE_PATTERN)) {
                // 1 wildcard, 1 pattern
                return evaluateAssignPatternClauseWildPtn(database, clause);
            } else if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::WILDCARD) {
                // 1 identifier, 1 wildcard
                return evaluateAssignPatternClauseIdWild(database, clause);
            } else if (argType1 == ArgType::IDENTIFIER &&
                (argType2 == ArgType::EXACT_PATTERN || argType2 == ArgType::INCLUSIVE_PATTERN)) {
                // 1 identifier, 1 pattern
                return evaluateAssignPatternClauseIdPtn(database, clause);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // 1 synonym, 1 wildcard
                return evaluateAssignPatternClauseSynWild(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM &&
                (argType2 == ArgType::EXACT_PATTERN || argType2 == ArgType::INCLUSIVE_PATTERN)) {
                // 1 synonym, 1 pattern
                return evaluateAssignPatternClauseSynPtn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("AssignPatternEvaluator::evaluateAssignPatternClause: Invalid ArgTypes for Assign Pattern clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}