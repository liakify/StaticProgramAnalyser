#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

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
            std::pair<Argument, Argument> args = clause.getArgs();
            ProcId arg1 = database.procTable.getProcId(args.first.value);
            StmtId arg2 = std::stoi(args.second.value);

            ClauseResult clauseResult;
            if (database.containsKB.contains(arg1, arg2)) {
                clauseResult.trueResult = true;
            }
            return clauseResult;
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

            ClauseResult clauseResult;
            clauseResult.trueResult = true;
            return clauseResult;
        }

        /**
        * Evaluates a single Contains clause on the given PKB where the input contains a ProcName and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateContainsClauseIdWild(PKB::PKB& database, RelationClause clause) {
            ProcId arg1 = database.procTable.getProcId(clause.getArgs().first.value);

            ClauseResult clauseResult;
            if (arg1 != -1) {
                clauseResult.trueResult = true;
            }
            return clauseResult;
        }

        /**
        * Evaluates a single Contains clause on the given PKB where the input contains a wildcard and a StmtId.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateContainsClauseWildInt(PKB::PKB& database, RelationClause clause) {
            StmtId arg2 = std::stoi(clause.getArgs().second.value);

            ClauseResult clauseResult;
            if (1 <= arg2 && arg2 <= database.stmtTable.size()) {
                clauseResult.trueResult = true;
            }
            return clauseResult;
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

            std::pair<Argument, Argument> args = clause.getArgs();
            ProcId arg1 = database.procTable.getProcId(args.first.value);
            Synonym arg2 = args.second.value;

            std::unordered_set<StmtId> stmts = database.containsKB.getAllContains(arg1);
            ClauseResult clauseResult;
            clauseResult.syns.emplace_back(arg2);
            for (StmtId stmt : stmts) {
                if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg2])) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(std::to_string(stmt));
                    clauseResult.rows.emplace_back(resultEntry);
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

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            StmtId arg2 = std::stoi(args.second.value);

            ProcId container = database.containsKB.getContainer(arg2);
            ClauseResult clauseResult;
            clauseResult.syns.emplace_back(arg1);
            if (container != INVALID_PROC_ID) {
                ClauseResultEntry resultEntry;
                resultEntry.emplace_back(database.procTable.get(container).getName());
                clauseResult.rows.emplace_back(resultEntry);
            }
            return clauseResult;
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

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                Synonym arg2 = args.second.value;

                // Case 1: Wildcard, Synonym
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(i));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            } else {
                Synonym arg1 = args.first.value;

                // Case 2: Synonym, Wildcard
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                for (ProcId i = 1; i <= database.procTable.size(); i++) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(database.procTable.get(i).getName());
                    clauseResult.rows.emplace_back(resultEntry);
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

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            Synonym arg2 = args.second.value;

            ClauseResult clauseResult = {};
            clauseResult.syns.emplace_back(arg1);
            clauseResult.syns.emplace_back(arg2);
            for (ProcId i = 1; i <= database.procTable.size(); i++) {
                std::unordered_set<StmtId> stmts = database.containsKB.getAllContains(i);
                for (StmtId stmt : stmts) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(database.procTable.get(i).getName());
                        resultEntry.emplace_back(std::to_string(stmt));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
            }
            return clauseResult;
        }

        ClauseResult evaluateContainsClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

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