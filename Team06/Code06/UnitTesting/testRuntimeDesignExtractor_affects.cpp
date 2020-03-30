#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "Simple.h"
#include "Constants.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting {
    PKB::PKB pkbAffects;
    FrontEnd::DesignExtractor DE_affects;
    
    // Extra 0 row and column for convenient indexing
    bool expectedAffects[28][28] = {
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   1, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 1,   0, 1, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,   0, 0, 1, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 1,   0, 1, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 1, 0, 0,   0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 1,   0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 1, 0, 0, 0},

        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0}
    };

    TEST_CLASS(TestRuntimeDesignExtractor_affects) {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            /*
            procedure Example {
            1   x = 2;
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
            23    z = x + 1; }
                else {
            24    x = z + x; } }

            procedure r {
            25  while (x!=2) {
            26    z = x + 1;
                }
            27  i = 2 * y;
            }
            */
            pkbAffects = PKB::PKB();

            pkbAffects.varTable.insertVar("x");  // VarId = 1
            pkbAffects.varTable.insertVar("z");  // VarId = 2
            pkbAffects.varTable.insertVar("i");  // VarId = 3
            pkbAffects.varTable.insertVar("y");  // VarId = 4

            Expression var_x = Expression("x", 1, ExprType::VAR);
            Expression var_z = Expression("z", 2, ExprType::VAR);
            Expression var_i = Expression("i", 3, ExprType::VAR);
            Expression var_y = Expression("y", 4, ExprType::VAR);

            pkbAffects.constTable.insertConst("2");  // ConstId = 1
            pkbAffects.constTable.insertConst("3");  // ConstId = 2
            pkbAffects.constTable.insertConst("5");  // ConstId = 3
            pkbAffects.constTable.insertConst("0");  // ConstId = 4
            pkbAffects.constTable.insertConst("1");  // ConstId = 5

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

            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, const_2)));  // StmtId = 1
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, const_3)));  // StmtId = 2
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, const_5)));  // StmtId = 3
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(i_not_equal_0, 2)));  // StmtId = 4
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, x_minus_1)));  // StmtId = 5
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(x_equal_1, 3, 4)));  // StmtId = 6
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, x_plus_1)));  // StmtId = 7
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(4, z_plus_x)));  // StmtId = 8
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, z_plus_x_plus_i)));  // StmtId = 9
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("q")));  // StmtId = 10
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, i_minus_1)));  // StmtId = 11
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("p")));  // StmtId = 12

            pkbAffects.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(x_less_than_0, 6, 8)));  // StmtId = 13
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(i_more_than_0, 7)));  // StmtId = 14
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, z_times_3_plus_two_times_y)));  // StmtId = 15
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("q")));  // StmtId = 16
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, i_minus_1)));  // StmtId = 17
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, x_plus_1)));  // StmtId = 18
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, x_plus_z)));  // StmtId = 19
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, const_1)));  // StmtId = 20
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, z_plus_x_plus_i)));  // StmtId = 21

            pkbAffects.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(x_equal_1, 10, 11)));  // StmtId = 22
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, x_plus_1)));  // StmtId = 23
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, z_plus_x)));  // StmtId = 24
            
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(x_not_equal_2, 13)));  // StmtId = 25
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, x_plus_1)));  // StmtId = 26
            pkbAffects.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, two_times_y)));  // StmtId = 27

            pkbAffects.procTable.insertProc(Procedure("Example", 1));
            pkbAffects.procTable.insertProc(Procedure("p", 5));
            pkbAffects.procTable.insertProc(Procedure("q", 9));
            pkbAffects.procTable.insertProc(Procedure("r", 12));

            StatementList sl1 = StatementList(std::vector<StmtId>{1, 2, 3, 4, 12});
            pkbAffects.stmtListTable.insertStmtLst(sl1);
            StatementList sl2 = StatementList(std::vector<StmtId>{5, 6, 9, 10, 11});
            pkbAffects.stmtListTable.insertStmtLst(sl2);
            StatementList sl3 = StatementList(std::vector<StmtId>{7});
            pkbAffects.stmtListTable.insertStmtLst(sl3);
            StatementList sl4 = StatementList(std::vector<StmtId>{8});
            pkbAffects.stmtListTable.insertStmtLst(sl4);
            StatementList sl5 = StatementList(std::vector<StmtId>{13, 21});
            pkbAffects.stmtListTable.insertStmtLst(sl5);
            StatementList sl6 = StatementList(std::vector<StmtId>{14, 18, 19});
            pkbAffects.stmtListTable.insertStmtLst(sl6);
            StatementList sl7 = StatementList(std::vector<StmtId>{15, 16, 17});
            pkbAffects.stmtListTable.insertStmtLst(sl7);
            StatementList sl8 = StatementList(std::vector<StmtId>{20});
            pkbAffects.stmtListTable.insertStmtLst(sl8);
            StatementList sl9 = StatementList(std::vector<StmtId>{22});
            pkbAffects.stmtListTable.insertStmtLst(sl9);
            StatementList sl10 = StatementList(std::vector<StmtId>{23});
            pkbAffects.stmtListTable.insertStmtLst(sl10);
            StatementList sl11 = StatementList(std::vector<StmtId>{24});
            pkbAffects.stmtListTable.insertStmtLst(sl11);
            StatementList sl12 = StatementList(std::vector<StmtId>{25, 27});
            pkbAffects.stmtListTable.insertStmtLst(sl12);
            StatementList sl13 = StatementList(std::vector<StmtId>{26});
            pkbAffects.stmtListTable.insertStmtLst(sl13);

            pkbAffects = DE_affects.run(pkbAffects);
        }

        TEST_METHOD(affects) {
            pkbAffects.clear();
            for (StmtId id1 = 1; id1 <= pkbAffects.stmtTable.size(); id1++) {
                for (StmtId id2 = 1; id2 <= pkbAffects.stmtTable.size(); id2++) {
                    Assert::IsTrue(pkbAffects.affects(id1, id2) == expectedAffects[id1][id2]);
                }
            }
        }

        TEST_METHOD(affectsGetDirectNodes) {
            for (StmtId id1 = 1; id1 <= pkbAffects.stmtTable.size(); id1++) {
                pkbAffects.clear();
                std::unordered_set<StmtId> affects = pkbAffects.affectsGetDirectNodes(id1, NodeType::SUCCESSOR);
                for (StmtId id2 = 1; id2 <= pkbAffects.stmtTable.size(); id2++) {
                    Assert::IsTrue((affects.find(id2) != affects.end()) == expectedAffects[id1][id2]);
                }
            }

            for (StmtId id1 = 1; id1 <= pkbAffects.stmtTable.size(); id1++) {
                pkbAffects.clear();
                std::unordered_set<StmtId> affectedBy = pkbAffects.affectsGetDirectNodes(id1, NodeType::PREDECESSOR);
                for (StmtId id2 = 1; id2 <= pkbAffects.stmtTable.size(); id2++) {
                    Assert::IsTrue((affectedBy.find(id2) != affectedBy.end()) == expectedAffects[id2][id1]);
                }
            }
        }
    };
}
