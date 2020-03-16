#include "stdafx.h"
#include "CppUnitTest.h"
#include "ModifiesKB.h"
#include "Constants.h"
#include "ModifiesEvaluator.h"
#include "PKB.h"
#include "PQLTypes.h"
#include "PKBBuilder.h"
#include "RelationClauseBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestModifiesEvaluator)
	{
	public:
		std::unordered_map<std::string, DesignEntity> EMPTY_SYNONYM_TABLE = {};
		ClauseResult TRUE_RESULT = ClauseResult({ ClauseResultEntry({{"_RESULT", "TRUE"}}) });
		ClauseResult EMPTY_RESULT = {};
		
		TEST_METHOD(evaluateModifiesStmtClauseIntId_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addVar("a")
				.addStmtModifiesRS(5, "a")
				.getPKB();
			
			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::MODIFIESS)
				.setFirstStmt(ArgType::INTEGER, "5")
				.setSecondEnt(ArgType::IDENTIFIER, "a")
				.getClause();

			ClauseResult result = PQL::ModifiesEvaluator::evaluateModifiesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}
		
		TEST_METHOD(evaluateModifiesStmtClauseIntId_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addVar("a")
				.addStmtModifiesRS(5, "a")
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::MODIFIESS)
				.setFirstStmt(ArgType::INTEGER, "5")
				.setSecondEnt(ArgType::IDENTIFIER, "b")
				.getClause();

			ClauseResult result = PQL::ModifiesEvaluator::evaluateModifiesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateModifiesProcClauseIdId_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addProc("proc1", 1)
				.addVar("a")
				.addProcModifiesRS("proc1", "a")
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::MODIFIESP)
				.setFirstEnt(ArgType::IDENTIFIER, "proc1")
				.setSecondEnt(ArgType::IDENTIFIER, "a")
				.getClause();

			ClauseResult result = PQL::ModifiesEvaluator::evaluateModifiesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateModifiesProcClauseIdId_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addProc("proc1", 1)
				.addVar("a")
				.addProcModifiesRS("proc1", "a")
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::MODIFIESP)
				.setFirstStmt(ArgType::IDENTIFIER, "proc1")
				.setSecondEnt(ArgType::IDENTIFIER, "b")
				.getClause();

			ClauseResult result = PQL::ModifiesEvaluator::evaluateModifiesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateModifiesStmtClauseIntWild_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addVar("c")
				.addStmtModifiesRS(10, "c")
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::MODIFIESS)
				.setFirstStmt(ArgType::INTEGER, "10")
				.setSecondEnt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::ModifiesEvaluator::evaluateModifiesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateModifiesStmtClauseIntWild_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::MODIFIESS)
				.setFirstStmt(ArgType::INTEGER, "10")
				.setSecondEnt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::ModifiesEvaluator::evaluateModifiesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

		TEST_METHOD(evaluateModifiesProcClauseIdWild_True)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.addProc("proc2", 5)
				.addVar("c")
				.addProcModifiesRS("proc2", "c")
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::MODIFIESP)
				.setFirstEnt(ArgType::IDENTIFIER, "proc2")
				.setSecondEnt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::ModifiesEvaluator::evaluateModifiesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == TRUE_RESULT);
		}

		TEST_METHOD(evaluateModifiesProcClauseIdWild_Empty)
		{
			PKBBuilder pkbBuilder;
			PKB::PKB pkb = pkbBuilder
				.getPKB();

			RelationClauseBuilder clauseBuilder;
			PQL::RelationClause clause = clauseBuilder
				.setRelationType(RelationType::MODIFIESP)
				.setFirstStmt(ArgType::INTEGER, "proc2")
				.setSecondEnt(ArgType::WILDCARD, "_")
				.getClause();

			ClauseResult result = PQL::ModifiesEvaluator::evaluateModifiesClause(pkb, clause, EMPTY_SYNONYM_TABLE);
			Assert::IsTrue(result == EMPTY_RESULT);
		}

	};
		
}