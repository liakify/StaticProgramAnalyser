#pragma once

#include "Types.h"
#include "PKB.h"
#include "PQL.h"

namespace PQL {
	namespace FollowsEvaluator {
		
		/**
		* Evaluates a single Follows clause on the given PKB.
		*
		* @param	database	The PKB to evaluate the clause on.
		* @param	clause		The clause to evaluate.
		* @synonymTable	The synonym table associated with the query containing the clause.
		* @return	The result of the evaluation.
		*/
		ClauseResult evaluateFollowsClause(PKB::PKB& database, RelationClause clause, 
			unordered_map<string, DesignEntity>& synonymTable);

	}
}