#include "FollowsEvaluator.h"

namespace PQL {
	namespace FollowsEvaluator {

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
		}

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

				std::unordered_set<StmtId> followers = database.followsKB.getAllFollowers(arg1);
				ClauseResult clauseResult;
				for (StmtId stmt : followers) {
					ClauseResultEntry result = { {arg2, std::to_string(stmt)} };
					clauseResult.emplace_back(result);
				}
				return clauseResult;
			}
			else {
				// Case 2: Synonym, Integer
				Synonym arg1 = clause.getArgs().first.second;
				StmtId arg2 = std::stoi(clause.getArgs().second.second);
				
				std::unordered_set<StmtId> following = database.followsKB.getAllFollowing(arg2);
				ClauseResult clauseResult;
				for (StmtId stmt : following) {
					ClauseResultEntry result = { {arg1, std::to_string(stmt)} };
					clauseResult.emplace_back(result);
				}
				return clauseResult;
			}
		}

	}
}