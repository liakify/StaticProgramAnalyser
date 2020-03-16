#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ParentEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace ParentEvaluator {

        /**
        * Evaluates a single Parent clause on the given PKB where the inputs are two StmtIds.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateParentClauseIntInt(PKB::PKB& database, RelationClause clause) {
            StmtId arg1 = std::stoi(clause.getArgs().first.second);
            StmtId arg2 = std::stoi(clause.getArgs().second.second);

            if (database.parentKB.parent(arg1, arg2)) {
                ClauseResultEntry resultEntry;
                resultEntry["_RESULT"] = "TRUE";
                return { resultEntry };
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single Parent clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateParentClauseWildWild(PKB::PKB& database) {
            for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                if (database.parentKB.getParent(i) != 0) {
                    ClauseResultEntry resultEntry;
                    resultEntry["_RESULT"] = "TRUE";
                    return { resultEntry };
                }
            }
            return {};
        }

        /**
        * Evaluates a single Parent clause on the given PKB where the input contains a StmtId and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateParentClauseIntWild(PKB::PKB& database, RelationClause clause) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                if (database.parentKB.getDirectChildren(arg1).size() > 0) {
                    ClauseResultEntry resultEntry;
                    resultEntry["_RESULT"] = "TRUE";
                    return { resultEntry };
                } else {
                    return {};
                }
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(clause.getArgs().second.second);
                if (database.parentKB.getParent(arg2) != 0) {
                    ClauseResultEntry resultEntry;
                    resultEntry["_RESULT"] = "TRUE";
                    return { resultEntry };
                } else {
                    return {};
                }
            }
        }

        /**
        * Evaluates a single Parent clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateParentClauseIntSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
                // Case 1: Integer, Synonym
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                Synonym arg2 = clause.getArgs().second.second;

                std::unordered_set<StmtId> directChildren = database.parentKB.getDirectChildren(arg1);
                ClauseResult clauseResult;
                for (StmtId child : directChildren) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(child)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg2] = std::to_string(child);
                        clauseResult.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = clause.getArgs().first.second;
                StmtId arg2 = std::stoi(clause.getArgs().second.second);

                StmtId parent = database.parentKB.getParent(arg2);
                if (parent == 0) {
                    return {};
                } else {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(parent)->getType(), synonymTable[arg1])) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg1] = std::to_string(parent);
                        return { resultEntry };
                    } else {
                        return {};
                    }
                }
            }
        }

        /**
        * Evaluates a single Parent clause on the given PKB where the inputs contain a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateParentClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                Synonym arg2 = clause.getArgs().second.second;

                // Case 1: Wildcard, Synonym
                ClauseResult clauseResult = {};
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.parentKB.getParent(i) != 0 &&
                        SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg2] = std::to_string(i);
                        clauseResult.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            } else {
                Synonym arg1 = clause.getArgs().first.second;
                // Case 2: Synonym, Wildcard
                ClauseResult clauseResult = {};
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.parentKB.getDirectChildren(i).size() != 0 &&
                        SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1])) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg1] = std::to_string(i);
                        clauseResult.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Parent clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateParentClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg1 = clause.getArgs().first.second;
            Synonym arg2 = clause.getArgs().second.second;

            bool singleSynonym = (arg1 == arg2);

            ClauseResult clauseResult = {};
            for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                StmtId parent = database.parentKB.getParent(i);
                if (parent != 0) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(parent)->getType(), synonymTable[arg1]) &&
                        SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg2])) {
                        if (!singleSynonym) {
                            ClauseResultEntry resultEntry;
                            resultEntry[arg1] = std::to_string(parent);
                            resultEntry[arg2] = std::to_string(i);
                            clauseResult.emplace_back(resultEntry);
                        } else {
                            if (i == parent) {
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

        ClauseResult evaluateParentClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                return evaluateParentClauseIntInt(database, clause);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                return evaluateParentClauseWildWild(database);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                return evaluateParentClauseIntWild(database, clause);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                return evaluateParentClauseIntSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                return evaluateParentClauseWildSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                return evaluateParentClauseSynSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("ParentEvaluator::evaluateParentClause: Invalid ArgTypes for Parent clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}
