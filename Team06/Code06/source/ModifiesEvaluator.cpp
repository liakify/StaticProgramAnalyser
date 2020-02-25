#include "ModifiesEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
	namespace ModifiesEvaluator {

		/**
		* Evaluates a single Modifies clause on the given PKB where the inputs one StmtId and one identifier.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateModifiesClauseIntId(PKB::PKB& database, RelationClause clause) {
			StmtId arg1 = std::stoi(clause.getArgs().first.second);
			VarId arg2 = database.varTable.getVarId(clause.getArgs().second.second);

			if (database.modifiesKB.stmtModifies(arg1, arg2)) {
				ClauseResultEntry resultEntry;
				resultEntry["_RESULT"] = "TRUE";
				return { resultEntry };
			}
			else {
				return {};
			}
		}

		/**
		* Evaluates a single Modifies clause on the given PKB where the input contains a StmtId and a wildcard.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateModifiesClauseIntWild(PKB::PKB& database, RelationClause clause) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			StmtId arg1 = std::stoi(clause.getArgs().first.second);
			if (database.modifiesKB.getAllVarsModifiedByStmt(arg1).size() > 0) {
				ClauseResultEntry resultEntry;
				resultEntry["_RESULT"] = "TRUE";
				return { resultEntry };
			}
			else {
				return {};
			}

		}

		/**
		* Evaluates a single Modifies clause on the given PKB where the input contains a StmtId and a synonym.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateModifiesClauseIntSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<std::string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			StmtId arg1 = std::stoi(clause.getArgs().first.second);
			Synonym arg2 = clause.getArgs().second.second;

			std::unordered_set<VarId> modifiedVars = database.modifiesKB.getAllVarsModifiedByStmt(arg1);
			ClauseResult clauseResult;
			for (VarId var: modifiedVars) {
				ClauseResultEntry resultEntry;
				resultEntry[arg2] = database.varTable.get(var);
				clauseResult.emplace_back(resultEntry);
			}

			return clauseResult;
		}

		/**
		* Evaluates a single Modifies clause on the given PKB where the input contains one synonym and one identifier.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateModifiesClauseSynId(PKB::PKB& database, RelationClause clause,
			unordered_map<std::string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			Synonym arg1 = clause.getArgs().first.second;
			VarId arg2 = database.varTable.getVarId(clause.getArgs().second.second);

			std::unordered_set<StmtId> modifyingStmts = database.modifiesKB.getAllStmtsModifyVar(arg2);
			ClauseResult clauseResult;
			for (StmtId stmt : modifyingStmts) {
				if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt).getType(), synonymTable[arg1])) {
					ClauseResultEntry resultEntry;
					resultEntry[arg1] = std::to_string(stmt);
					clauseResult.emplace_back(resultEntry);
				}
			}

			return clauseResult;
		}

		/**
		* Evaluates a single Modifies clause on the given PKB where the inputs contain a synonym and a wildcard.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateModifiesClauseSynWild(PKB::PKB& database, RelationClause clause,
			unordered_map<std::string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			Synonym arg1 = clause.getArgs().first.second;

			ClauseResult clauseResult = {};
			for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
				if (database.modifiesKB.getAllVarsModifiedByStmt(i).size() > 0) {
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
		* Evaluates a single Modifies clause on the given PKB where the input contains two synonyms.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateModifiesClauseSynSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<std::string, DesignEntity>& synonymTable) {
			Synonym arg1 = clause.getArgs().first.second;
			Synonym arg2 = clause.getArgs().second.second;

			ClauseResult clauseResult = {};
			for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
				if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i).getType(), synonymTable[arg1])) {
					std::unordered_set<VarId> modifiedVars = database.modifiesKB.getAllVarsModifiedByStmt(i);
					for (VarId var : modifiedVars) {
						ClauseResultEntry resultEntry;
						resultEntry[arg1] = std::to_string(i);
						resultEntry[arg2] = database.varTable.get(var);
						clauseResult.emplace_back(resultEntry);
					}
				}
			}
			return clauseResult;
		}

		ClauseResult evaluateModifiesClause(PKB::PKB& database, RelationClause clause,
			unordered_map<std::string, DesignEntity>& synonymTable) {

			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::INTEGER && argType2 == ArgType::IDENTIFIER) {
				// One statement number, one identifier supplied
				return evaluateModifiesClauseIntId(database, clause);
			}
			else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
				// One statement number, one wildcard supplied
				return evaluateModifiesClauseIntWild(database, clause);
			}
			else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
				// One statement number, one synonym
				return evaluateModifiesClauseIntSyn(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::IDENTIFIER) {
				// One synonym, one identifier
				return evaluateModifiesClauseSynId(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
				// One synonym, one wildcard
				return evaluateModifiesClauseSynWild(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
				// Two synonyms
				return evaluateModifiesClauseSynSyn(database, clause, synonymTable);
			}
			else {
				SPA::LoggingUtils::LogErrorMessage("ModifiesEvaluator::evaluateModifiesClause: Invalid ArgTypes for Modifies clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
				return {};
			}
		}

	}
}
