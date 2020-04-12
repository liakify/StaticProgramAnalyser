#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "NextStarEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace NextStarEvaluator {

        /**
        * Evaluates a single Next* clause on the given PKB where the inputs are two StmtIds.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateNextStarClauseIntInt(PKB::PKB& database, RelationClause clause) {
            StmtId arg1 = std::stoi(clause.getArgs().first.second);
            StmtId arg2 = std::stoi(clause.getArgs().second.second);

            if (database.nextStar(arg1, arg2)) {
                ClauseResultEntry resultEntry;
                resultEntry["_RESULT"] = "TRUE";
                return { resultEntry };
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single Next* clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateNextStarClauseWildWild(PKB::PKB& database) {
            if (database.hasNextRelation()) {
                ClauseResultEntry resultEntry;
                resultEntry["_RESULT"] = "TRUE";
                return { resultEntry };
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single Next* clause on the given PKB where the inputs contain one StmtId and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateNextStarClauseIntWild(PKB::PKB& database, RelationClause clause) {
            std::pair<std::pair<ArgType, std::string>, std::pair<ArgType, std::string>> args = clause.getArgs();

            ArgType argType1 = args.first.first;
            ArgType argType2 = args.second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                if (database.hasNext(arg1)) {
                    ClauseResultEntry resultEntry;
                    resultEntry["_RESULT"] = "TRUE";
                    return { resultEntry };
                } else {
                    return {};
                }
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(clause.getArgs().second.second);
                if (database.hasPrev(arg2)) {
                    ClauseResultEntry resultEntry;
                    resultEntry["_RESULT"] = "TRUE";
                    return { resultEntry };
                } else {
                    return {};
                }
            }
        }

        /**
        * Evaluates a single Next* clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateNextStarClauseIntSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
                // Case 1: Integer, Synonym
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                Synonym arg2 = clause.getArgs().second.second;

                std::unordered_set<StmtId> stmts = database.nextStarGetAllNodes(arg1, NodeType::SUCCESSOR);
                ClauseResult clauseResult;
                for (StmtId stmt : stmts) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg2] = std::to_string(stmt);
                        clauseResult.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = clause.getArgs().first.second;
                StmtId arg2 = std::stoi(clause.getArgs().second.second);

                std::unordered_set<StmtId> stmts = database.nextStarGetAllNodes(arg2, NodeType::PREDECESSOR);
                ClauseResult clauseResult;
                for (StmtId stmt : stmts) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg1])) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg1] = std::to_string(stmt);
                        clauseResult.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Next* clause on the given PKB where the input contains a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateNextStarClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                // Case 1: Wildcard, Synonym
                Synonym arg2 = clause.getArgs().second.second;

                ClauseResult clauseResult;
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.hasPrev(i)) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg2])) {
                            ClauseResultEntry resultEntry;
                            resultEntry[arg2] = std::to_string(i);
                            clauseResult.emplace_back(resultEntry);
                        }
                    }
                }
                return clauseResult;
            } else {
                // Case 2: Synonym, Wildcard
                Synonym arg1 = clause.getArgs().first.second;

                ClauseResult clauseResult;
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.hasNext(i)) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1])) {
                            ClauseResultEntry resultEntry;
                            resultEntry[arg1] = std::to_string(i);
                            clauseResult.emplace_back(resultEntry);
                        }
                    }
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Next* clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateNextStarClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg1 = clause.getArgs().first.second;
            Synonym arg2 = clause.getArgs().second.second;

            bool singleSynonym = (arg1 == arg2);

            ClauseResult clauseResult;
            for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                std::unordered_set<StmtId> nextStarStmts = database.nextStarGetAllNodes(i, NodeType::SUCCESSOR);
                for (StmtId nextStarStmt : nextStarStmts) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1]) &&
                        SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(nextStarStmt)->getType(), synonymTable[arg2])) {
                        if (!singleSynonym) {
                            ClauseResultEntry resultEntry;
                            resultEntry[arg1] = std::to_string(i);
                            resultEntry[arg2] = std::to_string(nextStarStmt);
                            clauseResult.emplace_back(resultEntry);
                        } else {
                            if (i == nextStarStmt) {
                                ClauseResultEntry resultEntry;
                                resultEntry[arg1] = std::to_string(i);
                                clauseResult.emplace_back(resultEntry);
                            }
                        }
                    }
                }
                if (!nextStarStmts.empty()) {
                    database.nextStarSetProcessedAll(i, NodeType::PREDECESSOR);
                }
            }

            return clauseResult;
        }

        ClauseResult evaluateNextStarClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                return evaluateNextStarClauseIntInt(database, clause);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                return evaluateNextStarClauseWildWild(database);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                return evaluateNextStarClauseIntWild(database, clause);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                return evaluateNextStarClauseIntSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                return evaluateNextStarClauseWildSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                return evaluateNextStarClauseSynSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("NextEvaluator::evaluateNextClause: Invalid ArgTypes for Next clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}
