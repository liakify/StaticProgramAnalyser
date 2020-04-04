#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting {
    PKB::PKB pkbUsesModifies;
    FrontEnd::DesignExtractor DE_usesModifies;
    
    // Extra 0 row and column for convenient indexing
    bool expectedStmtUses[28][5] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},

        {0, 1, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 1, 1, 0},

        {0, 1, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 1},
        {0, 1, 1, 1, 1},
        {0, 1, 1, 1, 1},

        {0, 0, 1, 0, 1},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 0, 0},

        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},

        {0, 1, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 1}
    };

    bool expectedStmtModifies[28][5] = {
        {0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 1},

        {0, 1, 0, 0, 0},
        {0, 0, 1, 0, 1},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1},
        {0, 0, 1, 0, 0},

        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 1, 1, 0},

        {0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0},

        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},

        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
    };

    bool expectedProcUses[5][5] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1},
        {0, 1, 1, 1, 1},
        {0, 1, 1, 0, 0},
        {0, 1, 1, 0, 1}
    };

    bool expectedProcModifies[5][5] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0}
    };


    TEST_CLASS(TestDesignExtractor_usesModifies) {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            /*
            procedure Example {
            1   read x;
            2   z = 3;
            3   i = 5;
            4   while (i!=0) {
            5     x = x - 1;
            6     if (x==1) then {
            7       z = x + 1; }
                  else {
            8       y = z + x; }
            9     z = z + x + i;
            10    call q;
            11    i = i - 1; }
            12  call p; }

            procedure p {
            13  if (x<0) then {
            14    while (i>0) {
            15      x = z * 3 + 2 * y;
            16      call q;
            17      i = i - 1; }
            18    x = x + 1;
            19    z = x + z; }
                else {
            20    z = 1; }
            21  z = z + x + i; }

            procedure q {
            22  if (x==1) then {
            23    read z; }
                else {
            24    print z; } }

            procedure r {
            25  while (x!=2) {
            26    z = x + z;
                }
            27  print y;
            }
            */
            pkbUsesModifies = PKB::PKB();

            pkbUsesModifies.varTable.insertVar("x");  // VarId = 1
            pkbUsesModifies.varTable.insertVar("z");  // VarId = 2
            pkbUsesModifies.varTable.insertVar("i");  // VarId = 3
            pkbUsesModifies.varTable.insertVar("y");  // VarId = 4

            Expression var_x = Expression("x", 1, ExprType::VAR);
            Expression var_z = Expression("z", 2, ExprType::VAR);
            Expression var_i = Expression("i", 3, ExprType::VAR);
            Expression var_y = Expression("y", 4, ExprType::VAR);

            pkbUsesModifies.constTable.insertConst("2");  // ConstId = 1
            pkbUsesModifies.constTable.insertConst("3");  // ConstId = 2
            pkbUsesModifies.constTable.insertConst("5");  // ConstId = 3
            pkbUsesModifies.constTable.insertConst("0");  // ConstId = 4
            pkbUsesModifies.constTable.insertConst("1");  // ConstId = 5

            Expression const_2 = Expression("2", 1, ExprType::CONST);
            Expression const_3 = Expression("3", 2, ExprType::CONST);
            Expression const_5 = Expression("5", 3, ExprType::CONST);
            Expression const_0 = Expression("0", 4, ExprType::CONST);
            Expression const_1 = Expression("1", 5, ExprType::CONST);

            Expression x_minus_1 = Expression(var_x, const_1, '-');
            Expression x_plus_1 = Expression(var_x, const_1, '+');
            Expression z_plus_x = Expression(var_z, var_x, '+');
            Expression z_plus_x_plus_i = Expression(z_plus_x, var_i, '+');
            Expression i_minus_1 = Expression(var_i, const_1, '-');
            Expression z_times_3 = Expression(var_z, const_3, '*');
            Expression two_times_y = Expression(const_2, var_y, '*');
            Expression z_times_3_plus_two_times_y = Expression(z_times_3, two_times_y, '+');
            Expression x_plus_z = Expression(var_x, var_z, '+');

            CondExpr i_not_equal_0 = CondExpr(var_i, const_0);
            CondExpr x_equal_1 = CondExpr(var_x, const_1);
            CondExpr x_less_than_0 = CondExpr(var_x, const_0);
            CondExpr i_more_than_0 = CondExpr(var_i, const_0);
            CondExpr x_not_equal_2 = CondExpr(var_x, const_2);

            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1)));  // StmtId = 1
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, const_3)));  // StmtId = 2
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, const_5)));  // StmtId = 3
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(i_not_equal_0, 2)));  // StmtId = 4
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, x_minus_1)));  // StmtId = 5
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(x_equal_1, 3, 4)));  // StmtId = 6
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, x_plus_1)));  // StmtId = 7
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(4, z_plus_x)));  // StmtId = 8
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, z_plus_x_plus_i)));  // StmtId = 9
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("q")));  // StmtId = 10
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, i_minus_1)));  // StmtId = 11
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("p")));  // StmtId = 12

            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(x_less_than_0, 6, 8)));  // StmtId = 13
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(i_more_than_0, 7)));  // StmtId = 14
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, z_times_3_plus_two_times_y)));  // StmtId = 15
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("q")));  // StmtId = 16
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, i_minus_1)));  // StmtId = 17
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, x_plus_1)));  // StmtId = 18
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, x_plus_z)));  // StmtId = 19
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, const_1)));  // StmtId = 20
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, z_plus_x_plus_i)));  // StmtId = 21

            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(x_equal_1, 10, 11)));  // StmtId = 22
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(2)));  // StmtId = 23
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<PrintStmt>(new PrintStmt(2)));  // StmtId = 24
            
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(x_not_equal_2, 13)));  // StmtId = 25
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, x_plus_z)));  // StmtId = 26
            pkbUsesModifies.stmtTable.insertStmt(std::shared_ptr<PrintStmt>(new PrintStmt(4)));  // StmtId = 27

            pkbUsesModifies.procTable.insertProc(Procedure("Example", 1));
            pkbUsesModifies.procTable.insertProc(Procedure("p", 5));
            pkbUsesModifies.procTable.insertProc(Procedure("q", 9));
            pkbUsesModifies.procTable.insertProc(Procedure("r", 12));

            StatementList sl1 = StatementList(std::vector<StmtId>{1, 2, 3, 4, 12});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl1);
            StatementList sl2 = StatementList(std::vector<StmtId>{5, 6, 9, 10, 11});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl2);
            StatementList sl3 = StatementList(std::vector<StmtId>{7});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl3);
            StatementList sl4 = StatementList(std::vector<StmtId>{8});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl4);
            StatementList sl5 = StatementList(std::vector<StmtId>{13, 21});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl5);
            StatementList sl6 = StatementList(std::vector<StmtId>{14, 18, 19});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl6);
            StatementList sl7 = StatementList(std::vector<StmtId>{15, 16, 17});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl7);
            StatementList sl8 = StatementList(std::vector<StmtId>{20});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl8);
            StatementList sl9 = StatementList(std::vector<StmtId>{22});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl9);
            StatementList sl10 = StatementList(std::vector<StmtId>{23});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl10);
            StatementList sl11 = StatementList(std::vector<StmtId>{24});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl11);
            StatementList sl12 = StatementList(std::vector<StmtId>{25, 27});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl12);
            StatementList sl13 = StatementList(std::vector<StmtId>{26});
            pkbUsesModifies.stmtListTable.insertStmtLst(sl13);

            pkbUsesModifies = DE_usesModifies.run(pkbUsesModifies);
        }

        TEST_METHOD(uses) {
            for (VarId vid = 1; vid <= pkbUsesModifies.varTable.size(); vid++) {
                for (StmtId sid = 1; sid <= pkbUsesModifies.stmtTable.size(); sid++) {
                    Assert::IsTrue(pkbUsesModifies.usesKB.stmtUses(sid, vid) == expectedStmtUses[sid][vid]);
                }
                for (ProcId pid = 1; pid <= pkbUsesModifies.procTable.size(); pid++) {
                    Assert::IsTrue(pkbUsesModifies.usesKB.procUses(pid, vid) == expectedProcUses[pid][vid]);
                }
            }
            // Out of range
            Assert::IsFalse(pkbUsesModifies.usesKB.procUses(-1, 1));
            Assert::IsFalse(pkbUsesModifies.usesKB.procUses(2, -1));
            Assert::IsFalse(pkbUsesModifies.usesKB.procUses(5, 3));
            Assert::IsFalse(pkbUsesModifies.usesKB.procUses(3, 5));
            Assert::IsFalse(pkbUsesModifies.usesKB.stmtUses(-1, 1));
            Assert::IsFalse(pkbUsesModifies.usesKB.stmtUses(2, -1));
            Assert::IsFalse(pkbUsesModifies.usesKB.stmtUses(28, 3));
            Assert::IsFalse(pkbUsesModifies.usesKB.stmtUses(3, 5));
        }

        TEST_METHOD(modifies) {
            for (VarId vid = 1; vid <= pkbUsesModifies.varTable.size(); vid++) {
                for (StmtId sid = 1; sid <= pkbUsesModifies.stmtTable.size(); sid++) {
                    Assert::IsTrue(pkbUsesModifies.modifiesKB.stmtModifies(sid, vid) == expectedStmtModifies[sid][vid]);
                }
                for (ProcId pid = 1; pid <= pkbUsesModifies.procTable.size(); pid++) {
                    Assert::IsTrue(pkbUsesModifies.modifiesKB.procModifies(pid, vid) == expectedProcModifies[pid][vid]);
                }
            }
            // Out of range
            Assert::IsFalse(pkbUsesModifies.modifiesKB.procModifies(-1, 1));
            Assert::IsFalse(pkbUsesModifies.modifiesKB.procModifies(2, -1));
            Assert::IsFalse(pkbUsesModifies.modifiesKB.procModifies(5, 3));
            Assert::IsFalse(pkbUsesModifies.modifiesKB.procModifies(3, 5));
            Assert::IsFalse(pkbUsesModifies.modifiesKB.stmtModifies(-1, 1));
            Assert::IsFalse(pkbUsesModifies.modifiesKB.stmtModifies(2, -1));
            Assert::IsFalse(pkbUsesModifies.modifiesKB.stmtModifies(28, 3));
            Assert::IsFalse(pkbUsesModifies.modifiesKB.stmtModifies(3, 5));
        }

        TEST_METHOD(allUses) {
            for (StmtId sid = 1; sid <= pkbUsesModifies.stmtTable.size(); sid++) {
                std::unordered_set<VarId> varsUsed = pkbUsesModifies.usesKB.getAllVarsUsedByStmt(sid);
                for (VarId vid = 1; vid <= pkbUsesModifies.varTable.size(); vid++) {
                    Assert::IsTrue((varsUsed.find(vid) != varsUsed.end()) == expectedStmtUses[sid][vid]);
                }
            }

            for (ProcId pid = 1; pid <= pkbUsesModifies.procTable.size(); pid++) {
                std::unordered_set<VarId> varsUsed = pkbUsesModifies.usesKB.getAllVarsUsedByProc(pid);
                for (VarId vid = 1; vid <= pkbUsesModifies.varTable.size(); vid++) {
                    Assert::IsTrue((varsUsed.find(vid) != varsUsed.end()) == expectedProcUses[pid][vid]);
                }
            }

            for (VarId vid = 1; vid <= pkbUsesModifies.varTable.size(); vid++) {
                std::unordered_set<StmtId> stmtUsing = pkbUsesModifies.usesKB.getAllStmtsUsingVar(vid);
                for (StmtId sid = 1; sid <= pkbUsesModifies.stmtTable.size(); sid++) {
                    Assert::IsTrue((stmtUsing.find(sid) != stmtUsing.end()) == expectedStmtUses[sid][vid]);
                }
                std::unordered_set<ProcId> procUsing = pkbUsesModifies.usesKB.getAllProcUsingVar(vid);
                for (ProcId pid = 1; pid <= pkbUsesModifies.procTable.size(); pid++) {
                    Assert::IsTrue((procUsing.find(pid) != procUsing.end()) == expectedProcUses[pid][vid]);
                }
            }
            // Out of range
            Assert::IsTrue(pkbUsesModifies.usesKB.getAllVarsUsedByProc(-1) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.usesKB.getAllStmtsUsingVar(-1) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.usesKB.getAllVarsUsedByProc(5) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.usesKB.getAllStmtsUsingVar(5) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.usesKB.getAllVarsUsedByStmt(-1) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.usesKB.getAllProcUsingVar(-1) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.usesKB.getAllVarsUsedByStmt(28) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.usesKB.getAllProcUsingVar(5) == EMPTY_RESULT);
        }

        TEST_METHOD(allModifies) {
            for (StmtId sid = 1; sid <= pkbUsesModifies.stmtTable.size(); sid++) {
                std::unordered_set<VarId> varsModified = pkbUsesModifies.modifiesKB.getAllVarsModifiedByStmt(sid);
                for (VarId vid = 1; vid <= pkbUsesModifies.varTable.size(); vid++) {
                    Assert::IsTrue((varsModified.find(vid) != varsModified.end()) == expectedStmtModifies[sid][vid]);
                }
            }

            for (ProcId pid = 1; pid <= pkbUsesModifies.procTable.size(); pid++) {
                std::unordered_set<VarId> varsModified = pkbUsesModifies.modifiesKB.getAllVarsModifiedByProc(pid);
                for (VarId vid = 1; vid <= pkbUsesModifies.varTable.size(); vid++) {
                    Assert::IsTrue((varsModified.find(vid) != varsModified.end()) == expectedProcModifies[pid][vid]);
                }
            }

            for (VarId vid = 1; vid <= pkbUsesModifies.varTable.size(); vid++) {
                std::unordered_set<StmtId> stmtModify = pkbUsesModifies.modifiesKB.getAllStmtsModifyVar(vid);
                for (StmtId sid = 1; sid <= pkbUsesModifies.stmtTable.size(); sid++) {
                    Assert::IsTrue((stmtModify.find(sid) != stmtModify.end()) == expectedStmtModifies[sid][vid]);
                }
                std::unordered_set<ProcId> procModify = pkbUsesModifies.modifiesKB.getAllProcModifyVar(vid);
                for (ProcId pid = 1; pid <= pkbUsesModifies.procTable.size(); pid++) {
                    Assert::IsTrue((procModify.find(pid) != procModify.end()) == expectedProcModifies[pid][vid]);
                }
            }
            // Out of range
            Assert::IsTrue(pkbUsesModifies.modifiesKB.getAllVarsModifiedByProc(-1) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.modifiesKB.getAllStmtsModifyVar(-1) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.modifiesKB.getAllVarsModifiedByProc(5) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.modifiesKB.getAllStmtsModifyVar(5) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.modifiesKB.getAllVarsModifiedByStmt(-1) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.modifiesKB.getAllProcModifyVar(-1) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.modifiesKB.getAllVarsModifiedByStmt(28) == EMPTY_RESULT);
            Assert::IsTrue(pkbUsesModifies.modifiesKB.getAllProcModifyVar(5) == EMPTY_RESULT);
        }
    };
}
