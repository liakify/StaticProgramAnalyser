#include "AssignPatternEvaluator.h"
#include "FollowsEvaluator.h"
#include "FollowsStarEvaluator.h"
#include "LoggingUtils.h"
#include "ModifiesEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentStarEvaluator.h"
#include "QueryEvaluator.h"
#include "UsesEvaluator.h"

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

		// Combine Results
		ClauseResult result = combineClauseResults(clauseResults);
		
		return QueryResult();
	}

	std::vector<ClauseResult> combineTwoClauseResults(ClauseResult clauseResults1, ClauseResult clauseResults2) {

	}

	ClauseResult combineClauseResults(std::vector<ClauseResult> clauseResults) {
		if (clauseResults.size() == 1) {
			return clauseResults[0];
		}
		std::vector<ClauseResult> left;
		std::vector<ClauseResult> right;
		for (int i = 0; i < clauseResults.size() / 2; i++) {
			left.emplace_back(clauseResults[i]);
		}
		for (int i = clauseResults.size() / 2; i < clauseResults.size(); i++) {
			right.emplace_back(clauseResults[i]);
		}
		ClauseResult leftResult = combineClauseResults(left);
		ClauseResult rightResult = combineClauseResults(right);
		ClauseResult combinedResults = combineTwoClauseResults(leftResult, rightResult);
		return combinedResults;
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
		case RelationType::MODIFIESS:
			return ModifiesEvaluator::evaluateModifiesClause(this->database, relationClause, synonymTable);
			break;
		case RelationType::MODIFIESP:
			SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::evaluateRelationClause: MODIFIESP not implemented!\n");
			return {};
			break;
		case RelationType::USESS:
			return UsesEvaluator::evaluateUsesClause(this->database, relationClause, synonymTable);
			break;
		case RelationType::USESP:
			SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::evaluateRelationClause: USESP not implemented!\n");
			return {};
			break;
		default:
			SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::evaluateRelationClause: Unknown relation type %d\n", relationClause.type);
			return {};
		}
	}

	ClauseResult QueryEvaluator::evaluatePatternClause(PatternClause &patternClause, 
		std::unordered_map<std::string, DesignEntity> &synonymTable) {
		
		switch (patternClause.type) {
		case PatternType::ASSIGN_PATTERN:
			return AssignPatternEvaluator::evaluateAssignPatternClause(this->database, patternClause, synonymTable);
			break;
		case PatternType::IF_PATTERN:
			SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::evaluatePatternClause: Evaluator for IF_PATTERN pattern type not implemented!");
			return {};
			break;
		case PatternType::WHILE_PATTERN:
			SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::evaluatePatternClause: Evaluator for WHILE_PATTERN pattern type not implemented!");
			return {};
			break;
		default:
			SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::evaluatePatternClause: Unknown pattern type %d\n", patternClause.type);
			return {};
		}

	}

}
