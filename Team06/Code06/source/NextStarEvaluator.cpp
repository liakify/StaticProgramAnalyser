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
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateNextStarClauseIntInt(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            StmtId arg1 = std::stoi(args.first.value);
            StmtId arg2 = std::stoi(args.second.value);

            if (!database.nextStar(arg1, arg2)) {
                intResult.rows.clear();
                intResult.trueResult = false;
            }

        }

        /**
        * Evaluates a single Next* clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateNextStarClauseWildWild(PKB::PKB& database, ClauseResult& intResult) {

            if (!database.hasNextRelation()) {
                intResult.rows.clear();
                intResult.trueResult = false;
            }

        }

        /**
        * Evaluates a single Next* clause on the given PKB where the inputs contain one StmtId and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateNextStarClauseIntWild(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(args.first.value);
                if (!database.hasNext(arg1)) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(args.second.value);
                if (!database.hasPrev(arg2)) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            }
        }

        /**
        * Evaluates a single Next* clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateNextStarClauseIntSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
                // Case 1: Integer, Synonym
                StmtId arg1 = std::stoi(args.first.value);
                Synonym arg2 = args.second.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end()) {
                    std::unordered_set<StmtId> stmts = database.nextStarGetAllNodes(arg1, NodeType::SUCCESSOR);
                    intResult.syns.emplace_back(arg2);
                    for (StmtId stmt : stmts) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg2])) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(stmt));
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.nextStar(arg1, std::stoi(resultEntry[index]))) {
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
                    std::unordered_set<StmtId> stmts = database.nextStarGetAllNodes(arg2, NodeType::PREDECESSOR);
                    intResult.syns.emplace_back(arg1);
                    for (StmtId stmt : stmts) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg1])) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(stmt));
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.nextStar(std::stoi(resultEntry[index]), arg2)) {
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
        * Evaluates a single Next* clause on the given PKB where the input contains a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateNextStarClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                // Case 1: Wildcard, Synonym
                Synonym arg2 = args.second.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg2);
                    for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                        if (database.hasPrev(i)) {
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
                        if (database.hasPrev(std::stoi(resultEntry[index]))) {
                            if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(std::stoi(resultEntry[index]))->getType(), synonymTable[arg2])) {
                                updatedResult.emplace_back(resultEntry);
                            }
                        }
                    }
                    intResult.rows = updatedResult;
                }
    
            } else {
                // Case 2: Synonym, Wildcard
                Synonym arg1 = args.first.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg1);
                    for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                        if (database.hasNext(i)) {
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
                        if (database.hasNext(std::stoi(resultEntry[index]))) {
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
        * Evaluates a single Next* clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateNextStarClauseSynSyn(PKB::PKB& database, RelationClause clause,
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
                } else if (arg1 < arg2) {
                    intResult.syns.emplace_back(arg1);
                    intResult.syns.emplace_back(arg2);
                } else {
                    intResult.syns.emplace_back(arg2);
                    intResult.syns.emplace_back(arg1);
                }

                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    std::unordered_set<StmtId> nextStarStmts = database.nextStarGetAllNodes(i, NodeType::SUCCESSOR);
                    for (StmtId nextStarStmt : nextStarStmts) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1]) &&
                            SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(nextStarStmt)->getType(), synonymTable[arg2])) {
                            if (!singleSynonym) {
                                ClauseResultEntry resultEntry;
                                if (arg1 < arg2) {
                                    resultEntry.emplace_back(std::to_string(i));
                                    resultEntry.emplace_back(std::to_string(nextStarStmt));
                                } else {
                                    resultEntry.emplace_back(std::to_string(nextStarStmt));
                                    resultEntry.emplace_back(std::to_string(i));
                                }
                                intResult.rows.emplace_back(resultEntry);
                            } else {
                                if (i == nextStarStmt) {
                                    ClauseResultEntry resultEntry;
                                    resultEntry.emplace_back(std::to_string(i));
                                    intResult.rows.emplace_back(resultEntry);
                                }
                            }
                        }
                    }
                    if (!nextStarStmts.empty()) {
                        database.nextStarSetProcessedAll(i, NodeType::PREDECESSOR);
                    }
                }

            } else if (foundSyn1 && !foundSyn2) {
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                intResult.syns.emplace_back(arg2);
                std::sort(intResult.syns.begin(), intResult.syns.end());
                int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    std::unordered_set<StmtId> stmts = database.nextStarGetAllNodes(std::stoi(resultEntry[index1]), NodeType::SUCCESSOR);
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
                    std::unordered_set<StmtId> stmts = database.nextStarGetAllNodes(std::stoi(resultEntry[index2]), NodeType::PREDECESSOR);
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
                    if (database.nextStar(std::stoi(resultEntry[index1]), std::stoi(resultEntry[index2]))) {
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

        void evaluateNextStarClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                evaluateNextStarClauseIntInt(database, clause, intResult);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                evaluateNextStarClauseWildWild(database, intResult);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                evaluateNextStarClauseIntWild(database, clause, intResult);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                evaluateNextStarClauseIntSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                evaluateNextStarClauseWildSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                evaluateNextStarClauseSynSyn(database, clause, synonymTable, intResult);
            } else {
                SPA::LoggingUtils::LogErrorMessage("NextEvaluator::evaluateNextClause: Invalid ArgTypes for Next clause. argType1 = %d, argType2 = %d\n", argType1, argType2);

            }
        }

    }
}
