#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "CallsEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace CallsEvaluator {

        /**
        * Evaluates a single Calls clause on the given PKB where the inputs are two procedure names.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateCallsClauseIdId(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ProcId arg1 = database.procTable.getProcId(args.first.value);
            ProcId arg2 = database.procTable.getProcId(args.second.value);

            if (!database.callsKB.calls(arg1, arg2)) {
                intResult.rows.clear();
                intResult.trueResult = false;
            }

        }

        /**
        * Evaluates a single Calls clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateCallsClauseWildWild(PKB::PKB& database, ClauseResult& intResult) {

            if (!database.callsKB.hasCallsRelation()) {
                intResult.rows.clear();
                intResult.trueResult = false;
            }

        }

        /**
        * Evaluates a single Calls clause on the given PKB where the input contains a procedure name and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateCallsClauseIdWild(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::WILDCARD) {
                // Case 1: Identifier, Wildcard
                ProcId arg1 = database.procTable.getProcId(args.first.value);
                if (!database.callsKB.hasCallee(arg1)) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            } else {
                // Case 2: Wildcard, Identifier
                ProcId arg2 = database.procTable.getProcId(args.second.value);
                if (!database.callsKB.hasCaller(arg2)) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            }
        }

        /**
        * Evaluates a single Calls clause on the given PKB where the input contains a procedure name and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateCallsClauseIdSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::SYNONYM) {
                // Case 1: Identifier, Synonym
                ProcId arg1 = database.procTable.getProcId(args.first.value);
                Synonym arg2 = args.second.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg2);
                    std::unordered_set<StmtId> directCallees = database.callsKB.getDirectNodes(arg1, NodeType::SUCCESSOR);
                    for (ProcId callee : directCallees) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(database.procTable.get(callee).getName());
                        intResult.rows.emplace_back(resultEntry);
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.callsKB.calls(arg1, database.procTable.getProcId(resultEntry[index]))) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
    
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = args.first.value;
                ProcId arg2 = database.procTable.getProcId(args.second.value);

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg1);
                    std::unordered_set<StmtId> directCallers = database.callsKB.getDirectNodes(arg2, NodeType::PREDECESSOR);
                    for (ProcId caller : directCallers) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(database.procTable.get(caller).getName());
                        intResult.rows.emplace_back(resultEntry);
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.callsKB.calls(database.procTable.getProcId(resultEntry[index]), arg2)) {
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
        * Evaluates a single Calls clause on the given PKB where the inputs contain a wildcard and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateCallsClauseWildSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
                Synonym arg2 = args.second.value;

                // Case 1: Wildcard, Synonym
                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg2);
                    std::unordered_set<ProcId> callees = database.callsKB.getAllCallees();
                    for (ProcId callee : callees) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(database.procTable.get(callee).getName());
                        intResult.rows.emplace_back(resultEntry);
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.callsKB.hasCaller(database.procTable.getProcId(resultEntry[index]))) {
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
                    std::unordered_set<ProcId> callers = database.callsKB.getAllCallers();
                    for (ProcId caller : callers) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(database.procTable.get(caller).getName());
                        intResult.rows.emplace_back(resultEntry);
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.callsKB.hasCallee(database.procTable.getProcId(resultEntry[index]))) {
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
        * Evaluates a single Calls clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateCallsClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            Synonym arg2 = args.second.value;

            if (arg1 == arg2) {
                // In SIMPLE, a procedure should not be able to call itself.
                intResult.rows.clear();
                intResult.trueResult = false;
                return;
            }

            bool foundSyn1 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) != intResult.syns.end());
            bool foundSyn2 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) != intResult.syns.end());

            if (!foundSyn1 && !foundSyn2) {
                if (arg1 < arg2) {
                    intResult.syns.emplace_back(arg1);
                    intResult.syns.emplace_back(arg2);
                } else {
                    intResult.syns.emplace_back(arg2);
                    intResult.syns.emplace_back(arg1);
                }
                for (ProcId caller = 1; caller <= database.procTable.size(); caller++) {
                    std::unordered_set<ProcId> callees = database.callsKB.getDirectNodes(caller, NodeType::SUCCESSOR);
                    for (ProcId callee : callees) {
                        ClauseResultEntry resultEntry;
                        if (arg1 < arg2) {
                            resultEntry.emplace_back(database.procTable.get(caller).getName());
                            resultEntry.emplace_back(database.procTable.get(callee).getName());
                        } else {
                            resultEntry.emplace_back(database.procTable.get(callee).getName());
                            resultEntry.emplace_back(database.procTable.get(caller).getName());
                        }

                        intResult.rows.emplace_back(resultEntry);
                    }
                }
            } else if (foundSyn1 && !foundSyn2) {
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                intResult.syns.emplace_back(arg2);
                std::sort(intResult.syns.begin(), intResult.syns.end());
                int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    std::unordered_set<ProcId> procs = database.callsKB.getDirectNodes(database.procTable.getProcId(resultEntry[index1]), NodeType::SUCCESSOR);
                    for (ProcId proc : procs) {
                        ClauseResultEntry newResultEntry(resultEntry);
                        newResultEntry.insert(newResultEntry.begin() + index2, database.procTable.get(proc).getName());
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
                    std::unordered_set<ProcId> procs = database.callsKB.getDirectNodes(database.procTable.getProcId(resultEntry[index2]), NodeType::PREDECESSOR);
                    for (ProcId proc : procs) {
                        ClauseResultEntry newResultEntry(resultEntry);
                        newResultEntry.insert(newResultEntry.begin() + index1, database.procTable.get(proc).getName());
                        updatedResult.emplace_back(newResultEntry);
                    }
                }
                intResult.rows = updatedResult;
            } else if (foundSyn1 && foundSyn2) {
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    if (database.callsKB.calls(database.procTable.getProcId(resultEntry[index1]), database.procTable.getProcId(resultEntry[index2]))) {
                        updatedResult.emplace_back(resultEntry);
                    }
                }
                intResult.rows = updatedResult;
            }

            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }

        }

        void evaluateCallsClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::IDENTIFIER) {
                // Two identifiers supplied
                evaluateCallsClauseIdId(database, clause, intResult);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                evaluateCallsClauseWildWild(database, intResult);
            } else if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::IDENTIFIER) {
                // One identifier, one wildcard supplied
                evaluateCallsClauseIdWild(database, clause, intResult);
            } else if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::IDENTIFIER) {
                // One identifier, one synonym
                evaluateCallsClauseIdSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                evaluateCallsClauseWildSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                evaluateCallsClauseSynSyn(database, clause, synonymTable, intResult);
            } else {
                SPA::LoggingUtils::LogErrorMessage("CallsEvaluator::evaluateCallsClause: Invalid ArgTypes for Calls clause. argType1 = %d, argType2 = %d\n", argType1, argType2);

            }
        }

    }
}
