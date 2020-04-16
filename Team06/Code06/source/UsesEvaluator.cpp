#include <string>
#include <unordered_map>
#include <unordered_set>

#include "UsesEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace UsesEvaluator {

        /**
        * Evaluates a single Uses clause on the given PKB where the inputs one StmtId and one identifier.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateUsesClauseIntIdId(PKB::PKB& database, RelationClause clause) {
            ArgType argType1 = clause.getArgs().first.first;

            if (argType1 == ArgType::INTEGER) {
                // Case 1: Statement number provided
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                VarId arg2 = database.varTable.getVarId(clause.getArgs().second.second);

                ClauseResult clauseResult;
                if (database.usesKB.stmtUses(arg1, arg2)) {
                    clauseResult.trueResult = true;
                }
                return clauseResult;
            } else {
                // Case 2: Procedure name provided
                ProcId arg1 = database.procTable.getProcId(clause.getArgs().first.second);
                VarId arg2 = database.varTable.getVarId(clause.getArgs().second.second);

                ClauseResult clauseResult;
                if (database.usesKB.procUses(arg1, arg2)) {
                    clauseResult.trueResult = true;
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Uses clause on the given PKB where the input contains a StmtId and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateUsesClauseIntIdWild(PKB::PKB& database, RelationClause clause) {
            ArgType argType1 = clause.getArgs().first.first;

            if (argType1 == ArgType::INTEGER) {
                // Case 1: Statement number provided
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                ClauseResult clauseResult;
                if (database.usesKB.getAllVarsUsedByStmt(arg1).size() > 0) {
                    clauseResult.trueResult = true;
                }
                return clauseResult;
            } else {
                // Case 2: Procedure name provided
                ProcId arg1 = database.procTable.getProcId(clause.getArgs().first.second);
                ClauseResult clauseResult;
                if (database.usesKB.getAllVarsUsedByProc(arg1).size() > 0) {
                    clauseResult.trueResult = true;
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Uses clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateUsesClauseIntIdSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;

            if (argType1 == ArgType::INTEGER) {
                // Case 1: Statement number provided
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                Synonym arg2 = clause.getArgs().second.second;

                std::unordered_set<VarId> usedVars = database.usesKB.getAllVarsUsedByStmt(arg1);
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);
                for (VarId var : usedVars) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(database.varTable.get(var));
                    clauseResult.rows.emplace_back(resultEntry);
                }

                return clauseResult;
            } else {
                // Case 2: Procedure name provided
                ProcId arg1 = database.procTable.getProcId(clause.getArgs().first.second);
                Synonym arg2 = clause.getArgs().second.second;

                std::unordered_set<VarId> usedVars = database.usesKB.getAllVarsUsedByProc(arg1);
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);
                for (VarId var : usedVars) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(database.varTable.get(var));
                    clauseResult.rows.emplace_back(resultEntry);
                }

                return clauseResult;
            }
        }

        /**
        * Evaluates a single Uses clause on the given PKB where the input contains one synonym and one identifier.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateUsesClauseSynId(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            Synonym arg1 = clause.getArgs().first.second;
            VarId arg2 = database.varTable.getVarId(clause.getArgs().second.second);

            if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                // Case 1: Synonym is a procedure
                std::unordered_set<StmtId> usingProcs = database.usesKB.getAllProcUsingVar(arg2);
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                for (ProcId proc : usingProcs) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(database.procTable.get(proc).getName());
                    clauseResult.rows.emplace_back(resultEntry);
                }
                return clauseResult;
            } else {
                // Case 2: Synonym is a statement
                std::unordered_set<StmtId> usingStmts = database.usesKB.getAllStmtsUsingVar(arg2);
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                for (StmtId stmt : usingStmts) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg1])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(stmt));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Uses clause on the given PKB where the inputs contain a synonym and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateUsesClauseSynWild(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            Synonym arg1 = clause.getArgs().first.second;

            if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                // Case 1: Synonym is a procedure
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                for (ProcId i = 1; i <= database.procTable.size(); i++) {
                    if (database.usesKB.getAllVarsUsedByProc(i).size() > 0) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(database.procTable.get(i).getName());
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            } else {
                // Case 2: Synonym is a statement
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.usesKB.getAllVarsUsedByStmt(i).size() > 0) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1])) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(i));
                            clauseResult.rows.emplace_back(resultEntry);
                        }
                    }
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Uses clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateUsesClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg1 = clause.getArgs().first.second;
            Synonym arg2 = clause.getArgs().second.second;

            ClauseResult clauseResult;
            if (arg1 < arg2) {
                clauseResult.syns.emplace_back(arg1);
                clauseResult.syns.emplace_back(arg2);
            } else {
                clauseResult.syns.emplace_back(arg2);
                clauseResult.syns.emplace_back(arg1);
            }
            if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                for (StmtId i = 1; i <= database.procTable.size(); i++) {
                    std::unordered_set<VarId> usedVars = database.usesKB.getAllVarsUsedByProc(i);
                    for (VarId var : usedVars) {
                        ClauseResultEntry resultEntry;
                        if (arg1 < arg2) {
                            resultEntry.emplace_back(database.procTable.get(i).getName());
                            resultEntry.emplace_back(database.varTable.get(var));
                        } else {
                            resultEntry.emplace_back(database.varTable.get(var));
                            resultEntry.emplace_back(database.procTable.get(i).getName());
                        }
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            } else {
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1])) {
                        std::unordered_set<VarId> usedVars = database.usesKB.getAllVarsUsedByStmt(i);
                        for (VarId var : usedVars) {
                            ClauseResultEntry resultEntry;
                            if (arg1 < arg2) {
                                resultEntry.emplace_back(std::to_string(i));
                                resultEntry.emplace_back(database.varTable.get(var));
                            } else {
                                resultEntry.emplace_back(database.varTable.get(var));
                                resultEntry.emplace_back(std::to_string(i));
                            }
                            clauseResult.rows.emplace_back(resultEntry);
                        }
                    }
                }
                return clauseResult;
            }
        }

        ClauseResult evaluateUsesClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if ((argType1 == ArgType::INTEGER || argType1 == ArgType::IDENTIFIER) && argType2 == ArgType::IDENTIFIER) {
                // One statement number/procedure name, one identifier supplied
                return evaluateUsesClauseIntIdId(database, clause);
            } else if ((argType1 == ArgType::INTEGER || argType1 == ArgType::IDENTIFIER) && argType2 == ArgType::WILDCARD) {
                // One statement number/procedure name, one wildcard supplied
                return evaluateUsesClauseIntIdWild(database, clause);
            } else if ((argType1 == ArgType::INTEGER || argType1 == ArgType::IDENTIFIER) && argType2 == ArgType::SYNONYM) {
                // One statement number/procedure name, one synonym
                return evaluateUsesClauseIntIdSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::IDENTIFIER) {
                // One synonym, one identifier
                return evaluateUsesClauseSynId(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                return evaluateUsesClauseSynWild(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                return evaluateUsesClauseSynSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("UsesEvaluator::evaluateUsesClause: Invalid ArgTypes for Uses clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}
