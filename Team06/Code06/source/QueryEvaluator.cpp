#include "AssignPatternEvaluator.h"
#include "FollowsEvaluator.h"
#include "FollowsStarEvaluator.h"
#include "LoggingUtils.h"
#include "ModifiesEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentStarEvaluator.h"
#include "QueryEvaluator.h"
#include "UsesEvaluator.h"
#include "TypeUtils.h"

namespace PQL {
	
	QueryEvaluator::QueryEvaluator(PKB::PKB &database) {
		this->database = database;
	}

	ClauseResult QueryEvaluator::evaluateQuery(Query &query) {
		// Results of Clauses
		std::vector<ClauseResult> clauseResults;

		// Add table containing "TRUE" to handle empty queries
		ClauseResult trueClauseResult;
		ClauseResultEntry trueClauseResultEntry;
		trueClauseResultEntry["_RESULT"] = "TRUE";
		trueClauseResult.emplace_back(trueClauseResultEntry);
		clauseResults.emplace_back(trueClauseResult);

		// Evaluate Relation Clauses
		for (RelationClause relation : query.relations) {
			clauseResults.emplace_back(evaluateRelationClause(relation, query.synonymTable));
		}

		// Evaluate Pattern Clauses
		for (PatternClause pattern : query.patterns) {
			clauseResults.emplace_back(evaluatePatternClause(pattern, query.synonymTable));
		}

		// Combine Results
		ClauseResult combinedResult = combineClauseResults(clauseResults);
		
		// Extract necessary results to answer query
		ClauseResult result = extractQueryResults(query, combinedResult);

		return result;
	}

	ClauseResult QueryEvaluator::extractQueryResults(Query &query, ClauseResult& combinedResult) {
		// Iteration 1: Only one target entity
		if (combinedResult.empty()) {
			SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::extractQueryResults: Empty Result!\n");
			return {};
		}

		Synonym target = query.targetEntities[0];

		if (combinedResult[0].find(target) != combinedResult[0].end()) {
			// Target does not exist in table: treat results from table as true if table is not empty
			ClauseResult result;
			for (ClauseResultEntry entry : combinedResult) {
				ClauseResultEntry resultEntry;
				resultEntry[target] = entry[target];
				result.emplace_back(resultEntry);
			}
			return result;
		}
		else {
			// Case bash by target entity type
			if (query.synonymTable[target] == DesignEntity::PROCEDURE) {
				// Iteration 1: Only one procedure
				ClauseResult result;
				ClauseResultEntry resultEntry;
				resultEntry[target] = database.procTable.get(1).getName();
				result.emplace_back(resultEntry);
				return result;
			}
			else if (query.synonymTable[target] == DesignEntity::CONSTANT) {
				ClauseResult result;
				for (ConstId i = 1; i <= database.constTable.size(); i++) {
					ClauseResultEntry resultEntry;
					resultEntry[target] = database.constTable.get(i);
					result.emplace_back(resultEntry);
				}
				return result;
			}
			else if (query.synonymTable[target] == DesignEntity::VARIABLE) {
				ClauseResult result;
				std::unordered_set<VarName> vars = database.varTable.getAllVars();
				for (VarName var : vars) {
					ClauseResultEntry resultEntry;
					resultEntry[target] = var;
					result.emplace_back(resultEntry);
				}
				return result;
			}
			else {
				// Statement
				ClauseResult result;
				for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
					if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i).getType(), query.synonymTable[target])) {
						ClauseResultEntry resultEntry;
						resultEntry[target] = std::to_string(i);
						result.emplace_back(resultEntry);
					}
				}
				return result;
			}
		}
	}

	ClauseResultEntry QueryEvaluator::combineTwoClauseResultEntries(ClauseResultEntry &entry1, ClauseResultEntry &entry2, 
		std::unordered_set<Synonym> &commonSynonyms) {

		ClauseResultEntry combinedEntry;
		for (std::pair<std::string, std::string> field : entry1) {
			combinedEntry.insert(field);
		}

		for (std::pair<std::string, std::string> field : entry2) {
			if (commonSynonyms.find(field.first) == commonSynonyms.end()) {
				combinedEntry.insert(field);
			}
		}

		return combinedEntry;
	}

	bool QueryEvaluator::checkCommonSynonyms(ClauseResultEntry &entry1, ClauseResultEntry &entry2, 
		std::unordered_set<Synonym> &commonSynonyms) {
		for (Synonym synonym : commonSynonyms) {
			if (entry1[synonym] != entry2[synonym]) {
				return false;
			}
		}
		return true;
	}

	ClauseResult QueryEvaluator::combineTwoClauseResults(ClauseResult clauseResults1, ClauseResult clauseResults2) {
		if (clauseResults1.empty() || clauseResults2.empty()) {
			return {};
		}

		// Extract common synonyms
		std::unordered_set<Synonym> commonSynonyms;
		for (std::pair<std::string, std::string> entry : clauseResults1[0]) {
			if (clauseResults2[0].find(entry.first) != clauseResults2[0].end()) {
				commonSynonyms.insert(entry.first);
			}
		}

		ClauseResult combinedResult;
		// Perform a Cartesian Product
		for (ClauseResultEntry entry1 : clauseResults1) {
			for (ClauseResultEntry entry2 : clauseResults2) {
				if (checkCommonSynonyms(entry1, entry2, commonSynonyms)) {
					combinedResult.emplace_back(combineTwoClauseResultEntries(entry1, entry2, commonSynonyms));
				}
			}
		}

		return combinedResult;
	}

	ClauseResult QueryEvaluator::combineClauseResults(std::vector<ClauseResult> clauseResults) {
		if (clauseResults.size() == 1) {
			return clauseResults[0];
		}
		if (clauseResults.size() < 1) {
			SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::combineClauseResults: clauseResults empty!");
			return {};
		}
		std::vector<ClauseResult> left;
		std::vector<ClauseResult> right;
		for (int i = 0; i < (int)clauseResults.size() / 2; i++) {
			left.emplace_back(clauseResults[i]);
		}
		for (int i = clauseResults.size() / 2; i < (int)clauseResults.size(); i++) {
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
