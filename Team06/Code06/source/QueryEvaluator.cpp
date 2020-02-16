#include "QueryEvaluator.h"

namespace PQL {
	
	QueryEvaluator::QueryEvaluator(PKB::PKB &database) {
		this->database = database;
	}

	QueryResult QueryEvaluator::evaluateQuery(Query &query) {
		// Results of Clauses
		std::vector<ClauseResult> clauseResults;

		// Evaluate Relation Clauses
		for (RelationClause relation : query.relations) {
			clauseResults.emplace_back(evaluateRelationClause(relation));
		}

		// Evaluate Pattern Clauses
		for (PatternClause pattern : query.patterns) {
			clauseResults.emplace_back(evaluatePatternClause(pattern));
		}
		return QueryResult();
	}

	ClauseResult QueryEvaluator::evaluateRelationClause(RelationClause &relationClause) {
		switch (relationClause.type) {
		case RelationType::FOLLOWS:
			
			break;
		case RelationType::FOLLOWST:

			break;
		}
		return ClauseResult();
	}

	ClauseResult QueryEvaluator::evaluatePatternClause(PatternClause &patternClause) {
		return ClauseResult();
	}

}