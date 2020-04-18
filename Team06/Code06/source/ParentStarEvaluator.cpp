#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "ParentStarEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace ParentStarEvaluator {

        /**
        * Evaluates a single Parent* clause on the given PKB where the inputs are two StmtIds.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateParentStarClauseIntInt(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            StmtId arg1 = std::stoi(args.first.value);
            StmtId arg2 = std::stoi(args.second.value);

            if (!database.parentKB.parentStar(arg1, arg2)) {
                intResult.rows.clear();
                intResult.trueResult = false;
            }

        }

        /**
        * Evaluates a single Parent* clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateParentStarClauseWildWild(PKB::PKB& database, ClauseResult& intResult) {
            if (!database.parentKB.hasParentRelation()) {
                intResult.rows.clear();
                intResult.trueResult = false;
            }

        }

        /**
        * Evaluates a single Parent* clause on the given PKB where the input contains a StmtId and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateParentStarClauseIntWild(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(args.first.value);
                if (database.parentKB.getDirectChildren(arg1).size() <= 0) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(args.second.value);
                ClauseResult clauseResult;
                if (database.parentKB.getParent(arg2) == 0) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            }
        }

        /**
        * Evaluates a single Parent* clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateParentStarClauseIntSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
                // Case 1: Integer, Synonym
                StmtId arg1 = std::stoi(args.first.value);
                Synonym arg2 = args.second.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end()) {
                    std::unordered_set<StmtId> children = database.parentKB.getAllChildren(arg1);
                    intResult.syns.emplace_back(arg2);
                    for (StmtId child : children) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(child)->getType(), synonymTable[arg2])) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(child));
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.parentKB.parentStar(arg1, std::stoi(resultEntry[index]))) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
    
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = args.first.value;
                StmtId arg2 = std::stoi(args.second.value);

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end()) {
                    std::unordered_set<StmtId> parents = database.parentKB.getAllParents(arg2);
                    intResult.syns.emplace_back(arg1);
                    for (StmtId parent : parents) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(parent)->getType(), synonymTable[arg1])) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(parent));
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.parentKB.parentStar(std::stoi(resultEntry[index]), arg2)) {
                            updatedResult.emplace_back(resultEntry);
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
        * Evaluates a single Parent* clause on the given PKB where the inputs contain a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateParentStarClauseWildSyn(PKB::PKB& database, RelationClause clause,
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
                        if (database.parentKB.getParent(i) != 0) {
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
                        if (database.parentKB.hasParent(std::stoi(resultEntry[index]))) {
                            updatedResult.emplace_back(resultEntry);
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
                        if (database.parentKB.getDirectChildren(i).size() > 0) {
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
                        if (database.parentKB.hasDirectChildren(std::stoi(resultEntry[index]))) {
                            updatedResult.emplace_back(resultEntry);
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
        * Evaluates a single Parent* clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateParentStarClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            Synonym arg2 = args.second.value;

            bool singleSynonym = (arg1 == arg2);

            bool foundSyn1 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end());
            bool foundSyn2 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end());

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
                    std::unordered_set<StmtId> parents = database.parentKB.getAllParents(i);
                    for (StmtId parent : parents) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(parent)->getType(), synonymTable[arg1]) &&
                            SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg2])) {
                            if (!singleSynonym) {
                                ClauseResultEntry resultEntry;
                                if (arg1 < arg2) {
                                    resultEntry.emplace_back(std::to_string(parent));
                                    resultEntry.emplace_back(std::to_string(i));
                                } else {
                                    resultEntry.emplace_back(std::to_string(i));
                                    resultEntry.emplace_back(std::to_string(parent));
                                }
                                intResult.rows.emplace_back(resultEntry);
                            } else {
                                if (i == parent) {
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
                    std::unordered_set<StmtId> stmts = database.parentKB.getAllChildren(std::stoi(resultEntry[index1]));
                    for (StmtId stmt : stmts) {
                        ClauseResultEntry newResultEntry(resultEntry);
                        newResultEntry.insert(newResultEntry.begin() + index2, std::to_string(stmt));
                        updatedResult.emplace_back(newResultEntry);
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
                    std::unordered_set<StmtId> stmts = database.parentKB.getAllParents(std::stoi(resultEntry[index2]));
                    for (StmtId stmt : stmts) {
                        ClauseResultEntry newResultEntry(resultEntry);
                        newResultEntry.insert(newResultEntry.begin() + index1, std::to_string(stmt));
                        updatedResult.emplace_back(newResultEntry);
                    }
                }
                intResult.rows = updatedResult;
            } else if (foundSyn1 && foundSyn2) {
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    if (database.parentKB.parentStar(std::stoi(resultEntry[index1]), std::stoi(resultEntry[index2]))) {
                        updatedResult.emplace_back(resultEntry);
                    }
                }
                intResult.rows = updatedResult;
            }
            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        void evaluateParentStarClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                evaluateParentStarClauseIntInt(database, clause, intResult);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                evaluateParentStarClauseWildWild(database, intResult);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                evaluateParentStarClauseIntWild(database, clause, intResult);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                evaluateParentStarClauseIntSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                evaluateParentStarClauseWildSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                evaluateParentStarClauseSynSyn(database, clause, synonymTable, intResult);
            } else {
                SPA::LoggingUtils::LogErrorMessage("ParentEvaluator::evaluateParentStarClause: Invalid ArgTypes for Parent* clause. argType1 = %d, argType2 = %d\n", argType1, argType2);

            }
        }

    }
}
