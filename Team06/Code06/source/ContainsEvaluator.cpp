#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ContainsEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace ContainsEvaluator {

        /**
        * Evaluates a single Contains clause on the given PKB where the inputs are one ProcName and one StmtId.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateContainsClauseIdInt(PKB::PKB& database, RelationClause clause) {
            ProcId arg1 = database.procTable.getProcId(clause.getArgs().first.second);
            StmtId arg2 = std::stoi(clause.getArgs().second.second);

            if (database.containsKB.contains(arg1, arg2)) {
                ClauseResultEntry resultEntry;
                resultEntry["_RESULT"] = "TRUE";
                return { resultEntry };
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single Contains clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateContainsClauseWildWild(PKB::PKB& database) {
            // There must be at least one procedure, and the procedure must contain at least one statement
            // So this will definitely evaluate to true
            ClauseResultEntry resultEntry;
            resultEntry["_RESULT"] = "TRUE";
            return { resultEntry };
        }

        /**
        * Evaluates a single Contains clause on the given PKB where the input contains a ProcName and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateContainsClauseIdWild(PKB::PKB& database, RelationClause clause) {
            ProcId arg1 = database.procTable.getProcId(clause.getArgs().first.second);
            if (arg1 != -1) {
                ClauseResultEntry resultEntry;
                resultEntry["_RESULT"] = "TRUE";
                return { resultEntry };
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single Contains clause on the given PKB where the input contains a wildcard and a StmtId.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateContainsClauseWildInt(PKB::PKB& database, RelationClause clause) {
            StmtId arg2 = std::stoi(clause.getArgs().second.second);
            if (1 <= arg2 && arg2 <= database.stmtTable.size()) {
                ClauseResultEntry resultEntry;
                resultEntry["_RESULT"] = "TRUE";
                return { resultEntry };
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single Contains clause on the given PKB where the input contains a procedure name and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateContainsClauseIdSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ProcId arg1 = database.procTable.getProcId(clause.getArgs().first.second);
            Synonym arg2 = clause.getArgs().second.second;

            std::unordered_set<StmtId> stmts = database.containsKB.getAllContains(arg1);
            ClauseResult clauseResult;
            for (StmtId stmt : stmts) {
                if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg2])) {
                    ClauseResultEntry resultEntry;
                    resultEntry[arg2] = std::to_string(stmt);
                    clauseResult.emplace_back(resultEntry);
                }
            }
            return clauseResult;
        }

        /**
        * Evaluates a single Contains clause on the given PKB where the input contains a synonym and a StmtId.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateContainsClauseSynInt(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg1 = clause.getArgs().first.second;
            StmtId arg2 = std::stoi(clause.getArgs().second.second);

            ProcId container = database.containsKB.getContainer(arg2);
            if (container != INVALID_PROC_ID) {
                ClauseResultEntry resultEntry;
                resultEntry[arg1] = database.procTable.get(container).getName();
                return { resultEntry };
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single Contains clause on the given PKB where the inputs contain a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateContainsClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                Synonym arg2 = clause.getArgs().second.second;

                // Case 1: Wildcard, Synonym
                ClauseResult clauseResult = {};
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg2] = std::to_string(i);
                        clauseResult.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            } else {
                Synonym arg1 = clause.getArgs().first.second;
                // Case 2: Synonym, Wildcard
                ClauseResult clauseResult = {};
                for (ProcId i = 1; i <= database.procTable.size(); i++) {
                    ClauseResultEntry resultEntry;
                    resultEntry[arg1] = database.procTable.get(i).getName();
                    clauseResult.emplace_back(resultEntry);
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Contains clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateContainsClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg1 = clause.getArgs().first.second;
            Synonym arg2 = clause.getArgs().second.second;

            ClauseResult clauseResult = {};
            for (ProcId i = 1; i <= database.procTable.size(); i++) {
                std::unordered_set<StmtId> stmts = database.containsKB.getAllContains(i);
                for (StmtId stmt : stmts) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg1] = database.procTable.get(i).getName();
                        resultEntry[arg2] = std::to_string(stmt);
                        clauseResult.emplace_back(resultEntry);
                    }
                }
            }
            return clauseResult;
        }

        ClauseResult evaluateContainsClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::INTEGER) {
                // One identifier and one statement number supplied
                return evaluateContainsClauseIdInt(database, clause);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                return evaluateContainsClauseWildWild(database);
            } else if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::WILDCARD) {
                // One identifier, one wildcard supplied
                return evaluateContainsClauseIdWild(database, clause);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One wildcard, one statement number supplied
                return evaluateContainsClauseWildInt(database, clause);
            } else if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::SYNONYM) {
                // One identifier, one synonym supplied
                return evaluateContainsClauseIdSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One identifier, one synonym supplied
                return evaluateContainsClauseSynInt(database, clause, synonymTable);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                return evaluateContainsClauseWildSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                return evaluateContainsClauseSynSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("ContainsEvaluator::evaluateContainsClause: Invalid ArgTypes for Contains clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}