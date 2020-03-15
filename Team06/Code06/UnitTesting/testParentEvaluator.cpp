#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentKB.h"
#include "Constants.h"
#include "ParentEvaluator.h"
#include "PKB.h"
#include "PQLTypes.h"
#include "PKBBuilder.h"
#include "RelationClauseBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestParentEvaluator)
	{
	public:
		std::unordered_map<std::string, DesignEntity> EMPTY_SYNONYM_TABLE = {};
		ClauseResult TRUE_RESULT = ClauseResult({ ClauseResultEntry({{"_RESULT", "TRUE"}}) });
		ClauseResult EMPTY_RESULT = {};
		TEST_METHOD(evaluateParentClauseIntInt_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addParentRS(1, 2)
				.getPKB();
			
			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::PARENT)
				.setFirstStmt(ArgType::INTEGER, "1")
				.setSecondStmt(ArgType::INTEGER, "2")
				.getClause();

			ClauseResult result = PQL::ParentEvaluator::evaluateParentClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateParentClauseIntInt_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addParentRS(1, 2)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::PARENT)
				.setFirstStmt(ArgType::INTEGER, "2")
				.setSecondStmt(ArgType::INTEGER, "1")
				.getClause();

			ClauseResult result = PQL::ParentEvaluator::evaluateParentClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateParentClauseWildWild_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addParentRS(10, 15)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::PARENT)
				.setFirstStmt(ArgType::WILDCARD, "_")
				.setSecondStmt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::ParentEvaluator::evaluateParentClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateParentClauseWildWild_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::PARENT)
				.setFirstStmt(ArgType::WILDCARD, "_")
				.setSecondStmt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::ParentEvaluator::evaluateParentClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateParentClauseIntWild_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addParentRS(3, 5)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::PARENT)
				.setFirstStmt(ArgType::INTEGER, "3")
				.setSecondStmt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::ParentEvaluator::evaluateParentClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateParentClauseIntWild_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addParentRS(3, 5)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::PARENT)
				.setFirstStmt(ArgType::INTEGER, "5")
				.setSecondStmt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::ParentEvaluator::evaluateParentClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateParentClauseWildInt_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addParentRS(3, 5)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::PARENT)
				.setFirstStmt(ArgType::WILDCARD, "_")
				.setSecondStmt(ArgType::INTEGER, "5")
				.getClause();

			ClauseResult result = PQL::ParentEvaluator::evaluateParentClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateParentClauseWildInt_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addParentRS(3, 5)
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::PARENT)
				.setFirstStmt(ArgType::WILDCARD, "_")
				.setSecondStmt(ArgType::INTEGER, "3")
				.getClause();

			ClauseResult result = PQL::ParentEvaluator::evaluateParentClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}
	};
		
}