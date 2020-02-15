#include "QueryEvaluator.h"

namespace PQL {
	
	QueryEvaluator::QueryEvaluator(PKB::PKB &database) {
		this->database = database;
	}

	QueryResult QueryEvaluator::evaluateQuery(const Query &query) {
		// Results of Clauses
		std::vector<ClauseResult> clauseResults;

		// Evaluate Relation Clauses
		for (RelationClause relation : query.relations) {
			clauseResults.push_back(evaluateRelationClause(relation));
		}

		// Evaluate Pattern Clauses
		for (PatternClause pattern : query.patterns) {
			clauseResults.push_back(evaluatePatternClause(pattern));
		}
	}

	ClauseResult QueryEvaluator::evaluateRelationClause(const RelationClause &relationClause) {
		switch (relationClause.type) {
		case RelationType::FOLLOWS:

			break;
		case RelationType::FOLLOWST:

			break;
		}
	}

	ClauseResult QueryEvaluator::evaluatePatternClause(const PatternClause &patternClause) {

	}

}