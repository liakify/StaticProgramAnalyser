#include "QueryEvaluator.h"
#include "FollowsEvaluator.h"
#include "FollowsStarEvaluator.h"
#include "LoggingUtils.h"
#include "ParentEvaluator.h"
#include "ParentStarEvaluator.h"

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
			return FollowsStarEvaluator::evaluateFollowsStarClause(this->database, relationClause, synonymTable);
			break;
		case RelationType::PARENT:
			return ParentEvaluator::evaluateParentClause(this->database, relationClause, synonymTable);
			break;
		case RelationType::PARENTT:
			return ParentStarEvaluator::evaluateParentStarClause(this->database, relationClause, synonymTable);
			break;
		default:
			SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::evaluateRelationClause: Unknown relation type %d\n", relationClause.type);
			return {};
		}
	}

	ClauseResult QueryEvaluator::evaluatePatternClause(PatternClause &patternClause, 
		std::unordered_map<std::string, DesignEntity> &synonymTable) {
		return ClauseResult();
	}

}