#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "AffectsStarEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace AffectsStarEvaluator {

        /**
        * Evaluates a single Affects* clause on the given PKB where the inputs are two StmtIds.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAffectsStarClauseIntInt(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            StmtId arg1 = std::stoi(args.first.value);
            StmtId arg2 = std::stoi(args.second.value);

            if (!database.affectsStar(arg1, arg2)) {
                intResult.rows.clear();
            }


        }

        /**
        * Evaluates a single Affects* clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAffectsStarClauseWildWild(PKB::PKB& database, ClauseResult& intResult) {

            bool success = false;
            for (StmtId i : database.stmtTable.getStmtsByType(StmtType::ASSIGN)) {
                if (database.affectsGetDirectNodes(i, NodeType::SUCCESSOR).size() > 0) {
                    success = true;
                    break;
                }
            }
            if (!success) {
                intResult.rows.clear();
            }

        }

        /**
        * Evaluates a single Affects* clause on the given PKB where the inputs contain one StmtId and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAffectsStarClauseIntWild(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(args.first.value);
                if (database.affectsGetDirectNodes(arg1, NodeType::SUCCESSOR).size() <= 0) {
                    intResult.rows.clear();
                }
    
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(args.second.value);
                if (database.affectsGetDirectNodes(arg2, NodeType::PREDECESSOR).size() <= 0) {
                    intResult.rows.clear();
                }
            }


        }

        /**
        * Evaluates a single Affects* clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAffectsStarClauseIntSyn(PKB::PKB& database, RelationClause clause,
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

                    std::unordered_set<StmtId> stmts = database.affectsStarGetAllNodes(arg1, NodeType::SUCCESSOR);
                    for (StmtId stmt : stmts) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(stmt));
                        intResult.rows.emplace_back(resultEntry);
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.affectsStar(arg1, std::stoi(resultEntry[index]))) {
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
                    intResult.syns.emplace_back(arg1);

                    std::unordered_set<StmtId> stmts = database.affectsStarGetAllNodes(arg2, NodeType::PREDECESSOR);
                    for (StmtId stmt : stmts) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(stmt));
                        intResult.rows.emplace_back(resultEntry);
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.affectsStar(std::stoi(resultEntry[index]), arg2)) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
    
            }
        }

        /**
        * Evaluates a single Affects* clause on the given PKB where the input contains a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAffectsStarClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                // Case 1: Wildcard, Synonym
                Synonym arg2 = args.second.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg2);

                    for (StmtId i : database.stmtTable.getStmtsByType(StmtType::ASSIGN)) {
                        if (database.affectsGetDirectNodes(i, NodeType::PREDECESSOR).size() > 0) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(i));
                            intResult.rows.emplace_back(resultEntry);
                        }
                        database.affectsStarSetProcessedAll(i, NodeType::SUCCESSOR);
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.affectsGetDirectNodes(std::stoi(resultEntry[index]), NodeType::PREDECESSOR).size() > 0) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
    
            } else {
                // Case 2: Synonym, Wildcard
                Synonym arg1 = args.first.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg1);

                    for (StmtId i : database.stmtTable.getStmtsByType(StmtType::ASSIGN)) {
                        if (database.affectsGetDirectNodes(i, NodeType::SUCCESSOR).size() > 0) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(i));
                            intResult.rows.emplace_back(resultEntry);
                        }
                        database.affectsStarSetProcessedAll(i, NodeType::PREDECESSOR);
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.affectsGetDirectNodes(std::stoi(resultEntry[index]), NodeType::SUCCESSOR).size() > 0) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
    
            }
        }

        /**
        * Evaluates a single Affects* clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAffectsStarClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

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
                std::unordered_set<StmtId> stmts = database.affectsStarGetAllNodes(i, NodeType::SUCCESSOR);
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
                database.affectsStarSetProcessedAll(i, NodeType::PREDECESSOR);
            }

        }

        ClauseResult evaluateAffectsStarClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                evaluateAffectsStarClauseIntInt(database, clause, intResult);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                evaluateAffectsStarClauseWildWild(database, intResult);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                evaluateAffectsStarClauseIntWild(database, clause, intResult);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                evaluateAffectsStarClauseIntSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                evaluateAffectsStarClauseWildSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                evaluateAffectsStarClauseSynSyn(database, clause, synonymTable, intResult);
            } else {
                SPA::LoggingUtils::LogErrorMessage("AffectsStarEvaluator::evaluateAffectsStarClause: Invalid ArgTypes for AffectsStar clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
            }
        }

    }
}
