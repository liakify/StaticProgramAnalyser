#include <string>
#include <unordered_map>
#include <unordered_set>

#include "FollowsEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace FollowsEvaluator {

        /**
        * Evaluates a single Follows clause on the given PKB where the inputs are two StmtIds.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsClauseIntInt(PKB::PKB& database, RelationClause clause) {
            StmtId arg1 = std::stoi(clause.getArgs().first.second);
            StmtId arg2 = std::stoi(clause.getArgs().second.second);

            ClauseResult clauseResult;
            if (database.followsKB.follows(arg1, arg2)) {
                clauseResult.trueResult = true;
            } else {
                clauseResult.emptyResult = true;
            }
            return clauseResult;
        }

        /**
        * Evaluates a single Follows clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsClauseWildWild(PKB::PKB& database) {
            ClauseResult clauseResult;
            if (database.followsKB.hasFollowsRelation()) {
                clauseResult.trueResult = true;
            } else {
                clauseResult.emptyResult = true;
            }
            return clauseResult;
        }

        /**
        * Evaluates a single Follows clause on the given PKB where the input contains a StmtId and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsClauseIntWild(PKB::PKB& database, RelationClause clause) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                ClauseResult clauseResult;
                if (database.followsKB.getFollower(arg1) != 0) {
                    clauseResult.trueResult = true;
                } else {
                    clauseResult.emptyResult = true;
                }
                return clauseResult;
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(clause.getArgs().second.second);
                ClauseResult clauseResult;
                if (database.followsKB.getFollowing(arg2) != 0) {
                    clauseResult.trueResult = true;
                } else {
                    clauseResult.emptyResult = true;
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Follows clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsClauseIntSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
                // Case 1: Integer, Synonym
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                Synonym arg2 = clause.getArgs().second.second;

                ClauseResult clauseResult;
                StmtId follower = database.followsKB.getFollower(arg1);
                if (follower == 0) {
                    clauseResult.emptyResult = true;
                    return clauseResult;
                } else {
                    clauseResult.syns.emplace_back(arg2);
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(follower)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(follower));
                        clauseResult.rows.emplace_back(resultEntry);
                    } else {
                        clauseResult.emptyResult = true;
                    }
                    return clauseResult;
                }
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = clause.getArgs().first.second;
                StmtId arg2 = std::stoi(clause.getArgs().second.second);

                ClauseResult clauseResult;
                StmtId following = database.followsKB.getFollowing(arg2);
                if (following == 0) {
                    clauseResult.emptyResult = true;
                    return clauseResult;
                } else {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(following)->getType(), synonymTable[arg1])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(following));
                        clauseResult.rows.emplace_back(resultEntry);
                    } else {
                        clauseResult.emptyResult = true;
                    }
                    return clauseResult;
                }
            }
        }

        /**
        * Evaluates a single Follows clause on the given PKB where the inputs contain a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                Synonym arg2 = clause.getArgs().second.second;

                // Case 1: Wildcard, Synonym
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.followsKB.getFollowing(i) != 0 &&
                        SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(i));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            } else {
                Synonym arg1 = clause.getArgs().first.second;
                // Case 2: Synonym, Wildcard
                ClauseResult clauseResult;
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.followsKB.getFollower(i) != 0 &&
                        SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(i));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Follows clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateFollowsClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg1 = clause.getArgs().first.second;
            Synonym arg2 = clause.getArgs().second.second;

            bool singleSynonym = (arg1 == arg2);

            ClauseResult clauseResult;
            if (singleSynonym) {
                clauseResult.syns.emplace_back(arg1);
            } else if (arg1 < arg2) {
                clauseResult.syns.emplace_back(arg1);
                clauseResult.syns.emplace_back(arg2);
            } else {
                clauseResult.syns.emplace_back(arg2);
                clauseResult.syns.emplace_back(arg1);
            }

            for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                StmtId follower = database.followsKB.getFollower(i);
                if (follower != 0) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1]) &&
                        SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(follower)->getType(), synonymTable[arg2])) {
                        if (!singleSynonym) {
                            ClauseResultEntry resultEntry;
                            if (arg1 < arg2) {
                                resultEntry.emplace_back(std::to_string(i));
                                resultEntry.emplace_back(std::to_string(follower));
                            } else {
                                resultEntry.emplace_back(std::to_string(follower));
                                resultEntry.emplace_back(std::to_string(i));
                            }
                            clauseResult.rows.emplace_back(resultEntry);
                        } else {
                            if (i == follower) {
                                ClauseResultEntry resultEntry;
                                resultEntry.emplace_back(std::to_string(i));
                                clauseResult.rows.emplace_back(resultEntry);
                            }
                        }
                    }
                }
            }
            return clauseResult;
        }

        ClauseResult evaluateFollowsClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                return evaluateFollowsClauseIntInt(database, clause);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                return evaluateFollowsClauseWildWild(database);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                return evaluateFollowsClauseIntWild(database, clause);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                return evaluateFollowsClauseIntSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                return evaluateFollowsClauseWildSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                return evaluateFollowsClauseSynSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("FollowsEvaluator::evaluateFollowsClause: Invalid ArgTypes for Follows clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}