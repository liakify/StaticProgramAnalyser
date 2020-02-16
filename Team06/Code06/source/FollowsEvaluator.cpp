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
				// check this
			}
			else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD || 
				argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
				// One statement number, one wildcard supplied
				return evaluateFollowsClauseIntWild(database, clause);
			}
		}

		// e.g. Follows(2, 3)
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
				if (database.followsKB.getAllFollowers(arg1).size() != 0) {
					return { {"_RESULT", "TRUE"} };
				}
				else {
					return {};
				}
			}
			else {
				// Case 2: Wildcard, Integer
				StmtId arg2 = std::stoi(clause.getArgs().second.second);
				if (database.followsKB.getAllFollowing(arg2).size() > 0) {
					return { {"_RESULT", "TRUE"} };
				}
				else {
					return {};
				}
			}
		}

	}
}