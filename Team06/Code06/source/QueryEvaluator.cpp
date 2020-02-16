#include "QueryEvaluator.h"
#include "FollowsEvaluator.h"

namespace PQL {
	
	QueryEvaluator::QueryEvaluator(PKB::PKB &database) {
		this->database = database;
	}

	QueryResult QueryEvaluator::evaluateQuery(Query &query) {
		// Results of Clauses
		std::vector<ClauseResult> clauseResults;

		// Evaluate Relation Clauses
		for (RelationClause relation : query.relations) {
			clauseResults.emplace_back(evaluateRelationClause(relation, query.synonymTable));
		}

		// Evaluate Pattern Clauses
		for (PatternClause pattern : query.patterns) {
			clauseResults.emplace_back(evaluatePatternClause(pattern, query.synonymTable));
		}
		return QueryResult();
	}

	ClauseResult QueryEvaluator::evaluateRelationClause(RelationClause &relationClause, 
		std::unordered_map<std::string, DesignEntity> &synonymTable) {
		switch (relationClause.type) {
		case RelationType::FOLLOWS:
			return FollowsEvaluator::evaluateFollowsClause(this->database, relationClause, synonymTable);
			break;
		case RelationType::FOLLOWST:

			break;
		}
		return ClauseResult();
	}

	ClauseResult QueryEvaluator::evaluatePatternClause(PatternClause &patternClause, 
		std::unordered_map<std::string, DesignEntity> &synonymTable) {
		return ClauseResult();
	}

}