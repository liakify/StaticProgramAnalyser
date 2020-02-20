#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentEvaluator.h"
#include "PKB.h"
#include "PQLTypes.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestParentEvaluator)
	{
	public:
		TEST_CLASS_INITIALIZE(setup) {

		}
		TEST_METHOD(evaluateParentClauseIntInt) {
			// Create PKB
			PKB::PKB pkb;
			pkb.parentKB.addParent(3, 4);
			
			// Test a successful instance of Parent(Int, Int)
			PQL::RelationClause relation;
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::INTEGER, "3");
			relation.secondStmt = std::make_pair(ArgType::INTEGER, "4");
			std::unordered_map<std::string, DesignEntity> synonymTable;

			ClauseResult clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb, relation, synonymTable);
			Assert::IsTrue(!clauseResult.empty() && clauseResult[0]["_RESULT"] == "TRUE");

			// Test an unsuccessful instance of Parent(Int, Int)
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::INTEGER, "3");
			relation.secondStmt = std::make_pair(ArgType::INTEGER, "7");

			clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb, relation, synonymTable);
			Assert::IsTrue(clauseResult.empty());
		}

		TEST_METHOD(evaluateParentClauseWildWild) {
			// Test a successful instance of Parent(_, _)
			PKB::PKB pkb;
			for (int i = 1; i <= 5; i++) {
				pkb.stmtTable.insertStmt(SIMPLE::AssignStmt(i, SIMPLE::Expression("a")));
			}
			pkb.parentKB.addParent(3, 4);

			PQL::RelationClause relation;
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::WILDCARD, "_");
			relation.secondStmt = std::make_pair(ArgType::WILDCARD, "_");
			std::unordered_map<std::string, DesignEntity> synonymTable;

			ClauseResult clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb, relation, synonymTable);
			Assert::IsTrue(!clauseResult.empty() && clauseResult[0]["_RESULT"] == "TRUE");

			// Test an unsuccessful instance of Parent(_, _)
			PKB::PKB pkb2;
			
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::WILDCARD, "_");
			relation.secondStmt = std::make_pair(ArgType::WILDCARD, "_");

			clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb2, relation, synonymTable);
			Assert::IsTrue(clauseResult.empty());
		}

		TEST_METHOD(evaluateParentClauseIntWild) {
			PKB::PKB pkb;
			pkb.parentKB.addParent(3, 4);

			// Test a successful instance of Parent(Int, _)
			PQL::RelationClause relation;
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::INTEGER, "3");
			relation.secondStmt = std::make_pair(ArgType::WILDCARD, "_");
			std::unordered_map<std::string, DesignEntity> synonymTable;

			ClauseResult clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb, relation, synonymTable);
			Assert::IsTrue(!clauseResult.empty() && clauseResult[0]["_RESULT"] == "TRUE");

			// Test an unsuccessful instance of Parent(Int, _)
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::INTEGER, "4");
			relation.secondStmt = std::make_pair(ArgType::WILDCARD, "_");

			clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb, relation, synonymTable);
			Assert::IsTrue(clauseResult.empty());
		
			// Test a successful instance of Parent(_, Int)
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::WILDCARD, "_");
			relation.secondStmt = std::make_pair(ArgType::INTEGER, "4");

			clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb, relation, synonymTable);
			Assert::IsTrue(!clauseResult.empty() && clauseResult[0]["_RESULT"] == "TRUE");

			// Test an unsuccessful instance of Parent(_, Int)
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::WILDCARD, "_");
			relation.secondStmt = std::make_pair(ArgType::INTEGER, "3");

			clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb, relation, synonymTable);
			Assert::IsTrue(clauseResult.empty());
		}

		/*
		TEST_METHOD(evaluateParentClauseIntSyn) {
			PKB::PKB pkb;
			pkb.parentKB.addParent(3, 4);
			std::unordered_map<std::string, DesignEntity> synonymTable;
			synonymTable["s"] = DesignEntity::STATEMENT;

			// Test a successful instance of Parent(Int, Syn)
			PQL::RelationClause relation;
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::INTEGER, "3");
			relation.secondStmt = std::make_pair(ArgType::SYNONYM, "s");

			ClauseResult clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb, relation, synonymTable);
			Assert::IsTrue(!clauseResult.empty() && clauseResult[0]["s"] == "4");

			// Test an unsuccessful instance of Parent(Int, Syn)
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::INTEGER, "4");
			relation.secondStmt = std::make_pair(ArgType::SYNONYM, "s");

			clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb, relation, synonymTable);
			Assert::IsTrue(clauseResult.empty());

			// Test a successful instance of Parent(Syn, Int)
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::SYNONYM, "s");
			relation.secondStmt = std::make_pair(ArgType::INTEGER, "4");

			clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb, relation, synonymTable);
			Assert::IsTrue(!clauseResult.empty() && clauseResult[0]["s"] == "3");

			// Test an unsuccessful instance of Parent(Syn, Int)
			relation.type = RelationType::PARENT;
			relation.firstStmt = std::make_pair(ArgType::SYNONYM, "v");
			relation.secondStmt = std::make_pair(ArgType::INTEGER, "3");

			clauseResult = PQL::ParentEvaluator::evaluateParentClause(pkb, relation, synonymTable);
			Assert::IsTrue(clauseResult.empty());
		}*/
	};

}