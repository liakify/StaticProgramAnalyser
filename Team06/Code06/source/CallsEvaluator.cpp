#include "CallsEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
	namespace CallsEvaluator {

		/**
		* Evaluates a single Calls clause on the given PKB where the inputs are two procedure names.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateCallsClauseIdId(PKB::PKB& database, RelationClause clause) {
			ProcId arg1 = database.procTable.getProcId(clause.getArgs().first.second);
			ProcId arg2 = database.procTable.getProcId(clause.getArgs().second.second);

			if (database.callsKB.calls(arg1, arg2)) {
				ClauseResultEntry resultEntry;
				resultEntry["_RESULT"] = "TRUE";
				return { resultEntry };
			}
			else {
				return {};
			}
		}

		/**
		* Evaluates a single Calls clause on the given PKB where the inputs are two wildcards.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateCallsClauseWildWild(PKB::PKB& database) {
			
			if (database.callsKB.hasCallsRelation()) {
				ClauseResultEntry resultEntry;
				resultEntry["_RESULT"] = "TRUE";
				return { resultEntry };
			}
			else {
				return {};
			}
		}

		/**
		* Evaluates a single Calls clause on the given PKB where the input contains a procedure name and a wildcard.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateCallsClauseIdWild(PKB::PKB& database, RelationClause clause) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::WILDCARD) {
				// Case 1: Identifier, Wildcard
				ProcId arg1 = database.procTable.getProcId(clause.getArgs().first.second);
				if (database.callsKB.hasCallee(arg1)) {
					ClauseResultEntry resultEntry;
					resultEntry["_RESULT"] = "TRUE";
					return { resultEntry };
				}
				else {
					return {};
				}
			}
			else {
				// Case 2: Wildcard, Identifier
				ProcId arg2 = database.procTable.getProcId(clause.getArgs().second.second);
				if (database.callsKB.hasCaller(arg2) != 0) {
					ClauseResultEntry resultEntry;
					resultEntry["_RESULT"] = "TRUE";
					return { resultEntry };
				}
				else {
					return {};
				}
			}
		}

		/**
		* Evaluates a single Calls clause on the given PKB where the input contains a procedure name and a synonym.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateCallsClauseIdSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<std::string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::SYNONYM) {
				// Case 1: Identifier, Synonym
				ProcId arg1 = database.procTable.getProcId(clause.getArgs().first.second);
				Synonym arg2 = clause.getArgs().second.second;

				std::unordered_set<StmtId> directCallees = database.callsKB.getDirectNodes(arg1, NodeType::SUCCESSOR);
				ClauseResult clauseResult;
				for (ProcId callee : directCallees) {
					ClauseResultEntry resultEntry;
					resultEntry[arg2] = database.procTable.get(callee).getName();
					clauseResult.emplace_back(resultEntry);
				}
				return clauseResult;
			}
			else {
				// Case 2: Synonym, Integer
				Synonym arg1 = clause.getArgs().first.second;
				ProcId arg2 = database.procTable.getProcId(clause.getArgs().second.second);

				std::unordered_set<StmtId> directCallers = database.callsKB.getDirectNodes(arg2, NodeType::PREDECESSOR);
				ClauseResult clauseResult;
				for (ProcId caller : directCallers) {
					ClauseResultEntry resultEntry;
					resultEntry[arg1] = database.procTable.get(caller).getName();
					clauseResult.emplace_back(resultEntry);
				}
				return clauseResult;
			}
		}

		/**
		* Evaluates a single Calls clause on the given PKB where the inputs contain a wildcard and a synonym.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateCallsClauseWildSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<std::string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
				Synonym arg2 = clause.getArgs().second.second;

				// Case 1: Wildcard, Synonym
				std::unordered_set<ProcId> callees = database.callsKB.getAllCallees();

				ClauseResult clauseResult = {};
				for (ProcId callee : callees) {
					ClauseResultEntry resultEntry;
					resultEntry[arg2] = database.procTable.get(callee).getName();
					clauseResult.emplace_back(resultEntry);
				}
				return clauseResult;
			}
			else {
				Synonym arg1 = clause.getArgs().first.second;

				// Case 2: Synonym, Wildcard
				std::unordered_set<ProcId> callers = database.callsKB.getAllCallers();

				ClauseResult clauseResult = {};
				for (ProcId caller : callers) {
					ClauseResultEntry resultEntry;
					resultEntry[arg1] = database.procTable.get(caller).getName();
					clauseResult.emplace_back(resultEntry);
				}
				return clauseResult;
			}
		}

		/**
		* Evaluates a single Calls clause on the given PKB where the input contains two synonyms.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateCallsClauseSynSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<std::string, DesignEntity>& synonymTable) {
			Synonym arg1 = clause.getArgs().first.second;
			Synonym arg2 = clause.getArgs().second.second;

			if (arg1 == arg2) {
				// In SIMPLE, a procedure should not be able to call itself.
				return {};
			}

			ClauseResult clauseResult = {};
			for (ProcId caller = 1; caller <= database.procTable.size(); caller++) {
				std::unordered_set<ProcId> callees = database.callsKB.getDirectNodes(caller, NodeType::SUCCESSOR);
				for (ProcId callee : callees) {
					ClauseResultEntry resultEntry;
					resultEntry[arg1] = database.procTable.get(caller).getName();
					resultEntry[arg2] = database.procTable.get(callee).getName();
					clauseResult.emplace_back(resultEntry);
				}
			}
			return clauseResult;
		}

		ClauseResult evaluateCallsClause(PKB::PKB& database, RelationClause clause,
			unordered_map<std::string, DesignEntity>& synonymTable) {

			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::IDENTIFIER) {
				// Two identifiers supplied
				return evaluateCallsClauseIdId(database, clause);
			}
			else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
				// Two wildcards supplied
				return evaluateCallsClauseWildWild(database);
			}
			else if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::WILDCARD ||
				argType1 == ArgType::WILDCARD && argType2 == ArgType::IDENTIFIER) {
				// One identifier, one wildcard supplied
				return evaluateCallsClauseIdWild(database, clause);
			}
			else if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::SYNONYM ||
				argType1 == ArgType::SYNONYM && argType2 == ArgType::IDENTIFIER) {
				// One identifier, one synonym
				return evaluateCallsClauseIdSyn(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
				argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
				// One synonym, one wildcard
				return evaluateCallsClauseWildSyn(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
				// Two synonyms
				return evaluateCallsClauseSynSyn(database, clause, synonymTable);
			}
			else {
				SPA::LoggingUtils::LogErrorMessage("CallsEvaluator::evaluateCallsClause: Invalid ArgTypes for Calls clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
				return {};
			}
		}

	}
}
