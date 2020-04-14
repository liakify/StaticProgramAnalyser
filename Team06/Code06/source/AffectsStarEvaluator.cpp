#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "AffectsStarEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace AffectsStarEvaluator {

        /**
        * Evaluates a single Affects* clause on the given PKB where the inputs are two StmtIds.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsStarClauseIntInt(PKB::PKB& database, RelationClause clause) {
            StmtId arg1 = std::stoi(clause.getArgs().first.second);
            StmtId arg2 = std::stoi(clause.getArgs().second.second);

            ClauseResult clauseResult;
            if (database.affectsStar(arg1, arg2)) {
                clauseResult.trueResult = true;
            } else {
                clauseResult.emptyResult = true;
            }

            return clauseResult;
        }

        /**
        * Evaluates a single Affects* clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsStarClauseWildWild(PKB::PKB& database) {

            ClauseResult clauseResult;
            for (StmtId i : database.stmtTable.getStmtsByType(StmtType::ASSIGN)) {
                if (database.affectsGetDirectNodes(i, NodeType::SUCCESSOR).size() > 0) {
                    clauseResult.trueResult = true;
                    return clauseResult;
                }
            }
            clauseResult.emptyResult = true;
            return clauseResult;
        }

        /**
        * Evaluates a single Affects* clause on the given PKB where the inputs contain one StmtId and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsStarClauseIntWild(PKB::PKB& database, RelationClause clause) {
            std::pair<std::pair<ArgType, std::string>, std::pair<ArgType, std::string>> args = clause.getArgs();

            ArgType argType1 = args.first.first;
            ArgType argType2 = args.second.first;

            ClauseResult clauseResult;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                if (database.affectsGetDirectNodes(arg1, NodeType::SUCCESSOR).size() > 0) {
                    clauseResult.trueResult = true;
                } else {
                    clauseResult.trueResult = false;
                }
                return clauseResult;
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(clause.getArgs().second.second);
                if (database.affectsGetDirectNodes(arg2, NodeType::PREDECESSOR).size() > 0) {
                    clauseResult.trueResult = true;
                } else {
                    clauseResult.trueResult = false;
                }
            }

            return clauseResult;
        }

        /**
        * Evaluates a single Affects* clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsStarClauseIntSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
                // Case 1: Integer, Synonym
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                Synonym arg2 = clause.getArgs().second.second;

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);

                std::unordered_set<StmtId> stmts = database.affectsStarGetAllNodes(arg1, NodeType::SUCCESSOR);
                ClauseResult clauseResult;
                for (StmtId stmt : stmts) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(std::to_string(stmt));
                    clauseResult.rows.emplace_back(resultEntry);
                }

                clauseResult.emptyResult = clauseResult.rows.empty();

                return clauseResult;
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = clause.getArgs().first.second;
                StmtId arg2 = std::stoi(clause.getArgs().second.second);

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);

                std::unordered_set<StmtId> stmts = database.affectsStarGetAllNodes(arg2, NodeType::PREDECESSOR);
                ClauseResult clauseResult;
                for (StmtId stmt : stmts) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(std::to_string(stmt));
                    clauseResult.rows.emplace_back(resultEntry);
                }

                clauseResult.emptyResult = clauseResult.rows.empty();

                return clauseResult;
            }
        }

        /**
        * Evaluates a single Affects* clause on the given PKB where the input contains a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsStarClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                // Case 1: Wildcard, Synonym
                Synonym arg2 = clause.getArgs().second.second;

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);

                for (StmtId i : database.stmtTable.getStmtsByType(StmtType::ASSIGN)) {
                    if (database.affectsGetDirectNodes(i, NodeType::PREDECESSOR).size() > 0) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(i));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                    database.affectsStarSetProcessedAll(i, NodeType::SUCCESSOR);
                }
                clauseResult.emptyResult = clauseResult.rows.empty();
                return clauseResult;
            } else {
                // Case 2: Synonym, Wildcard
                Synonym arg1 = clause.getArgs().first.second;

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);

                for (StmtId i : database.stmtTable.getStmtsByType(StmtType::ASSIGN)) {
                    if (database.affectsGetDirectNodes(i, NodeType::SUCCESSOR).size() > 0) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(i));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                    database.affectsStarSetProcessedAll(i, NodeType::PREDECESSOR);
                }
                clauseResult.emptyResult = clauseResult.rows.empty();
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Affects* clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsStarClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg1 = clause.getArgs().first.second;
            Synonym arg2 = clause.getArgs().second.second;

            bool singleSynonym = (arg1 == arg2);

            ClauseResult clauseResult;
            if (singleSynonym) {
                clauseResult.rows.emplace_back(arg1);
            } else {
                if (arg1 < arg2) {
                    clauseResult.rows.emplace_back(arg1);
                    clauseResult.rows.emplace_back(arg2);
                } else {
                    clauseResult.rows.emplace_back(arg2);
                    clauseResult.rows.emplace_back(arg1);
                }

            }

            ClauseResult clauseResult;
            for (StmtId i : database.stmtTable.getStmtsByType(StmtType::ASSIGN)) {
                std::unordered_set<StmtId> stmts = database.affectsStarGetAllNodes(i, NodeType::SUCCESSOR);
                for (StmtId stmt : stmts) {
                    if (!singleSynonym) {
                        ClauseResultEntry resultEntry;
                        if (arg1 < arg2) {
                            resultEntry.emplace_back(std::to_string(i));
                            resultEntry.emplace_back(std::to_string(stmt));
                        } else {
                            resultEntry.emplace_back(std::to_string(stmt));
                            resultEntry.emplace_back(std::to_string(i));
                        }
                        clauseResult.rows.emplace_back(resultEntry);
                    } else {
                        if (i == stmt) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(i));
                            clauseResult.rows.emplace_back(resultEntry);
                        }
                    }
                }
                database.affectsStarSetProcessedAll(i, NodeType::PREDECESSOR);
            }
            clauseResult.emptyResult = clauseResult.rows.empty();
            return clauseResult;
        }

        ClauseResult evaluateAffectsStarClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                return evaluateAffectsStarClauseIntInt(database, clause);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                return evaluateAffectsStarClauseWildWild(database);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                return evaluateAffectsStarClauseIntWild(database, clause);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                return evaluateAffectsStarClauseIntSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                return evaluateAffectsStarClauseWildSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                return evaluateAffectsStarClauseSynSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("AffectsStarEvaluator::evaluateAffectsStarClause: Invalid ArgTypes for AffectsStar clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}
