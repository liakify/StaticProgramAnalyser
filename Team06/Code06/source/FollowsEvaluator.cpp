#include "FollowsEvaluator.h"
#include "LoggingUtils.h"

namespace PQL {
	namespace FollowsEvaluator {

		ClauseResult evaluateFollowsClauseIntInt(PKB::PKB& database, RelationClause clause) {
			StmtId arg1 = std::stoi(clause.getArgs().first.second);
			StmtId arg2 = std::stoi(clause.getArgs().second.second);

			if (database.followsKB.follows(arg1, arg2)) {
				return { {"_RESULT", "TRUE"} };
			}
			else {
				return {};
			}
		}

		ClauseResult evaluateFollowsClauseIntWild(PKB::PKB& database, RelationClause clause) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
				// Case 1: Integer, Wildcard
				StmtId arg1 = std::stoi(clause.getArgs().first.second);
				if (database.followsKB.getFollower(arg1) != 0) {
					return { {"_RESULT", "TRUE"} };
				}
				else {
					return {};
				}
			}
			else {
				// Case 2: Wildcard, Integer
				StmtId arg2 = std::stoi(clause.getArgs().second.second);
				if (database.followsKB.getFollowing(arg2) != 0) {
					return { {"_RESULT", "TRUE"} };
				}
				else {
					return {};
				}
			}
		}
		
		ClauseResult evaluateFollowsClauseIntSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM) {
				// Case 1: Integer, Synonym
				StmtId arg1 = std::stoi(clause.getArgs().first.second);
				Synonym arg2 = clause.getArgs().second.second;

				StmtId follower = database.followsKB.getFollower(arg1);
				if (follower == 0) {
					return {};
				}
				else {
					std::unordered_map<std::string, std::string> resultPair;
					resultPair[arg2] = follower;
					ClauseResultEntry resultEntry = { resultPair };
					return { resultEntry };
				}
			}
			else {
				// Case 2: Synonym, Integer
				Synonym arg1 = clause.getArgs().first.second;
				StmtId arg2 = std::stoi(clause.getArgs().second.second);
				
				StmtId following = database.followsKB.getFollowing(arg2);
				if (following == 0) {
					return {};
				}
				else {
					std::unordered_map<std::string, std::string> resultPair;
					resultPair[arg1] = following;
					ClauseResultEntry resultEntry = { resultPair };
					return { resultEntry };
				}
			}
		}
		
		ClauseResult evaluateFollowsClauseWildSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {
			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM) {
				Synonym arg2 = clause.getArgs().second.second;

				// Case 1: Wildcard, Synonym
				ClauseResult clauseResult = {};
				for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
					if (database.followsKB.getFollowing(i) != 0) {
						std::unordered_map<std::string, std::string> resultPair;
						resultPair[arg2] = std::to_string(i);
						ClauseResultEntry resultEntry = { resultPair };
						clauseResult.emplace_back(resultEntry);
					}
				}
				return clauseResult;
			}
			else {
				Synonym arg1 = clause.getArgs().first.second;
				// Case 2: Synonym, Wildcard
				ClauseResult clauseResult = {};
				for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
					if (database.followsKB.getFollower(i) != 0) {
						std::unordered_map<std::string, std::string> resultPair;
						resultPair[arg1] = std::to_string(i);
						ClauseResultEntry resultEntry = { resultPair };
						clauseResult.emplace_back(resultEntry);
					}
				}
				return clauseResult;
			}
		}
		

		ClauseResult evaluateFollowsClauseSynSyn(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {
			Synonym arg1 = clause.getArgs().first.second;
			Synonym arg2 = clause.getArgs().second.second;

			ClauseResult clauseResult = {};
			for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
				StmtId follower = database.followsKB.getFollower(i);
				if (follower != 0) {
					std::unordered_map<std::string, std::string> resultPair;
					resultPair[arg1] = std::to_string(i);
					resultPair[arg2] = std::to_string(follower);
				}
			}
			return clauseResult;
		}

		ClauseResult evaluateFollowsClause(PKB::PKB& database, RelationClause clause,
			unordered_map<string, DesignEntity>& synonymTable) {

			ArgType argType1 = clause.getArgs().first.first;
			ArgType argType2 = clause.getArgs().second.first;

			if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
				// Two statement numbers supplied
				return evaluateFollowsClauseIntInt(database, clause);
			}
			else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
				// Two wildcards supplied
				// TODO: Request PKB to add indicator
			}
			
			else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
				argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
				// One statement number, one wildcard supplied
				return evaluateFollowsClauseIntWild(database, clause);
			}
			else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
				argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
				// One statement number, one synonym
				return evaluateFollowsClauseIntSyn(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
				argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
				// One synonym, one wildcard
				return evaluateFollowsClauseWildSyn(database, clause, synonymTable);
			}
			else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
				// Two synonyms
				return evaluateFollowsClauseSynSyn(database, clause, synonymTable);
			}
			else {
				SPA::LoggingUtils::LogErrorMessage("FollowsEvaluator::evaluateFollowsClause: Invalid ArgTypes for Follows clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
			}
		}

	}
}