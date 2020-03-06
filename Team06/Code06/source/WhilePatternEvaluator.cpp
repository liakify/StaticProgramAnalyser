#include "WhilePatternEvaluator.h"
#include "LoggingUtils.h"

namespace PQL {
	namespace WhilePatternEvaluator {

		/**
		* Evaluates a single while pattern clause on the given PKB where the inputs is one wildcard.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateWhilePatternClauseWild(PKB::PKB& database) {
			// Here, we just need to check if there is at least one while statement in the program
			if (database.stmtTable.getStmtsByType(StmtType::WHILE).size() > 0) {
				ClauseResultEntry resultEntry;
				resultEntry["_RESULT"] = "TRUE";
				return { resultEntry };
			}
			else {
				return {};
			}
		}

		/**
		* Evaluates a single while pattern clause on the given PKB where the input is one identifier.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateWhilePatternClauseId(PKB::PKB& database, PatternClause &clause,
			std::unordered_map<std::string, DesignEntity>& synonymTable) {

			Synonym arg0 = clause.synonym;
			VarId arg1 = database.varTable.getVarId(clause.getArgs().first.second);
			
			std::unordered_set<StmtId> stmts = database.patternKB.getWhilePatternStmts(arg1);
			
			ClauseResult clauseResult;
			for (StmtId stmt : stmts) {
				ClauseResultEntry resultEntry;
				resultEntry[arg0] = std::to_string(stmt);
				clauseResult.emplace_back(resultEntry);
			}
			return clauseResult;
		}

		/**
		* Evaluates a single while pattern clause on the given PKB where the input is one synonym.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateWhilePatternClauseSyn(PKB::PKB& database, PatternClause& clause,
			std::unordered_map<std::string, DesignEntity>& synonymTable) {

			Synonym arg0 = clause.synonym;
			Synonym arg1 = clause.getArgs().first.second;

			// TODO
			ClauseResult clauseResult;
			
			return clauseResult;
		}

		ClauseResult evaluateWhilePatternClause(PKB::PKB& database, PatternClause clause,
			std::unordered_map<std::string, DesignEntity>& synonymTable) {

			ArgType argType1 = clause.getArgs().first.first;

			if (argType1 == ArgType::WILDCARD) {
				// 1 wildcard
				return evaluateWhilePatternClauseWild(database);
			}
			else if (argType1 == ArgType::IDENTIFIER) {
				// 1 identifier
				return evaluateWhilePatternClauseId(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::SYNONYM) {
				// 1 synonym
				return evaluateWhilePatternClauseSyn(database, clause, synonymTable);
			}
			else {
				SPA::LoggingUtils::LogErrorMessage("WhilePatternEvaluator::evaluateWhilePatternClause: Invalid ArgTypes for While Pattern clause. argType1 = %d\n", argType1);
				return {};
			}
		}

	}
}