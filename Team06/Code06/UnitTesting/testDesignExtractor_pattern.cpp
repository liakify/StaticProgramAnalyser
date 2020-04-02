#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting {
    PKB::PKB pkbPattern;
    FrontEnd::DesignExtractor DE_pattern;

    TEST_CLASS(TestDesignExtractor_pattern) {
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
            10    y = x + i;
            11    i = i - 1; }
            12  i = x + i % (i + x); }

            procedure p {
            13  if (x<0) then {
            14    while (i>0) {
            15      x = z * 3 + 2 * y;
            16      i = z * (3 + 2) * y;
            17      i = i - 1; }
            18    x = x + 1;
            19    z = x + z; }
                else {
            20    z = 1; }
            21  z = z + x + i; }
            */
            pkbPattern = PKB::PKB();

            pkbPattern.varTable.insertVar("x");  // VarId = 1
            pkbPattern.varTable.insertVar("z");  // VarId = 2
            pkbPattern.varTable.insertVar("i");  // VarId = 3
            pkbPattern.varTable.insertVar("y");  // VarId = 4

            Expression var_x = Expression("x", 1, ExprType::VAR);
            Expression var_z = Expression("z", 2, ExprType::VAR);
            Expression var_i = Expression("i", 3, ExprType::VAR);
            Expression var_y = Expression("y", 4, ExprType::VAR);

            pkbPattern.constTable.insertConst("2");  // ConstId = 1
            pkbPattern.constTable.insertConst("3");  // ConstId = 2
            pkbPattern.constTable.insertConst("5");  // ConstId = 3
            pkbPattern.constTable.insertConst("0");  // ConstId = 4
            pkbPattern.constTable.insertConst("1");  // ConstId = 5

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
            Expression x_plus_i = Expression(var_x, var_i, '+');
            Expression i_plus_x = Expression(var_i, var_x, '+');
            Expression i_mod_i_plus_x = Expression(var_i, i_plus_x, '%');
            Expression x_plus_i_mod_i_plus_x = Expression(var_x, i_mod_i_plus_x, '+');
            Expression three_plus_2 = Expression(const_3, const_2, '+');
            Expression z_times_three_plus_2 = Expression(var_z, three_plus_2, '*');
            Expression z_times_three_plus_2_times_y = Expression(z_times_three_plus_2, var_y, '*');

            CondExpr i_not_equal_0 = CondExpr(var_i, const_0);
            CondExpr x_equal_1 = CondExpr(var_x, const_1);
            CondExpr x_less_than_0 = CondExpr(var_x, const_0);
            CondExpr i_more_than_0 = CondExpr(var_i, const_0);
            CondExpr x_not_equal_2 = CondExpr(var_x, const_2);

            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, const_2)));  // StmtId = 1
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, const_3)));  // StmtId = 2
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, const_5)));  // StmtId = 3
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(i_not_equal_0, 2)));  // StmtId = 4
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, x_minus_1)));  // StmtId = 5
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(x_equal_1, 3, 4)));  // StmtId = 6
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, x_plus_1)));  // StmtId = 7
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(4, z_plus_x)));  // StmtId = 8
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, z_plus_x_plus_i)));  // StmtId = 9
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(4, x_plus_i)));  // StmtId = 10
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, i_minus_1)));  // StmtId = 11
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, x_plus_i_mod_i_plus_x)));  // StmtId = 12

            pkbPattern.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(x_less_than_0, 6, 8)));  // StmtId = 13
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(i_more_than_0, 7)));  // StmtId = 14
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, z_times_3_plus_two_times_y)));  // StmtId = 15
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, z_times_three_plus_2_times_y)));  // StmtId = 16
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, i_minus_1)));  // StmtId = 17
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(1, x_plus_1)));  // StmtId = 18
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, x_plus_z)));  // StmtId = 19
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, const_1)));  // StmtId = 20
            pkbPattern.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, z_plus_x_plus_i)));  // StmtId = 21

            pkbPattern.procTable.insertProc(Procedure("Example", 1));
            pkbPattern.procTable.insertProc(Procedure("p", 5));

            StatementList sl1 = StatementList(std::vector<StmtId>{1, 2, 3, 4, 12});
            pkbPattern.stmtListTable.insertStmtLst(sl1);
            StatementList sl2 = StatementList(std::vector<StmtId>{5, 6, 9, 10, 11});
            pkbPattern.stmtListTable.insertStmtLst(sl2);
            StatementList sl3 = StatementList(std::vector<StmtId>{7});
            pkbPattern.stmtListTable.insertStmtLst(sl3);
            StatementList sl4 = StatementList(std::vector<StmtId>{8});
            pkbPattern.stmtListTable.insertStmtLst(sl4);
            StatementList sl5 = StatementList(std::vector<StmtId>{13, 21});
            pkbPattern.stmtListTable.insertStmtLst(sl5);
            StatementList sl6 = StatementList(std::vector<StmtId>{14, 18, 19});
            pkbPattern.stmtListTable.insertStmtLst(sl6);
            StatementList sl7 = StatementList(std::vector<StmtId>{15, 16, 17});
            pkbPattern.stmtListTable.insertStmtLst(sl7);
            StatementList sl8 = StatementList(std::vector<StmtId>{20});
            pkbPattern.stmtListTable.insertStmtLst(sl8);

            pkbPattern = DE_pattern.run(pkbPattern);
        }

        TEST_METHOD(assignPattern) {
            std::unordered_map<Pattern, std::unordered_set<StmtId>> expectedResult = std::unordered_map<Pattern, std::unordered_set<StmtId>>({
                {"((z+x)+i)", std::unordered_set<StmtId>({ 9, 21 })},
                {"_2_", std::unordered_set<StmtId>({ 1, 15, 16 })},
                {"_(x+(i%(i+x)))_", std::unordered_set<StmtId>({ 12 })},
                {"_z_", std::unordered_set<StmtId>({ 8, 9, 15, 16, 19, 21 })},
                {"_x_", std::unordered_set<StmtId>({ 5, 7, 8, 9, 10, 12, 18, 19, 21 })},
                {"_(i%(i+x))_", std::unordered_set<StmtId>({ 12 })},
                {"_(z+x)_", std::unordered_set<StmtId>({ 8, 9, 21 })},
                {"(x+z)", std::unordered_set<StmtId>({ 19 })},
                {"_((z+x)+i)_", std::unordered_set<StmtId>({ 9, 21 })},
                {"_i_", std::unordered_set<StmtId>({ 9, 10, 11, 12, 17, 21 })},
                {"_(3+2)_", std::unordered_set<StmtId>({ 16 })},
                {"2", std::unordered_set<StmtId>({ 1 })},
                {"(x+i)", std::unordered_set<StmtId>({ 10 })}
            });
            for (std::pair<Pattern, std::unordered_set<StmtId>> p : expectedResult) {
                Assert::IsTrue(pkbPattern.patternKB.getAssignPatternStmts(p.first) == p.second);
            }
        }
    };
}
