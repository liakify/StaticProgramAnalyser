#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "NextEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace NextEvaluator {

        /**
        * Evaluates a single Next clause on the given PKB where the inputs are two StmtIds.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        ClauseResult evaluateNextClauseIntInt(PKB::PKB& database, RelationClause clause) {
            std::pair<Argument, Argument> args = clause.getArgs();
            StmtId arg1 = std::stoi(args.first.value);
            StmtId arg2 = std::stoi(args.second.value);

            ClauseResult clauseResult;
            if (database.next(arg1, arg2)) {
                clauseResult.trueResult = true;
            }
            return clauseResult;
        }

        /**
        * Evaluates a single Next clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        ClauseResult evaluateNextClauseWildWild(PKB::PKB& database) {
            ClauseResult clauseResult;
            if (database.hasNextRelation()) {
                clauseResult.trueResult = true;
            }
            return clauseResult;
        }

        /**
        * Evaluates a single Next clause on the given PKB where the inputs contain one StmtId and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        ClauseResult evaluateNextClauseIntWild(PKB::PKB& database, RelationClause clause) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(args.first.value);
                ClauseResult clauseResult;
                if (database.hasNext(arg1)) {
                    clauseResult.trueResult = true;
                }
                return clauseResult;
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(args.second.value);
                ClauseResult clauseResult;
                if (database.hasPrev(arg2)) {
                    clauseResult.trueResult = true;
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Next clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        ClauseResult evaluateNextClauseIntSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
                // Case 1: Integer, Synonym
                StmtId arg1 = std::stoi(args.first.value);
                Synonym arg2 = args.second.value;

                std::unordered_set<StmtId> stmts = database.nextGetDirectNodes(arg1, NodeType::SUCCESSOR);
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);
                for (StmtId stmt : stmts) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg2])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(stmt));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = args.first.value;
                StmtId arg2 = std::stoi(args.second.value);

                std::unordered_set<StmtId> stmts = database.nextGetDirectNodes(arg2, NodeType::PREDECESSOR);
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                for (StmtId stmt : stmts) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg1])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(stmt));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Next clause on the given PKB where the input contains a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        ClauseResult evaluateNextClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                // Case 1: Wildcard, Synonym
                Synonym arg2 = args.second.value;

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.hasPrev(i)) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg2])) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(i));
                            clauseResult.rows.emplace_back(resultEntry);
                        }
                    }
                }
                return clauseResult;
            } else {
                // Case 2: Synonym, Wildcard
                Synonym arg1 = args.first.value;

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.hasNext(i)) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1])) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(i));
                            clauseResult.rows.emplace_back(resultEntry);
                        }
                    }
                }
                return clauseResult;
            }
        }

        /**
        * Evaluates a single Next clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        ClauseResult evaluateNextClauseSynSyn(PKB::PKB& database, RelationClause clause,
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
                std::unordered_set<StmtId> nextStmts = database.nextGetDirectNodes(i, NodeType::SUCCESSOR);
                for (StmtId nextStmt : nextStmts) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1]) &&
                        SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(nextStmt)->getType(), synonymTable[arg2])) {
                        if (!singleSynonym) {
                            ClauseResultEntry resultEntry;
                            if (arg1 < arg2) {
                                resultEntry.emplace_back(std::to_string(i));
                                resultEntry.emplace_back(std::to_string(nextStmt));
                            } else {
                                resultEntry.emplace_back(std::to_string(nextStmt));
                                resultEntry.emplace_back(std::to_string(i));
                            }
                            clauseResult.rows.emplace_back(resultEntry);
                        } else {
                            if (i == nextStmt) {
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

        ClauseResult evaluateNextClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                return evaluateNextClauseIntInt(database, clause);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                return evaluateNextClauseWildWild(database);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                return evaluateNextClauseIntWild(database, clause);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                return evaluateNextClauseIntSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                return evaluateNextClauseWildSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                return evaluateNextClauseSynSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("NextEvaluator::evaluateNextClause: Invalid ArgTypes for Next clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}
