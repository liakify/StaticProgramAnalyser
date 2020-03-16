#include "stdafx.h"
#include "CppUnitTest.h"
#include "UsesKB.h"
#include "Constants.h"
#include "UsesEvaluator.h"
#include "PKB.h"
#include "PQLTypes.h"
#include "PKBBuilder.h"
#include "RelationClauseBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestUsesEvaluator)
	{
	public:
		std::unordered_map<std::string, DesignEntity> EMPTY_SYNONYM_TABLE = {};
		ClauseResult TRUE_RESULT = ClauseResult({ ClauseResultEntry({{"_RESULT", "TRUE"}}) });
		ClauseResult EMPTY_RESULT = {};
		
		TEST_METHOD(evaluateUsesStmtClauseIntId_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addVar("a")
				.addStmtUsesRS(5, "a")
				.getPKB();
			
			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::USESS)
				.setFirstStmt(ArgType::INTEGER, "5")
				.setSecondEnt(ArgType::IDENTIFIER, "a")
				.getClause();

			ClauseResult result = PQL::UsesEvaluator::evaluateUsesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}
		
		TEST_METHOD(evaluateUsesStmtClauseIntId_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addVar("a")
				.addStmtUsesRS(5, "a")
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::USESS)
				.setFirstStmt(ArgType::INTEGER, "5")
				.setSecondEnt(ArgType::IDENTIFIER, "b")
				.getClause();

			ClauseResult result = PQL::UsesEvaluator::evaluateUsesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateUsesProcClauseIdId_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addProc("proc1", 1)
				.addVar("a")
				.addProcUsesRS("proc1", "a")
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::USESP)
				.setFirstEnt(ArgType::IDENTIFIER, "proc1")
				.setSecondEnt(ArgType::IDENTIFIER, "a")
				.getClause();

			ClauseResult result = PQL::UsesEvaluator::evaluateUsesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateUsesProcClauseIdId_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addProc("proc1", 1)
				.addVar("a")
				.addProcUsesRS("proc1", "a")
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::USESP)
				.setFirstStmt(ArgType::IDENTIFIER, "proc1")
				.setSecondEnt(ArgType::IDENTIFIER, "b")
				.getClause();

			ClauseResult result = PQL::UsesEvaluator::evaluateUsesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateUsesStmtClauseIntWild_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addVar("c")
				.addStmtUsesRS(10, "c")
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::USESS)
				.setFirstStmt(ArgType::INTEGER, "10")
				.setSecondEnt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::UsesEvaluator::evaluateUsesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateUsesStmtClauseIntWild_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::USESS)
				.setFirstStmt(ArgType::INTEGER, "10")
				.setSecondEnt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::UsesEvaluator::evaluateUsesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateUsesProcClauseIdWild_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addProc("proc2", 5)
				.addVar("c")
				.addProcUsesRS("proc2", "c")
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::USESP)
				.setFirstEnt(ArgType::IDENTIFIER, "proc2")
				.setSecondEnt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::UsesEvaluator::evaluateUsesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateUsesProcClauseIdWild_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::USESP)
				.setFirstStmt(ArgType::INTEGER, "proc2")
				.setSecondEnt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::UsesEvaluator::evaluateUsesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

	};
		
}