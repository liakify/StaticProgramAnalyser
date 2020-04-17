#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "AffectsEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace AffectsEvaluator {

        /**
        * Evaluates a single Affects clause on the given PKB where the inputs are two StmtIds.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsClauseIntInt(PKB::PKB& database, RelationClause clause) {
            std::pair<Argument, Argument> args = clause.getArgs();
            StmtId arg1 = std::stoi(args.first.value);
            StmtId arg2 = std::stoi(args.second.value);

            ClauseResult clauseResult;
            if (database.affects(arg1, arg2)) {
                clauseResult.trueResult = true;
            }

            return clauseResult;
        }

        /**
        * Evaluates a single Affects clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsClauseWildWild(PKB::PKB& database) {

            ClauseResult clauseResult;
            for (StmtId i : database.stmtTable.getStmtsByType(StmtType::ASSIGN)) {
                if (database.affectsGetDirectNodes(i, NodeType::SUCCESSOR).size() > 0) {
                    clauseResult.trueResult = true;
                    return clauseResult;
                }
            }
            return clauseResult;
        }

        /**
        * Evaluates a single Affects clause on the given PKB where the inputs contain one StmtId and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsClauseIntWild(PKB::PKB& database, RelationClause clause) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            ClauseResult clauseResult;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(args.first.value);
                if (database.affectsGetDirectNodes(arg1, NodeType::SUCCESSOR).size() > 0) {
                    clauseResult.trueResult = true;
                }
                return clauseResult;
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(args.second.value);
                if (database.affectsGetDirectNodes(arg2, NodeType::PREDECESSOR).size() > 0) {
                    clauseResult.trueResult = true;
                }
            }

            return clauseResult;
        }

        /**
        * Evaluates a single Affects clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsClauseIntSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
                // Case 1: Integer, Synonym
                StmtId arg1 = std::stoi(args.first.value);
                Synonym arg2 = args.second.value;

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);

                std::unordered_set<StmtId> stmts = database.affectsGetDirectNodes(arg1, NodeType::SUCCESSOR);
                for (StmtId stmt : stmts) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(std::to_string(stmt));
                    clauseResult.rows.emplace_back(resultEntry);
                }

                return clauseResult;
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = args.first.value;
                StmtId arg2 = std::stoi(args.second.value);

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);

                std::unordered_set<StmtId> stmts = database.affectsGetDirectNodes(arg2, NodeType::PREDECESSOR);
                for (StmtId stmt : stmts) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(std::to_string(stmt));
                    clauseResult.rows.emplace_back(resultEntry);
                }

                return clauseResult;
            }
        }

        /**
        * Evaluates a single Affects clause on the given PKB where the input contains a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                // Case 1: Wildcard, Synonym
                Synonym arg2 = args.second.value;

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);

                for (StmtId i : database.stmtTable.getStmtsByType(StmtType::ASSIGN)) {
                    if (database.affectsGetDirectNodes(i, NodeType::PREDECESSOR).size() > 0) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(i));
                        clauseResult.rows.emplace_back(resultEntry);
                    }

                    database.affectsSetProcessedDirect(i, NodeType::SUCCESSOR);
                }
                database.setAffectsFullyComputed();
                return clauseResult;
            } else {
                // Case 2: Synonym, Wildcard
                Synonym arg1 = args.first.value;

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);

                for (StmtId i : database.stmtTable.getStmtsByType(StmtType::ASSIGN)) {
                    if (database.affectsGetDirectNodes(i, NodeType::SUCCESSOR).size() > 0) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(i));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                    database.affectsSetProcessedDirect(i, NodeType::PREDECESSOR);
                }
                database.setAffectsFullyComputed();
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Affects clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAffectsClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            Synonym arg2 = args.second.value;

            bool singleSynonym = (arg1 == arg2);

            ClauseResult clauseResult;
            if (singleSynonym) {
                clauseResult.syns.emplace_back(arg1);
            } else {
                if (arg1 < arg2) {
                    clauseResult.syns.emplace_back(arg1);
                    clauseResult.syns.emplace_back(arg2);
                } else {
                    clauseResult.syns.emplace_back(arg2);
                    clauseResult.syns.emplace_back(arg1);
                }
            }

            for (StmtId i : database.stmtTable.getStmtsByType(StmtType::ASSIGN)) {
                std::unordered_set<StmtId> stmts = database.affectsGetDirectNodes(i, NodeType::SUCCESSOR);
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
                database.affectsSetProcessedDirect(i, NodeType::PREDECESSOR);
            }
            database.setAffectsFullyComputed();
            return clauseResult;
        }

        ClauseResult evaluateAffectsClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                return evaluateAffectsClauseIntInt(database, clause);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                return evaluateAffectsClauseWildWild(database);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                return evaluateAffectsClauseIntWild(database, clause);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                return evaluateAffectsClauseIntSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                return evaluateAffectsClauseWildSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                return evaluateAffectsClauseSynSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("AffectsEvaluator::evaluateAffectsClause: Invalid ArgTypes for Affects clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}
