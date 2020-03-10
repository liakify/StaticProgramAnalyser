#include "FollowsStarEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace FollowsStarEvaluator {

        /**
        * Evaluates a single Follows* clause on the given PKB where the inputs are two StmtIds.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsStarClauseIntInt(PKB::PKB& database, RelationClause clause) {
            StmtId arg1 = std::stoi(clause.getArgs().first.second);
            StmtId arg2 = std::stoi(clause.getArgs().second.second);

            if (database.followsKB.followStar(arg1, arg2)) {
                ClauseResultEntry resultEntry;
                resultEntry["_RESULT"] = "TRUE";
                return { resultEntry };
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single Follows* clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsStarClauseWildWild(PKB::PKB& database) {
            for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                if (database.followsKB.getFollower(i) != 0) {
                    ClauseResultEntry resultEntry;
                    resultEntry["_RESULT"] = "TRUE";
                    return { resultEntry };
                }
            }
            return {};
        }

        /**
        * Evaluates a single Follows* clause on the given PKB where the input contains a StmtId and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsStarClauseIntWild(PKB::PKB& database, RelationClause clause) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                if (database.followsKB.getFollower(arg1) != 0) {
                    ClauseResultEntry resultEntry;
                    resultEntry["_RESULT"] = "TRUE";
                    return { resultEntry };
                } else {
                    return {};
                }
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(clause.getArgs().second.second);
                if (database.followsKB.getFollowing(arg2) != 0) {
                    ClauseResultEntry resultEntry;
                    resultEntry["_RESULT"] = "TRUE";
                    return { resultEntry };
                } else {
                    return {};
                }
            }
        }

        /**
        * Evaluates a single Follows* clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsStarClauseIntSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
                // Case 1: Integer, Synonym
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                Synonym arg2 = clause.getArgs().second.second;

                ClauseResult clauseResult = {};
                std::unordered_set<StmtId> followers = database.followsKB.getAllFollowers(arg1);
                for (StmtId follower : followers) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(follower)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg2] = std::to_string(follower);
                        clauseResult.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = clause.getArgs().first.second;
                StmtId arg2 = std::stoi(clause.getArgs().second.second);

                ClauseResult clauseResult = {};
                std::unordered_set<StmtId> following = database.followsKB.getAllFollowing(arg2);
                for (StmtId follow : following) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(follow)->getType(), synonymTable[arg1])) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg1] = std::to_string(follow);
                        clauseResult.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Follows* clause on the given PKB where the inputs contain a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsStarClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                Synonym arg2 = clause.getArgs().second.second;

                // Case 1: Wildcard, Synonym
                ClauseResult clauseResult = {};
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.followsKB.getFollowing(i) != 0) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg2])) {
                            ClauseResultEntry resultEntry;
                            resultEntry[arg2] = std::to_string(i);
                            clauseResult.emplace_back(resultEntry);
                        }
                    }
                }
                return clauseResult;
            } else {
                Synonym arg1 = clause.getArgs().first.second;
                // Case 2: Synonym, Wildcard
                ClauseResult clauseResult = {};
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.followsKB.getFollower(i) != 0) {
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
        * Evaluates a single Follows* clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsStarClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg1 = clause.getArgs().first.second;
            Synonym arg2 = clause.getArgs().second.second;

            bool singleSynonym = (arg1 == arg2);

            ClauseResult clauseResult = {};
            for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                std::unordered_set<StmtId> followers = database.followsKB.getAllFollowers(i);
                for (StmtId follower : followers) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1]) &&
                        SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(follower)->getType(), synonymTable[arg2])) {
                        if (!singleSynonym) {
                            ClauseResultEntry resultEntry;
                            resultEntry[arg1] = std::to_string(i);
                            resultEntry[arg2] = std::to_string(follower);
                            clauseResult.emplace_back(resultEntry);
                        } else {
                            if (i == follower) {
                                ClauseResultEntry resultEntry;
                                resultEntry[arg1] = std::to_string(i);
                                clauseResult.emplace_back(resultEntry);
                            }
                        }
                    }
                }
            }
            return clauseResult;
        }

        ClauseResult evaluateFollowsStarClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                return evaluateFollowsStarClauseIntInt(database, clause);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                return evaluateFollowsStarClauseWildWild(database);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                return evaluateFollowsStarClauseIntWild(database, clause);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                return evaluateFollowsStarClauseIntSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                return evaluateFollowsStarClauseWildSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                return evaluateFollowsStarClauseSynSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("FollowsEvaluator::evaluateFollowsClause: Invalid ArgTypes for Follows* clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}