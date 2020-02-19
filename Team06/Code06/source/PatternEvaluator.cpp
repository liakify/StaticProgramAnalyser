#include "AssignPatternEvaluator.h"
#include "PatternEvaluator.h"

namespace PQL {
	namespace PatternEvaluator {

		ClauseResult evaluatePatternClause(PKB::PKB& database, PatternClause& clause,
			unordered_map<string, DesignEntity>& synonymTable) {

			switch (clause.type) {
			case PatternType::ASSIGN_PATTERN:
				return AssignPatternEvaluator::evaluateAssignPatternClause(database, clause, synonymTable);
				break;
			default:
				SPA::LoggingUtils::LogErrorMessage("PQL::PatternEvaluator::evaluatePatternClause: Pattern type %d not implemented!", clause.type);
			}

		}
	}
}