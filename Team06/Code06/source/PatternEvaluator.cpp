#include "PatternEvaluator.h"

namespace PQL {
	namespace PatternEvaluator {

		/**
		* Evaluates a single assign pattern clause.
		*
		* @param	database		The PKB to evaluate the assign pattern clause on.
		* @param	clause			The assign pattern clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The results of the evaluation.
		*/
		ClauseResult evaluateAssignPatternClause(const PKB::PKB& database, const PatternClause& clause,
			const unordered_map<string, DesignEntity>& synonymTable) {
			return vector<ClauseResultEntry>();
		}

		ClauseResult evaluatePatternClause(const PKB::PKB& database, const PatternClause& clause,
			const unordered_map<string, DesignEntity>& synonymTable) {

			switch (clause.type) {
			case PatternType::ASSIGN_PATTERN:
				return evaluateAssignPatternClause(database, clause, synonymTable);
				break;
			default:
				SPA::LoggingUtils::LogErrorMessage("PQL::PatternEvaluator::evaluatePatternClause: Pattern type %d not implemented!", clause.type);
			}

		}

		ClauseResult evaluateAssignPatternClause(const PKB::PKB& database, const PatternClause& clause, 
			const unordered_map<string, DesignEntity>& synonymTable) {
			
		}
	}
}