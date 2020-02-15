#pragma once

#include "Types.h"
#include "PQL.h"

namespace PQL {
	class QueryEvaluator {

	public:
		/**
		* Constructor for a QueryEvaluator.
		* The QueryEvaluator will evaluate queries based on the given PKB.
		*
		* @param	database	The PKB to evaluate queries on
		*/
		QueryEvaluator(PKB::PKB &database);

		/**
		* Evaluates a single PQL query.
		*
		* @param	query	The query to be evaluated.
		* @return	The results of the evaluation
		*/
		QueryResult evaluateQuery(const Query &query);

	private:
		PKB::PKB database;
		
		/**
		* Evaluates a single relation clause.
		*
		* @param	relationClause	The relation clause to evaluate
		* @return	The results of the evaluation
		*/
		ClauseResult evaluateRelationClause(const RelationClause &relationClause);

		/**
		* Evaluates a single pattern clause
		*
		* @param	patternClause	The pattern clause to evaluate
		* @return	The results of the evaluation
		*/
		ClauseResult evaluatePatternClause(const PatternClause &patternClause);
	};
}