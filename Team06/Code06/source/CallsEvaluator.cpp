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
                }
    
            } else {
                // Case 2: Wildcard, Identifier
                ProcId arg2 = database.procTable.getProcId(args.second.value);
                if (!database.callsKB.hasCaller(arg2)) {
                    intResult.rows.clear();
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

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);
                std::unordered_set<StmtId> directCallees = database.callsKB.getDirectNodes(arg1, NodeType::SUCCESSOR);
                for (ProcId callee : directCallees) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(database.procTable.get(callee).getName());
                    clauseResult.rows.emplace_back(resultEntry);
                }
    
            } else {
                // Case 2: Synonym, Integer
                Synonym arg1 = args.first.value;
                ProcId arg2 = database.procTable.getProcId(args.second.value);

                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                std::unordered_set<StmtId> directCallers = database.callsKB.getDirectNodes(arg2, NodeType::PREDECESSOR);
                for (ProcId caller : directCallers) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(database.procTable.get(caller).getName());
                    clauseResult.rows.emplace_back(resultEntry);
                }
    
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
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);
                std::unordered_set<ProcId> callees = database.callsKB.getAllCallees();
                for (ProcId callee : callees) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(database.procTable.get(callee).getName());
                    clauseResult.rows.emplace_back(resultEntry);
                }
    
            } else {
                Synonym arg1 = args.first.value;

                // Case 2: Synonym, Wildcard
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                std::unordered_set<ProcId> callers = database.callsKB.getAllCallers();
                for (ProcId caller : callers) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(database.procTable.get(caller).getName());
                    clauseResult.rows.emplace_back(resultEntry);
                }
    
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

            ClauseResult clauseResult;
            if (arg1 == arg2) {
                // In SIMPLE, a procedure should not be able to call itself.
    
            }

            if (arg1 < arg2) {
                clauseResult.syns.emplace_back(arg1);
                clauseResult.syns.emplace_back(arg2);
            } else {
                clauseResult.syns.emplace_back(arg2);
                clauseResult.syns.emplace_back(arg1);
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

                    clauseResult.rows.emplace_back(resultEntry);
                }
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
                evaluateCallsClauseWildWild(database);
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
