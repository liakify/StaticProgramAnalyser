#pragma once

#include "LoggingUtils.h"
#include "Types.h"
#include "PKB.h"
#include "PQL.h"

namespace PQL {
	namespace PatternEvaluator {

		/**
		* Evaluates a single pattern clause.
		*
		* @param	database		The PKB to evaluate the pattern clause on.
		* @param	clause			The pattern clause to evaluate.
		* @param	synonymTable	The synonym table associated with the query containing the clause.
		* @return	The results of the evaluation.
		*/
		ClauseResult evaluatePatternClause(PKB::PKB& database, PatternClause& clause,
			unordered_map<string, DesignEntity>& synonymTable);

	}
}