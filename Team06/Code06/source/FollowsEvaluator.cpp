#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

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
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateFollowsClauseIntInt(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            StmtId arg1 = std::stoi(args.first.value);
            StmtId arg2 = std::stoi(args.second.value);

            if (!database.followsKB.follows(arg1, arg2)) {
                intResult.rows.clear();
            }

        }

        /**
        * Evaluates a single Follows clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateFollowsClauseWildWild(PKB::PKB& database, ClauseResult& intResult) {
            if (!database.followsKB.hasFollowsRelation()) {
                intResult.rows.clear();
            }
        }

        /**
        * Evaluates a single Follows clause on the given PKB where the input contains a StmtId and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        * @return   The result of the evaluation.
        */
        void evaluateFollowsClauseIntWild(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(args.first.value);
                if (database.followsKB.getFollower(arg1) == 0) {
                    intResult.rows.clear();
                }
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(args.second.value);
                if (database.followsKB.getFollowing(arg2) == 0) {
                    intResult.rows.clear();
                }
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

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
                // Case 1: Integer, Synonym
                StmtId arg1 = std::stoi(args.first.value);
                Synonym arg2 = args.second.value;

                ClauseResult clauseResult;
                StmtId follower = database.followsKB.getFollower(arg1);
                if (follower == 0) {
                    return clauseResult;
                } else {
                    clauseResult.syns.emplace_back(arg2);
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(follower)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(follower));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                    return clauseResult;
                }
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = args.first.value;
                StmtId arg2 = std::stoi(args.second.value);

                ClauseResult clauseResult;
                StmtId following = database.followsKB.getFollowing(arg2);
                if (following == 0) {
                    return clauseResult;
                } else {
                    clauseResult.syns.emplace_back(arg1);
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(following)->getType(), synonymTable[arg1])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(following));
                        clauseResult.rows.emplace_back(resultEntry);
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

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                Synonym arg2 = args.second.value;

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
                Synonym arg1 = args.first.value;

                // Case 2: Synonym, Wildcard
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
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

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            Synonym arg2 = args.second.value;

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
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                return evaluateFollowsClauseIntInt(database, clause, intResult);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                return evaluateFollowsClauseWildWild(database, intResult);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                return evaluateFollowsClauseIntWild(database, clause, intResult);
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