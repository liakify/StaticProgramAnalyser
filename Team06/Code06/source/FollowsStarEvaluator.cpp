#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

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
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateFollowsStarClauseIntInt(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            StmtId arg1 = std::stoi(args.first.value);
            StmtId arg2 = std::stoi(args.second.value);

            if (!database.followsKB.followStar(arg1, arg2)) {
                intResult.rows.clear();
                intResult.trueResult = false;
            }

        }

        /**
        * Evaluates a single Follows* clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateFollowsStarClauseWildWild(PKB::PKB& database, ClauseResult& intResult) {
            if (!database.followsKB.hasFollowsRelation()) {
                intResult.rows.clear();
                intResult.trueResult = false;
            }

        }

        /**
        * Evaluates a single Follows* clause on the given PKB where the input contains a StmtId and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateFollowsStarClauseIntWild(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(args.first.value);
                if (database.followsKB.getFollower(arg1) == 0) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(args.second.value);
                if (database.followsKB.getFollowing(arg2) == 0) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            }
        }

        /**
        * Evaluates a single Follows* clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateFollowsStarClauseIntSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
                // Case 1: Integer, Synonym
                StmtId arg1 = std::stoi(args.first.value);
                Synonym arg2 = args.second.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg2);
                    std::unordered_set<StmtId> followers = database.followsKB.getAllFollowers(arg1);
                    for (StmtId follower : followers) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(follower)->getType(), synonymTable[arg2])) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(follower));
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.followsKB.followStar(arg1, std::stoi(resultEntry[index]))) {
                            if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(std::stoi(resultEntry[index]))->getType(), synonymTable[arg2])) {
                                updatedResult.emplace_back(resultEntry);
                            }
                        }
                    }
                    intResult.rows = updatedResult;
                }
    
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = args.first.value;
                StmtId arg2 = std::stoi(args.second.value);

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg1);
                    std::unordered_set<StmtId> following = database.followsKB.getAllFollowing(arg2);
                    for (StmtId follow : following) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(follow)->getType(), synonymTable[arg1])) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(follow));
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.followsKB.followStar(std::stoi(resultEntry[index]), arg2)) {
                            if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(std::stoi(resultEntry[index]))->getType(), synonymTable[arg1])) {
                                updatedResult.emplace_back(resultEntry);
                            }
                        }
                    }
                    intResult.rows = updatedResult;
                }
            }
            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        /**
        * Evaluates a single Follows* clause on the given PKB where the inputs contain a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateFollowsStarClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                Synonym arg2 = args.second.value;

                // Case 1: Wildcard, Synonym
                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg2);
                    for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                        if (database.followsKB.getFollowing(i) != 0) {
                            if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg2])) {
                                ClauseResultEntry resultEntry;
                                resultEntry.emplace_back(std::to_string(i));
                                intResult.rows.emplace_back(resultEntry);
                            }
                        }
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.followsKB.isFollowing(std::stoi(resultEntry[index]))) {
                            if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(std::stoi(resultEntry[index]))->getType(), synonymTable[arg2])) {
                                updatedResult.emplace_back(resultEntry);
                            }
                        }
                    }
                    intResult.rows = updatedResult;
                }
    
            } else {
                Synonym arg1 = args.first.value;

                // Case 2: Synonym, Wildcard
                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg1);
                    for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                        if (database.followsKB.getFollower(i) != 0) {
                            if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1])) {
                                ClauseResultEntry resultEntry;
                                resultEntry.emplace_back(std::to_string(i));
                                intResult.rows.emplace_back(resultEntry);
                            }
                        }
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.followsKB.hasFollower(std::stoi(resultEntry[index]))) {
                            if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(std::stoi(resultEntry[index]))->getType(), synonymTable[arg1])) {
                                updatedResult.emplace_back(resultEntry);
                            }
                        }
                    }
                    intResult.rows = updatedResult;
                }
            }
            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        /**
        * Evaluates a single Follows* clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateFollowsStarClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            Synonym arg2 = args.second.value;

            bool singleSynonym = (arg1 == arg2);

            bool foundSyn1 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) != intResult.syns.end());
            bool foundSyn2 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) != intResult.syns.end());
            
            if (!foundSyn1 && !foundSyn2) {
                if (singleSynonym) {
                    intResult.syns.emplace_back(arg1);
                } else {
                    if (arg1 < arg2) {
                        intResult.syns.emplace_back(arg1);
                        intResult.syns.emplace_back(arg2);
                    } else {
                        intResult.syns.emplace_back(arg2);
                        intResult.syns.emplace_back(arg1);
                    }
                }

                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    std::unordered_set<StmtId> followers = database.followsKB.getAllFollowers(i);
                    for (StmtId follower : followers) {
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
                                intResult.rows.emplace_back(resultEntry);
                            } else {
                                if (i == follower) {
                                    ClauseResultEntry resultEntry;
                                    resultEntry.emplace_back(std::to_string(i));
                                    intResult.rows.emplace_back(resultEntry);
                                }
                            }
                        }
                    }
                }
            } else if (foundSyn1 && !foundSyn2) {
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                intResult.syns.emplace_back(arg2);
                std::sort(intResult.syns.begin(), intResult.syns.end());
                int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    std::unordered_set<StmtId> stmts = database.followsKB.getAllFollowers(std::stoi(resultEntry[index1]));
                    for (StmtId stmt : stmts) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(std::stoi(resultEntry[index1]))->getType(), synonymTable[arg1]) &&
                            SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg2])) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index2, std::to_string(stmt));
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            } else if (!foundSyn1 && foundSyn2) {
                int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                intResult.syns.emplace_back(arg1);
                std::sort(intResult.syns.begin(), intResult.syns.end());
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    std::unordered_set<StmtId> stmts = database.followsKB.getAllFollowing(std::stoi(resultEntry[index2]));
                    for (StmtId stmt : stmts) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg1]) &&
                            SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(std::stoi(resultEntry[index2]))->getType(), synonymTable[arg2])) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index1, std::to_string(stmt));
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            } else if (foundSyn1 && foundSyn2) {
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    if (database.followsKB.followStar(std::stoi(resultEntry[index1]), std::stoi(resultEntry[index2]))) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(std::stoi(resultEntry[index1]))->getType(), synonymTable[arg1]) &&
                            SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(std::stoi(resultEntry[index2]))->getType(), synonymTable[arg2])) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            }
            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        void evaluateFollowsStarClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                evaluateFollowsStarClauseIntInt(database, clause, intResult);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                evaluateFollowsStarClauseWildWild(database, intResult);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                evaluateFollowsStarClauseIntWild(database, clause, intResult);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                evaluateFollowsStarClauseIntSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                evaluateFollowsStarClauseWildSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                evaluateFollowsStarClauseSynSyn(database, clause, synonymTable, intResult);
            } else {
                SPA::LoggingUtils::LogErrorMessage("FollowsEvaluator::evaluateFollowsClause: Invalid ArgTypes for Follows* clause. argType1 = %d, argType2 = %d\n", argType1, argType2);

            }
        }

    }
}