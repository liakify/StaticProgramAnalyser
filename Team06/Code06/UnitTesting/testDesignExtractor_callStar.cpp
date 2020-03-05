#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting
{
	PKB::PKB pkbCallStar;
	PKB::PKB pkbX;
	PKB::PKB pkbDiamond;
	PKB::PKB pkbRecursive;
	PKB::PKB pkbCyclic;
	PKB::PKB pkbNormalAndRecursive;
	PKB::PKB pkbNormalAndCyclic;
	FrontEnd::DesignExtractor DE_callStar;

	TEST_CLASS(TestDesignExtractor_callStar)
	{
	public:
		wchar_t* message = L"Cycle Detected";
		std::unordered_set<ProcId> emptyResult;

		TEST_CLASS_INITIALIZE(setup) {
			Procedure PROC_A = Procedure("a", 1);
			Procedure PROC_B = Procedure("b", 2);
			Procedure PROC_C = Procedure("c", 3);
			Procedure PROC_D = Procedure("d", 4);
			Procedure PROC_E = Procedure("e", 5);
			Procedure PROC_F = Procedure("f", 6);

			StatementList sl1 = StatementList(std::vector<StmtId>{1});
			StatementList sl2 = StatementList(std::vector<StmtId>{2});
			StatementList sl3 = StatementList(std::vector<StmtId>{3});
			StatementList sl4 = StatementList(std::vector<StmtId>{4});
			StatementList sl5 = StatementList(std::vector<StmtId>{5});
			StatementList sl6 = StatementList(std::vector<StmtId>{6});
			StatementList sl7 = StatementList(std::vector<StmtId>{1, 2});
			StatementList sl8 = StatementList(std::vector<StmtId>{2, 3});

			Statement* CALL_A = &CallStmt("a");
			Statement* CALL_B = &CallStmt("b");
			Statement* CALL_C = &CallStmt("c");
			Statement* CALL_D = &CallStmt("d");
			Statement* CALL_E = &CallStmt("e");
			Statement* READ_1 = &ReadStmt(1);
			Statement* READ_2 = &ReadStmt(2);

			// Valid SIMPLE: 1 -> 2 -> 3
			pkbCallStar = PKB::PKB();
			pkbCallStar.procTable.insertProc(PROC_A);
			pkbCallStar.procTable.insertProc(PROC_B);
			pkbCallStar.procTable.insertProc(PROC_C);
			
			pkbCallStar.stmtTable.insertStmt(CALL_B);
			pkbCallStar.stmtListTable.insertStmtLst(sl1);
			pkbCallStar.stmtTable.insertStmt(CALL_C);
			pkbCallStar.stmtListTable.insertStmtLst(sl2);
			pkbCallStar.stmtTable.insertStmt(READ_1);
			pkbCallStar.stmtListTable.insertStmtLst(sl3);
			
			pkbCallStar.callsKB.addCalls(1, 2);
			pkbCallStar.callsKB.addCalls(2, 3);
			pkbCallStar = DE_callStar.run(pkbCallStar);

			/* 
				Valid SIMPLE. Calls form X-shaped DAG
				d	b
				 \ /
				  a
				 / \
				c	e
			*/
			pkbX = PKB::PKB();
			pkbX.procTable.insertProc(PROC_A);
			pkbX.procTable.insertProc(PROC_B);
			pkbX.procTable.insertProc(PROC_C);
			pkbX.procTable.insertProc(PROC_D);
			pkbX.procTable.insertProc(PROC_E);

			pkbX.stmtTable.insertStmt(CALL_C);
			pkbX.stmtTable.insertStmt(CALL_E);
			pkbX.stmtListTable.insertStmtLst(sl7);
			pkbX.stmtTable.insertStmt(CALL_A);
			pkbX.stmtListTable.insertStmtLst(sl3);
			pkbX.stmtTable.insertStmt(READ_1);
			pkbX.stmtListTable.insertStmtLst(sl4);
			pkbX.stmtTable.insertStmt(CALL_A);
			pkbX.stmtListTable.insertStmtLst(sl5);
			pkbX.stmtTable.insertStmt(READ_2);
			pkbX.stmtListTable.insertStmtLst(sl6);

			pkbX.callsKB.addCalls(4, 1);
			pkbX.callsKB.addCalls(2, 1);
			pkbX.callsKB.addCalls(1, 3);
			pkbX.callsKB.addCalls(1, 5);
			pkbX = DE_callStar.run(pkbX);

			/* 
				Valid SIMPLE. Calls form diamond-shaped DAG
					2
				   / \
				  1	  4
				   \ /
				    3
			*/
			pkbDiamond = PKB::PKB();
			pkbDiamond.procTable.insertProc(PROC_A);
			pkbDiamond.procTable.insertProc(PROC_B);
			pkbDiamond.procTable.insertProc(PROC_C);
			pkbDiamond.procTable.insertProc(PROC_D);

			pkbDiamond.stmtTable.insertStmt(CALL_C);
			pkbDiamond.stmtListTable.insertStmtLst(sl1);
			pkbDiamond.stmtTable.insertStmt(CALL_A);
			pkbDiamond.stmtTable.insertStmt(CALL_D);
			pkbDiamond.stmtListTable.insertStmtLst(sl8);
			pkbDiamond.stmtTable.insertStmt(READ_1);
			pkbDiamond.stmtListTable.insertStmtLst(sl4);
			pkbDiamond.stmtTable.insertStmt(CALL_C);
			pkbDiamond.stmtListTable.insertStmtLst(sl5);

			pkbDiamond.callsKB.addCalls(2, 1);
			pkbDiamond.callsKB.addCalls(2, 4);
			pkbDiamond.callsKB.addCalls(1, 3);
			pkbDiamond.callsKB.addCalls(4, 3);
			pkbDiamond = DE_callStar.run(pkbDiamond);

			// Recursive call in SIMPLE
			pkbRecursive = PKB::PKB();
			pkbRecursive.procTable.insertProc(PROC_A);
			pkbRecursive.callsKB.addCalls(1, 1);

			// Cyclic call in SIMPLE
			pkbCyclic = PKB::PKB();
			pkbCyclic.procTable.insertProc(PROC_A);
			pkbCyclic.procTable.insertProc(PROC_B);
			pkbCyclic.procTable.insertProc(PROC_C);
			pkbCyclic.callsKB.addCalls(1, 2);
			pkbCyclic.callsKB.addCalls(2, 3);
			pkbCyclic.callsKB.addCalls(3, 1);

			// Valid DAG and recursive call in SIMPLE
			pkbNormalAndRecursive = PKB::PKB();
			pkbNormalAndRecursive.procTable.insertProc(PROC_A);
			pkbNormalAndRecursive.callsKB.addCalls(1, 1);
			pkbNormalAndRecursive.procTable.insertProc(PROC_B);
			pkbNormalAndRecursive.procTable.insertProc(PROC_C);
			pkbNormalAndRecursive.procTable.insertProc(PROC_D);
			pkbNormalAndRecursive.callsKB.addCalls(2, 3);
			pkbNormalAndRecursive.callsKB.addCalls(3, 4);

			// Valid DAG and cyclic call in SIMPLE
			pkbNormalAndCyclic = PKB::PKB();
			pkbNormalAndCyclic.procTable.insertProc(PROC_A);
			pkbNormalAndCyclic.procTable.insertProc(PROC_B);
			pkbNormalAndCyclic.procTable.insertProc(PROC_C);
			pkbNormalAndCyclic.callsKB.addCalls(1, 2);
			pkbNormalAndCyclic.callsKB.addCalls(2, 3);
			pkbNormalAndCyclic.procTable.insertProc(PROC_D);
			pkbNormalAndCyclic.procTable.insertProc(PROC_E);
			pkbNormalAndCyclic.procTable.insertProc(PROC_F);
			pkbNormalAndCyclic.callsKB.addCalls(4, 5);
			pkbNormalAndCyclic.callsKB.addCalls(5, 6);
			pkbNormalAndCyclic.callsKB.addCalls(6, 4);
		}

		TEST_METHOD(populateCallStar_pkbCallStar) {
			Assert::IsTrue(pkbCallStar.callsKB.callStar(1, 2));
			Assert::IsTrue(pkbCallStar.callsKB.callStar(2, 3));
			Assert::IsTrue(pkbCallStar.callsKB.callStar(1, 3));

			Assert::IsFalse(pkbCallStar.callsKB.callStar(2, 1));
			Assert::IsFalse(pkbCallStar.callsKB.callStar(3, 1));
			Assert::IsFalse(pkbCallStar.callsKB.callStar(3, 2));

			Assert::IsFalse(pkbCallStar.callsKB.callStar(0, 1));
			Assert::IsFalse(pkbCallStar.callsKB.callStar(1, -1));
			Assert::IsFalse(pkbCallStar.callsKB.callStar(4, -1));
		}

		TEST_METHOD(populateCallStar_pkbX) {
			Assert::IsTrue(pkbX.callsKB.callStar(1, 3));
			Assert::IsTrue(pkbX.callsKB.callStar(1, 5));
			Assert::IsTrue(pkbX.callsKB.callStar(4, 1));
			Assert::IsTrue(pkbX.callsKB.callStar(4, 3));
			Assert::IsTrue(pkbX.callsKB.callStar(4, 5));
			Assert::IsTrue(pkbX.callsKB.callStar(2, 1));
			Assert::IsTrue(pkbX.callsKB.callStar(2, 3));
			Assert::IsTrue(pkbX.callsKB.callStar(2, 5));

			Assert::IsFalse(pkbX.callsKB.callStar(4, 2));
			Assert::IsFalse(pkbX.callsKB.callStar(2, 4));
			Assert::IsFalse(pkbX.callsKB.callStar(1, 4));
			Assert::IsFalse(pkbX.callsKB.callStar(1, 2));
			Assert::IsTrue(pkbX.callsKB.getAllNodes(1, NodeType::SUCCESSOR) == std::unordered_set<ProcId>{3, 5});
			Assert::IsTrue(pkbX.callsKB.getAllNodes(2, NodeType::SUCCESSOR) == std::unordered_set<ProcId>{1, 3, 5});
			Assert::IsTrue(pkbX.callsKB.getAllNodes(3, NodeType::SUCCESSOR) == emptyResult);
			Assert::IsTrue(pkbX.callsKB.getAllNodes(4, NodeType::SUCCESSOR) == std::unordered_set<ProcId>{1, 3, 5});
			Assert::IsTrue(pkbX.callsKB.getAllNodes(5, NodeType::SUCCESSOR) == emptyResult);
			Assert::IsTrue(pkbX.callsKB.getAllNodes(1, NodeType::PREDECESSOR) == std::unordered_set<ProcId>{2, 4});
			Assert::IsTrue(pkbX.callsKB.getAllNodes(2, NodeType::PREDECESSOR) == emptyResult);
			Assert::IsTrue(pkbX.callsKB.getAllNodes(3, NodeType::PREDECESSOR) == std::unordered_set<ProcId>{1, 2, 4});
			Assert::IsTrue(pkbX.callsKB.getAllNodes(4, NodeType::PREDECESSOR) == emptyResult);
			Assert::IsTrue(pkbX.callsKB.getAllNodes(5, NodeType::PREDECESSOR) == std::unordered_set<ProcId>{1, 2, 4});

			Assert::IsFalse(pkbX.callsKB.callStar(0, 1));
			Assert::IsFalse(pkbX.callsKB.callStar(1, -1));
			Assert::IsFalse(pkbX.callsKB.callStar(6, -1));
		}

		TEST_METHOD(populateCallStar_pkbDiamond) {
			Assert::IsTrue(pkbDiamond.callsKB.callStar(2, 1));
			Assert::IsTrue(pkbDiamond.callsKB.callStar(2, 4));
			Assert::IsTrue(pkbDiamond.callsKB.callStar(2, 3));
			Assert::IsTrue(pkbDiamond.callsKB.callStar(1, 3));
			Assert::IsTrue(pkbDiamond.callsKB.callStar(4, 3));

			Assert::IsFalse(pkbDiamond.callsKB.callStar(4, 2));
			Assert::IsFalse(pkbDiamond.callsKB.callStar(1, 2));
			Assert::IsFalse(pkbDiamond.callsKB.callStar(3, 1));
			Assert::IsFalse(pkbDiamond.callsKB.callStar(3, 2));
			Assert::IsFalse(pkbDiamond.callsKB.callStar(3, 4));
			Assert::IsTrue(pkbDiamond.callsKB.getAllNodes(1, NodeType::SUCCESSOR) == std::unordered_set<ProcId>{3});
			Assert::IsTrue(pkbDiamond.callsKB.getAllNodes(2, NodeType::SUCCESSOR) == std::unordered_set<ProcId>{1, 3, 4});
			Assert::IsTrue(pkbDiamond.callsKB.getAllNodes(3, NodeType::SUCCESSOR) == emptyResult);
			Assert::IsTrue(pkbDiamond.callsKB.getAllNodes(4, NodeType::SUCCESSOR) == std::unordered_set<ProcId>{3});
			Assert::IsTrue(pkbDiamond.callsKB.getAllNodes(1, NodeType::PREDECESSOR) == std::unordered_set<ProcId>{2});
			Assert::IsTrue(pkbDiamond.callsKB.getAllNodes(2, NodeType::PREDECESSOR) == emptyResult);
			Assert::IsTrue(pkbDiamond.callsKB.getAllNodes(3, NodeType::PREDECESSOR) == std::unordered_set<ProcId>{1, 2, 4});
			Assert::IsTrue(pkbDiamond.callsKB.getAllNodes(4, NodeType::PREDECESSOR) == std::unordered_set<ProcId>{2});

			Assert::IsFalse(pkbDiamond.callsKB.callStar(0, 1));
			Assert::IsFalse(pkbDiamond.callsKB.callStar(1, -1));
			Assert::IsFalse(pkbDiamond.callsKB.callStar(6, -1));
		}

		TEST_METHOD(populateCallStar_pkbRecursive) {
			auto lambda = [] { pkbRecursive = DE_callStar.run(pkbRecursive); };
			Assert::ExpectException<std::domain_error>(lambda, message);
		}

		TEST_METHOD(populateCallStar_pkbCyclic) {
			auto lambda = [] { pkbCyclic = DE_callStar.run(pkbCyclic); };
			Assert::ExpectException<std::domain_error>(lambda, message);
		}

		TEST_METHOD(populateCallStar_pkbNormalAndRecursive) {
			auto lambda = [] { pkbNormalAndRecursive = DE_callStar.run(pkbNormalAndRecursive); };
			Assert::ExpectException<std::domain_error>(lambda, message);
		}

		TEST_METHOD(populateCallStar_pkbNormalAndCyclic) {
			auto lambda = [] { pkbNormalAndCyclic = DE_callStar.run(pkbNormalAndCyclic); };
			Assert::ExpectException<std::domain_error>(lambda, message);
		}
	};
}
