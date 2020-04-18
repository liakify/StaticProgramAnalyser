#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting {
    PKB::PKB pkbContains;
    FrontEnd::DesignExtractor DE_contains;
    
    // Extra 0 row and column for convenient indexing
    bool expectedContains[5][28] = {
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0},
        {0, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 1, 1,   1, 1, 1, 1, 1,   1, 1, 0, 0, 0,   0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 1, 1, 1,   0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   1, 1, 1}
    };


    TEST_CLASS(TestDesignExtractor_contains) {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            /*
            procedure Example {
            1   read x;
            2   read x;
            3   read x;
            4   while (i!=0) {
            5     read x;
            6     if (x==1) then {
            7       read x; }
                  else {
            8       read x; }
            9     read x;
            10    call q;
            11    read x; }
            12  call p; }

            procedure p {
            13  if (x<0) then {
            14    while (i>0) {
            15      read x;
            16      call q;
            17      read x; }
            18    read x;
            19    read x; }
                else {
            20    read x; }
            21  read x; }

            procedure q {
            22  if (x==1) then {
            23    read x; }
                else {
            24    read x; } }

            procedure r {
            25  while (x!=2) {
            26    read x;
                }
            27  read x;
            }
            */
            pkbContains = PKB::PKB();

            pkbContains.varTable.insertVar("x");  // VarId = 1
            pkbContains.varTable.insertVar("i");  // VarId = 2

            Expression var_x = Expression("x", 1, ExprType::VAR);
            Expression var_i = Expression("i", 2, ExprType::VAR);

            pkbContains.constTable.insertConst("0");  // ConstId = 1
            pkbContains.constTable.insertConst("1");  // ConstId = 2
            pkbContains.constTable.insertConst("2");  // ConstId = 3

            Expression const_0 = Expression("0", 1, ExprType::CONST);
            Expression const_1 = Expression("1", 2, ExprType::CONST);
            Expression const_2 = Expression("2", 3, ExprType::CONST);

            CondExpr i_not_equal_0 = CondExpr(var_i, const_0);
            CondExpr x_equal_1 = CondExpr(var_x, const_1);
            CondExpr x_less_than_0 = CondExpr(var_x, const_0);
            CondExpr i_more_than_0 = CondExpr(var_i, const_0);
            CondExpr x_not_equal_2 = CondExpr(var_x, const_2);

            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 1
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 2
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 3
            pkbContains.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(i_not_equal_0, 2)));  // StmtId = 4
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 5
            pkbContains.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(x_equal_1, 3, 4)));  // StmtId = 6
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 7
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 8
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 9
            pkbContains.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("q")));  // StmtId = 10
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 11
            pkbContains.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("p")));  // StmtId = 12

            pkbContains.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(x_less_than_0, 6, 8)));  // StmtId = 13
            pkbContains.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(i_more_than_0, 7)));  // StmtId = 14
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 15
            pkbContains.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("q")));  // StmtId = 16
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 17
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 18
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 19
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 20
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 21

            pkbContains.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(x_equal_1, 10, 11)));  // StmtId = 22
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 23
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 24
            
            pkbContains.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(x_not_equal_2, 13)));  // StmtId = 25
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 26
            pkbContains.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 27

            pkbContains.procTable.insertProc(Procedure("Example", 1));
            pkbContains.procTable.insertProc(Procedure("p", 5));
            pkbContains.procTable.insertProc(Procedure("q", 9));
            pkbContains.procTable.insertProc(Procedure("r", 12));

            StatementList sl1 = StatementList(std::vector<StmtId>{1, 2, 3, 4, 12});
            pkbContains.stmtListTable.insertStmtLst(sl1);
            StatementList sl2 = StatementList(std::vector<StmtId>{5, 6, 9, 10, 11});
            pkbContains.stmtListTable.insertStmtLst(sl2);
            StatementList sl3 = StatementList(std::vector<StmtId>{7});
            pkbContains.stmtListTable.insertStmtLst(sl3);
            StatementList sl4 = StatementList(std::vector<StmtId>{8});
            pkbContains.stmtListTable.insertStmtLst(sl4);
            StatementList sl5 = StatementList(std::vector<StmtId>{13, 21});
            pkbContains.stmtListTable.insertStmtLst(sl5);
            StatementList sl6 = StatementList(std::vector<StmtId>{14, 18, 19});
            pkbContains.stmtListTable.insertStmtLst(sl6);
            StatementList sl7 = StatementList(std::vector<StmtId>{15, 16, 17});
            pkbContains.stmtListTable.insertStmtLst(sl7);
            StatementList sl8 = StatementList(std::vector<StmtId>{20});
            pkbContains.stmtListTable.insertStmtLst(sl8);
            StatementList sl9 = StatementList(std::vector<StmtId>{22});
            pkbContains.stmtListTable.insertStmtLst(sl9);
            StatementList sl10 = StatementList(std::vector<StmtId>{23});
            pkbContains.stmtListTable.insertStmtLst(sl10);
            StatementList sl11 = StatementList(std::vector<StmtId>{24});
            pkbContains.stmtListTable.insertStmtLst(sl11);
            StatementList sl12 = StatementList(std::vector<StmtId>{25, 27});
            pkbContains.stmtListTable.insertStmtLst(sl12);
            StatementList sl13 = StatementList(std::vector<StmtId>{26});
            pkbContains.stmtListTable.insertStmtLst(sl13);

            pkbContains = DE_contains.run(pkbContains);
        }

        TEST_METHOD(contains) {
            for (ProcId pid = 1; pid <= pkbContains.procTable.size(); pid++) {
                for (StmtId sid = 1; sid <= pkbContains.stmtTable.size(); sid++) {
                    Assert::IsTrue(pkbContains.containsKB.contains(pid, sid) == expectedContains[pid][sid]);
                }
            }
            // Out of range
            Assert::IsFalse(pkbContains.containsKB.contains(-1, 1));
            Assert::IsFalse(pkbContains.containsKB.contains(0, 1));
            Assert::IsFalse(pkbContains.containsKB.contains(2, -1));
            Assert::IsFalse(pkbContains.containsKB.contains(2, 0));
            Assert::IsFalse(pkbContains.containsKB.contains(5, 3));
            Assert::IsFalse(pkbContains.containsKB.contains(3, 28));
        }

        TEST_METHOD(getContainer) {
            for (StmtId sid = 1; sid <= pkbContains.stmtTable.size(); sid++) {
                Assert::IsTrue(expectedContains[pkbContains.containsKB.getContainer(sid)][sid]);
            }
            // Out of range
            Assert::IsTrue(pkbContains.containsKB.getContainer(-1) == INVALID_PROC_ID);
            Assert::IsTrue(pkbContains.containsKB.getContainer(0) == INVALID_PROC_ID);
            Assert::IsTrue(pkbContains.containsKB.getContainer(28) == INVALID_PROC_ID);
        }

        TEST_METHOD(getAllContains) {
            for (ProcId pid = 1; pid <= pkbContains.procTable.size(); pid++) {
                std::unordered_set<StmtId> stmtsContained = pkbContains.containsKB.getAllContains(pid);
                for (StmtId sid = 1; sid <= pkbContains.stmtTable.size(); sid++) {
                    Assert::IsTrue((stmtsContained.find(sid) != stmtsContained.end()) == expectedContains[pid][sid]);
                }
            }

            // Out of range
            Assert::IsTrue(pkbContains.containsKB.getAllContains(-1) == EMPTY_RESULT);
            Assert::IsTrue(pkbContains.containsKB.getAllContains(0) == EMPTY_RESULT);
            Assert::IsTrue(pkbContains.containsKB.getAllContains(5) == EMPTY_RESULT);
        }
    };
}
