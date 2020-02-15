#pragma once

#include "Types.h"
#include "PQL.h"

namespace PQL {
	namespace QueryEvaluator {
		/**
		* Evaluates a single PQL query.
		*
		* @param	query	The query to be evaluated.
		* @return	The results of the evaluation
		*/
		QueryResult evaluateQuery(Query query);
	}
};