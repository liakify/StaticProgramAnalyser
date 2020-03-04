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
	PKB::PKB pkbRecursive;
	PKB::PKB pkbCyclic;
	PKB::PKB pkbNormalAndRecursive;
	PKB::PKB pkbNormalAndCyclic;
	FrontEnd::DesignExtractor DE_callStar;

	TEST_CLASS(TestDesignExtractor_callStar)
	{
	public:
		TEST_CLASS_INITIALIZE(setup) {
			// Valid SIMPLE
			pkbCallStar = PKB::PKB();
			pkbCallStar.procTable.insertProc(Procedure("a", 1));
			pkbCallStar.procTable.insertProc(Procedure("b", 2));
			pkbCallStar.procTable.insertProc(Procedure("c", 3));
			
			pkbCallStar.stmtTable.insertStmt(&CallStmt("b"));
			std::vector<StmtId> v1 = { 1 };
			pkbCallStar.stmtListTable.insertStmtLst(StatementList(v1));
			pkbCallStar.stmtTable.insertStmt(&CallStmt("c"));
			std::vector<StmtId> v2 = { 2 };
			pkbCallStar.stmtListTable.insertStmtLst(StatementList(v2));
			pkbCallStar.stmtTable.insertStmt(&ReadStmt(1));
			std::vector<StmtId> v3 = { 3 };
			pkbCallStar.stmtListTable.insertStmtLst(StatementList(v3));
			
			pkbCallStar.callsKB.addCalls(1, 2);
			pkbCallStar.callsKB.addCalls(2, 3);
			pkbCallStar = DE_callStar.run(pkbCallStar);

			// Recursive call in SIMPLE
			pkbRecursive = PKB::PKB();
			pkbRecursive.procTable.insertProc(Procedure("a", 1));
			pkbRecursive.callsKB.addCalls(1, 1);

			// Cyclic call in SIMPLE
			pkbCyclic = PKB::PKB();
			pkbCyclic.procTable.insertProc(Procedure("a", 1));
			pkbCyclic.procTable.insertProc(Procedure("b", 2));
			pkbCyclic.procTable.insertProc(Procedure("c", 3));
			pkbCyclic.callsKB.addCalls(1, 2);
			pkbCyclic.callsKB.addCalls(2, 3);
			pkbCyclic.callsKB.addCalls(3, 1);

			// Valid DAG and recursive call in SIMPLE
			pkbNormalAndRecursive = PKB::PKB();
			pkbNormalAndRecursive.procTable.insertProc(Procedure("a", 1));
			pkbNormalAndRecursive.callsKB.addCalls(1, 1);
			pkbNormalAndRecursive.procTable.insertProc(Procedure("b", 2));
			pkbNormalAndRecursive.procTable.insertProc(Procedure("c", 3));
			pkbNormalAndRecursive.procTable.insertProc(Procedure("d", 4));
			pkbNormalAndRecursive.callsKB.addCalls(2, 3);
			pkbNormalAndRecursive.callsKB.addCalls(3, 4);

			// Valid DAG and cyclic call in SIMPLE
			pkbNormalAndCyclic = PKB::PKB();
			pkbNormalAndCyclic.procTable.insertProc(Procedure("a", 1));
			pkbNormalAndCyclic.procTable.insertProc(Procedure("b", 2));
			pkbNormalAndCyclic.procTable.insertProc(Procedure("c", 3));
			pkbNormalAndCyclic.callsKB.addCalls(1, 2);
			pkbNormalAndCyclic.callsKB.addCalls(2, 3);
			pkbNormalAndCyclic.procTable.insertProc(Procedure("d", 4));
			pkbNormalAndCyclic.procTable.insertProc(Procedure("e", 5));
			pkbNormalAndCyclic.procTable.insertProc(Procedure("f", 6));
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

		TEST_METHOD(populateCallStar_pkbRecursive) {
			auto lambda = [] { pkbRecursive = DE_callStar.run(pkbRecursive); };
			wchar_t* message = L"CycleDetected";
			Assert::ExpectException<std::domain_error>(lambda, message);
		}

		TEST_METHOD(populateCallStar_pkbCyclic) {
			auto lambda = [] { pkbCyclic = DE_callStar.run(pkbCyclic); };
			wchar_t* message = L"CycleDetected";
			Assert::ExpectException<std::domain_error>(lambda, message);
		}

		TEST_METHOD(populateCallStar_pkbNormalAndRecursive) {
			auto lambda = [] { pkbNormalAndRecursive = DE_callStar.run(pkbNormalAndRecursive); };
			wchar_t* message = L"CycleDetected";
			Assert::ExpectException<std::domain_error>(lambda, message);
		}

		TEST_METHOD(populateCallStar_pkbNormalAndCyclic) {
			auto lambda = [] { pkbNormalAndCyclic = DE_callStar.run(pkbNormalAndCyclic); };
			wchar_t* message = L"CycleDetected";
			Assert::ExpectException<std::domain_error>(lambda, message);
		}
	};
}
