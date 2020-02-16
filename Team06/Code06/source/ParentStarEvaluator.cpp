#include "ParentStarEvaluator.h"
#include "LoggingUtils.h"

namespace PQL {
	namespace ParentStarEvaluator {

		/**
		* Evaluates a single Parent* clause on the given PKB where the inputs are two StmtIds.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateParentStarClauseIntInt(PKB::PKB& database, RelationClause clause) {
			StmtId arg1 = std::stoi(clause.getArgs().first.second);
			StmtId arg2 = std::stoi(clause.getArgs().second.second);

			if (database.parentKB.parentStar(arg1, arg2)) {
				ClauseResultEntry resultEntry;
				resultEntry["_RESULT"] = "TRUE";
				return { resultEntry };
			}
			else {
				return {};
			}
		}

		/**
		* Evaluates a single Parent* clause on the given PKB where the inputs are two wildcards.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateParentStarClauseWildWild(PKB::PKB& database) {
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
		* Evaluates a single Parent* clause on the given PKB where the input contains a StmtId and a wildcard.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateParentStarClauseIntWild(PKB::PKB& database, RelationClause clause) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
				// Case 1: Integer, Wildcard
				StmtId arg1 = std::stoi(clause.getArgs().first.second);
				if (database.parentKB.getChild(arg1) != 0) {
					ClauseResultEntry resultEntry;
					resultEntry["_RESULT"] = "TRUE";
					return { resultEntry };
				}
				else {
					return {};
				}
			}
			else {
				// Case 2: Wildcard, Integer
				StmtId arg2 = std::stoi(clause.getArgs().second.second);
				if (database.parentKB.getParent(arg2) != 0) {
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
		* Evaluates a single Parent* clause on the given PKB where the input contains a StmtId and a synonym.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateParentStarClauseIntSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
				// TODO: Should return set of all children
				// Case 1: Integer, Synonym
				StmtId arg1 = std::stoi(clause.getArgs().first.second);
				Synonym arg2 = clause.getArgs().second.second;

				std::unordered_set<StmtId> children = database.parentKB.getAllChildren(arg1);
				ClauseResult clauseResult;
				for (StmtId child : children) {
					ClauseResultEntry resultEntry;
					resultEntry[arg2] = std::to_string(child);
					clauseResult.emplace_back(resultEntry);
				}
				
				return clauseResult;
			}
			else {
				// Case 2: Synonym, Integer
				Synonym arg1 = clause.getArgs().first.second;
				StmtId arg2 = std::stoi(clause.getArgs().second.second);

				std::unordered_set<StmtId> parents = database.parentKB.getAllParents(arg2);
				ClauseResult clauseResult;
				for (StmtId parent : parents) {
					ClauseResultEntry resultEntry;
					resultEntry[arg1] = std::to_string(parent);
					clauseResult.emplace_back(resultEntry);
				}

				return clauseResult;
			}
		}

		/**
		* Evaluates a single Parent* clause on the given PKB where the inputs contain a wildcard and a synonym.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateParentStarClauseWildSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
				Synonym arg2 = clause.getArgs().second.second;

				// Case 1: Wildcard, Synonym
				ClauseResult clauseResult = {};
				for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
					if (database.parentKB.getParent(i) != 0) {
						ClauseResultEntry resultEntry;
						resultEntry[arg2] = std::to_string(i);
						clauseResult.emplace_back(resultEntry);
					}
				}
				return clauseResult;
			}
			else {
				Synonym arg1 = clause.getArgs().first.second;
				// Case 2: Synonym, Wildcard
				// TODO: Check for set of children
				ClauseResult clauseResult = {};
				for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
					if (database.parentKB.getChild(i) != 0) {
						ClauseResultEntry resultEntry;
						resultEntry[arg1] = std::to_string(i);
						clauseResult.emplace_back(resultEntry);
					}
				}
				return clauseResult;
			}
		}

		/**
		* Evaluates a single Parent* clause on the given PKB where the input contains two synonyms.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateParentStarClauseSynSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {
			Synonym arg1 = clause.getArgs().first.second;
			Synonym arg2 = clause.getArgs().second.second;

			ClauseResult clauseResult = {};
			for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
				std::unordered_set<StmtId> parents = database.parentKB.getAllParents(i);
				for (StmtId parent : parents) {
					ClauseResultEntry resultEntry;
					resultEntry[arg1] = std::to_string(parent);
					resultEntry[arg2] = std::to_string(i);
					clauseResult.emplace_back(resultEntry);
				}
			}
			return clauseResult;
		}

		ClauseResult evaluateParentStarClause(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {

			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
				// Two statement numbers supplied
				return evaluateParentStarClauseIntInt(database, clause);
			}
			else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
				// Two wildcards supplied
				return evaluateParentStarClauseWildWild(database);
			}
			else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
				argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
				// One statement number, one wildcard supplied
				return evaluateParentStarClauseIntWild(database, clause);
			}
			else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
				argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
				// One statement number, one synonym
				return evaluateParentStarClauseIntSyn(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
				argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
				// One synonym, one wildcard
				return evaluateParentStarClauseWildSyn(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
				// Two synonyms
				return evaluateParentStarClauseSynSyn(database, clause, synonymTable);
			}
			else {
				SPA::LoggingUtils::LogErrorMessage("ParentEvaluator::evaluateParentStarClause: Invalid ArgTypes for Parent* clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
				return {};
			}
		}

	}
}
