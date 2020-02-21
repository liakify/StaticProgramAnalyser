#include "UsesEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
	namespace UsesEvaluator {

		/**
		* Evaluates a single Uses clause on the given PKB where the inputs one StmtId and one identifier.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateUsesClauseIntId(PKB::PKB& database, RelationClause clause) {
			StmtId arg1 = std::stoi(clause.getArgs().first.second);
			VarId arg2 = database.varTable.getVarId(clause.getArgs().second.second);

			if (database.usesKB.stmtUses(arg1, arg2)) {
				ClauseResultEntry resultEntry;
				resultEntry["_RESULT"] = "TRUE";
				return { resultEntry };
			}
			else {
				return {};
			}
		}

		/**
		* Evaluates a single Uses clause on the given PKB where the input contains a StmtId and a wildcard.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateUsesClauseIntWild(PKB::PKB& database, RelationClause clause) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			StmtId arg1 = std::stoi(clause.getArgs().first.second);
			if (database.usesKB.getAllVarsUsedByStmt(arg1).size() > 0) {
				ClauseResultEntry resultEntry;
				resultEntry["_RESULT"] = "TRUE";
				return { resultEntry };
			}
			else {
				return {};
			}

		}

		/**
		* Evaluates a single Uses clause on the given PKB where the input contains a StmtId and a synonym.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateUsesClauseIntSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			StmtId arg1 = std::stoi(clause.getArgs().first.second);
			Synonym arg2 = clause.getArgs().second.second;

			std::unordered_set<VarId> usedVars = database.usesKB.getAllVarsUsedByStmt(arg1);
			ClauseResult clauseResult;
			for (VarId var : usedVars) {
				ClauseResultEntry resultEntry;
				resultEntry[arg2] = database.varTable.get(var);
				clauseResult.emplace_back(resultEntry);
			}

			return clauseResult;
		}

		/**
		* Evaluates a single Uses clause on the given PKB where the input contains one synonym and one identifier.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateUsesClauseSynId(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			Synonym arg1 = clause.getArgs().first.second;
			VarId arg2 = database.varTable.getVarId(clause.getArgs().second.second);

			std::unordered_set<StmtId> usingStmts = database.usesKB.getAllStmtsUsingVar(arg2);
			ClauseResult clauseResult;
			for (StmtId stmt : usingStmts) {
				if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt).getType(), synonymTable[arg1])) {
					ClauseResultEntry resultEntry;
					resultEntry[arg1] = std::to_string(stmt);
					clauseResult.emplace_back(resultEntry);
				}
			}

			return clauseResult;
		}

		/**
		* Evaluates a single Uses clause on the given PKB where the inputs contain a synonym and a wildcard.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateUsesClauseSynWild(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			Synonym arg1 = clause.getArgs().first.second;

			ClauseResult clauseResult = {};
			for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
				if (database.usesKB.getAllVarsUsedByStmt(i).size() > 0) {
					if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i).getType(), synonymTable[arg1])) {
						ClauseResultEntry resultEntry;
						resultEntry[arg1] = std::to_string(i);
						clauseResult.emplace_back(resultEntry);
					}
				}
			}
			return clauseResult;
		}

		/**
		* Evaluates a single Uses clause on the given PKB where the input contains two synonyms.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateUsesClauseSynSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {
			Synonym arg1 = clause.getArgs().first.second;
			Synonym arg2 = clause.getArgs().second.second;

			ClauseResult clauseResult = {};
			for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
				if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i).getType(), synonymTable[arg1])) {
					std::unordered_set<VarId> usedVars = database.usesKB.getAllVarsUsedByStmt(i);
					for (VarId var : usedVars) {
						ClauseResultEntry resultEntry;
						resultEntry[arg1] = std::to_string(i);
						resultEntry[arg2] = database.varTable.get(var);
						clauseResult.emplace_back(resultEntry);
					}
				}
			}
			return clauseResult;
		}

		ClauseResult evaluateUsesClause(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {

			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::INTEGER && argType2 == ArgType::IDENTIFIER) {
				// One statement number, one identifier supplied
				return evaluateUsesClauseIntId(database, clause);
			}
			else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
				// One statement number, one wildcard supplied
				return evaluateUsesClauseIntWild(database, clause);
			}
			else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
				// One statement number, one synonym
				return evaluateUsesClauseIntSyn(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::IDENTIFIER) {
				// One synonym, one identifier
				return evaluateUsesClauseSynId(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
				// One synonym, one wildcard
				return evaluateUsesClauseSynWild(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
				// Two synonyms
				return evaluateUsesClauseSynSyn(database, clause, synonymTable);
			}
			else {
				SPA::LoggingUtils::LogErrorMessage("UsesEvaluator::evaluateUsesClause: Invalid ArgTypes for Uses clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
				return {};
			}
		}

	}
}
