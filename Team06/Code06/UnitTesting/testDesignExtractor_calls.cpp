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
	PKB::PKB pkbInvalidProcCall;
	FrontEnd::DesignExtractor DE_calls;

	TEST_CLASS(TestDesignExtractor_calls)
	{
	public:
		wchar_t* message = L"Cycle Detected";
		wchar_t* message2 = L"Invalid procedure call detected";
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

			Statement* CALL_A = new CallStmt("a");
			Statement* CALL_B = new CallStmt("b");
			Statement* CALL_C = new CallStmt("c");
			Statement* CALL_D = new CallStmt("d");
			Statement* CALL_E = new CallStmt("e");
			Statement* CALL_F = new CallStmt("f");
			Statement* READ_1 = new ReadStmt(1);
			Statement* READ_2 = new ReadStmt(2);

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

			pkbCallStar = DE_calls.run(pkbCallStar);

			/* 
				Valid SIMPLE. Calls form X-shaped DAG
				d   b
				 \ /
				  a
				 / \
				c   e
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

			pkbX = DE_calls.run(pkbX);

			/* 
				Valid SIMPLE. Calls form diamond-shaped DAG
				    2
				   / \
				  1   4
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

			pkbDiamond = DE_calls.run(pkbDiamond);

			// Recursive call in SIMPLE
			pkbRecursive = PKB::PKB();
			pkbRecursive.stmtTable.insertStmt(CALL_A);
			pkbRecursive.stmtListTable.insertStmtLst(sl1);
			pkbRecursive.procTable.insertProc(PROC_A);

			// Cyclic call in SIMPLE
			pkbCyclic = PKB::PKB();
			pkbCyclic.procTable.insertProc(PROC_A);
			pkbCyclic.procTable.insertProc(PROC_B);
			pkbCyclic.procTable.insertProc(PROC_C);

			pkbCyclic.stmtTable.insertStmt(CALL_B);
			pkbCyclic.stmtListTable.insertStmtLst(sl1);
			pkbCyclic.stmtTable.insertStmt(CALL_C);
			pkbCyclic.stmtListTable.insertStmtLst(sl2);
			pkbCyclic.stmtTable.insertStmt(CALL_A);
			pkbCyclic.stmtListTable.insertStmtLst(sl3);

			// Valid DAG and recursive call in SIMPLE
			pkbNormalAndRecursive = PKB::PKB();

			pkbNormalAndRecursive.stmtTable.insertStmt(CALL_A);
			pkbNormalAndRecursive.stmtListTable.insertStmtLst(sl1);
			pkbNormalAndRecursive.procTable.insertProc(PROC_A);
			
			pkbNormalAndRecursive.stmtTable.insertStmt(CALL_C);
			pkbNormalAndRecursive.stmtListTable.insertStmtLst(sl2);
			pkbNormalAndRecursive.stmtTable.insertStmt(CALL_D);
			pkbNormalAndRecursive.stmtListTable.insertStmtLst(sl3);
			pkbNormalAndRecursive.stmtTable.insertStmt(READ_1);
			pkbNormalAndRecursive.stmtListTable.insertStmtLst(sl4);

			pkbNormalAndRecursive.procTable.insertProc(PROC_B);
			pkbNormalAndRecursive.procTable.insertProc(PROC_C);
			pkbNormalAndRecursive.procTable.insertProc(PROC_D);

			// Valid DAG and cyclic call in SIMPLE
			pkbNormalAndCyclic = PKB::PKB();
			pkbNormalAndCyclic.stmtTable.insertStmt(CALL_B);
			pkbNormalAndCyclic.stmtListTable.insertStmtLst(sl1);
			pkbNormalAndCyclic.stmtTable.insertStmt(CALL_C);
			pkbNormalAndCyclic.stmtListTable.insertStmtLst(sl2);
			pkbNormalAndCyclic.stmtTable.insertStmt(READ_1);
			pkbNormalAndCyclic.stmtListTable.insertStmtLst(sl3);

			pkbNormalAndCyclic.procTable.insertProc(PROC_A);
			pkbNormalAndCyclic.procTable.insertProc(PROC_B);
			pkbNormalAndCyclic.procTable.insertProc(PROC_C);

			pkbNormalAndCyclic.stmtTable.insertStmt(CALL_E);
			pkbNormalAndCyclic.stmtListTable.insertStmtLst(sl4);
			pkbNormalAndCyclic.stmtTable.insertStmt(CALL_F);
			pkbNormalAndCyclic.stmtListTable.insertStmtLst(sl5);
			pkbNormalAndCyclic.stmtTable.insertStmt(CALL_D);
			pkbNormalAndCyclic.stmtListTable.insertStmtLst(sl6);

			pkbNormalAndCyclic.procTable.insertProc(PROC_D);
			pkbNormalAndCyclic.procTable.insertProc(PROC_E);
			pkbNormalAndCyclic.procTable.insertProc(PROC_F);

			// Invalid procedure call to non-existent procedure
			pkbInvalidProcCall = PKB::PKB();
			pkbInvalidProcCall.stmtTable.insertStmt(CALL_B);
			pkbInvalidProcCall.stmtTable.insertStmt(CALL_C);
			pkbInvalidProcCall.stmtListTable.insertStmtLst(sl7);
			pkbInvalidProcCall.procTable.insertProc(PROC_A);
			pkbInvalidProcCall.stmtTable.insertStmt(READ_1);
			pkbInvalidProcCall.stmtListTable.insertStmtLst(sl3);
			pkbInvalidProcCall.procTable.insertProc(PROC_B);
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
			auto lambda = [] { pkbRecursive = DE_calls.run(pkbRecursive); };
			Assert::ExpectException<std::invalid_argument>(lambda, message);
		}

		TEST_METHOD(populateCallStar_pkbCyclic) {
			auto lambda = [] { pkbCyclic = DE_calls.run(pkbCyclic); };
			Assert::ExpectException<std::invalid_argument>(lambda, message);
		}

		TEST_METHOD(populateCallStar_pkbNormalAndRecursive) {
			auto lambda = [] { pkbNormalAndRecursive = DE_calls.run(pkbNormalAndRecursive); };
			Assert::ExpectException<std::invalid_argument>(lambda, message);
		}

		TEST_METHOD(populateCallStar_pkbNormalAndCyclic) {
			auto lambda = [] { pkbNormalAndCyclic = DE_calls.run(pkbNormalAndCyclic); };
			Assert::ExpectException<std::invalid_argument>(lambda, message);
		}


		TEST_METHOD(populateCallStar_pkbInvalidProcCall) {
			auto lambda = [] { pkbInvalidProcCall = DE_calls.run(pkbInvalidProcCall); };
			Assert::ExpectException<std::invalid_argument>(lambda, message2);
		}
	};
}
