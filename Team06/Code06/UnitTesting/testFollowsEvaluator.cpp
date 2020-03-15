#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowsKB.h"
#include "Constants.h"
#include "FollowsEvaluator.h"
#include "PKB.h"
#include "PQLTypes.h"
#include "PKBBuilder.h"
#include "RelationClauseBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestFollowsEvaluator)
	{
	public:
		std::unordered_map<std::string, DesignEntity> EMPTY_SYNONYM_TABLE = {};
		ClauseResult TRUE_RESULT = ClauseResult({ ClauseResultEntry({{"_RESULT", "TRUE"}}) });
		ClauseResult EMPTY_RESULT = {};

		TEST_CLASS_INITIALIZE(setup) {
		}
		TEST_METHOD(evaluateFollowsClauseIntInt_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addFollowsRS(1, 2)
				.getPKB();
			
			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::FOLLOWS)
				.setFirstStmt(ArgType::INTEGER, "1")
				.setSecondStmt(ArgType::INTEGER, "2")
				.getClause();

			ClauseResult result = PQL::FollowsEvaluator::evaluateFollowsClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateFollowsClauseIntInt_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addFollowsRS(1, 2)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::FOLLOWS)
				.setFirstStmt(ArgType::INTEGER, "2")
				.setSecondStmt(ArgType::INTEGER, "1")
				.getClause();

			ClauseResult result = PQL::FollowsEvaluator::evaluateFollowsClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateFollowsClauseWildWild_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addFollowsRS(10, 15)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::FOLLOWS)
				.setFirstStmt(ArgType::WILDCARD, "_")
				.setSecondStmt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::FollowsEvaluator::evaluateFollowsClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateFollowsClauseWildWild_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::FOLLOWS)
				.setFirstStmt(ArgType::WILDCARD, "_")
				.setSecondStmt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::FollowsEvaluator::evaluateFollowsClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateFollowsClauseIntWild_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addFollowsRS(3, 5)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::FOLLOWS)
				.setFirstStmt(ArgType::INTEGER, "3")
				.setSecondStmt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::FollowsEvaluator::evaluateFollowsClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateFollowsClauseIntWild_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addFollowsRS(3, 5)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::FOLLOWS)
				.setFirstStmt(ArgType::INTEGER, "5")
				.setSecondStmt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::FollowsEvaluator::evaluateFollowsClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateFollowsClauseWildInt_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addFollowsRS(3, 5)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::FOLLOWS)
				.setFirstStmt(ArgType::WILDCARD, "_")
				.setSecondStmt(ArgType::INTEGER, "5")
				.getClause();

			ClauseResult result = PQL::FollowsEvaluator::evaluateFollowsClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateFollowsClauseWildInt_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addFollowsRS(3, 5)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::FOLLOWS)
				.setFirstStmt(ArgType::WILDCARD, "_")
				.setSecondStmt(ArgType::INTEGER, "3")
				.getClause();

			ClauseResult result = PQL::FollowsEvaluator::evaluateFollowsClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}
	};
		
}