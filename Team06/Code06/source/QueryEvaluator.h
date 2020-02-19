#pragma once

#include "Types.h"
#include "PKB.h"
#include "PQL.h"

namespace PQL {

	struct RelationClause;
	struct PatternClause;
	struct Query;

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
		QueryResult evaluateQuery(Query &query);

	private:
		PKB::PKB database;
		
		/**
		* Evaluates a single relation clause.
		*
		* @param	relationClause	The relation clause to evaluate
		* @param	synonymTable	The synonym table associated with the query.
		* @return	The results of the evaluation
		*/
		ClauseResult evaluateRelationClause(RelationClause &relationClause, 
			std::unordered_map<std::string, DesignEntity> &synonymTable);

		/**
		* Evaluates a single pattern clause
		*
		* @param	patternClause	The pattern clause to evaluate
		* @param	synonymTable	The synonym table associated with the query.
		* @return	The results of the evaluation
		*/
		ClauseResult evaluatePatternClause(PatternClause &patternClause, 
			std::unordered_map<std::string, DesignEntity> &synonymTable);
	};
}
