#include "stdafx.h"
#include "CppUnitTest.h"
#include "PQL.h"
#include "PKB.h"
#include "Simple.h"
#include "Types.h"

using std::string;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PQL;
using namespace SIMPLE;

namespace IntegrationTesting {
    PQL::PQLManager pql = PQL::PQLManager(PKB::PKB());
    std::list<string> results;

    TEST_CLASS(TestPKB_PQL) {
    public:

        string query_selectDesignEntity_STMT = "stmt s; Select s";
        string query_selectDesignEntity_READ = "read r; Select r";
        string query_selectDesignEntity_PRINT = "print pn; Select pn";
        string query_selectDesignEntity_CALL = "call c; Select c";
        string query_selectDesignEntity_WHILE = "while w; Select w";
        string query_selectDesignEntity_IF = "if ifs; Select ifs";
        string query_selectDesignEntity_ASSIGN = "assign a; Select a";
        string query_selectDesignEntity_VAR = "variable v; Select v";
        string query_selectDesignEntity_CONST = "constant cn; Select cn";
        string query_selectDesignEntity_PROGLINE = "prog_line n; Select n";
        string query_selectDesignEntity_PROC = "procedure p; Select p";

        string query_selectAttrRef_p_procName = "procedure p; Select p.procName";
        string query_selectAttrRef_c_procName = "call c; Select c.procName";
        string query_selectAttrRef_v_varName = "variable v; Select v.varName";
        string query_selectAttrRef_r_varName = "read r; Select r.varName";
        string query_selectAttrRef_pn_varName = "print pn; Select pn.varName";
        string query_selectAttrRef_cn_value = "constant cn; Select cn.value";
        string query_selectAttrRef_s_stmt = "stmt s; Select s.stmt#";
        string query_selectAttrRef_r_stmt = "read r; Select r.stmt#";
        string query_selectAttrRef_pn_stmt = "print pn; Select pn.stmt#";
        string query_selectAttrRef_c_stmt = "call c; Select c.stmt#";
        string query_selectAttrRef_w_stmt = "while w; Select w.stmt#";
        string query_selectAttrRef_if_stmt = "if ifs; Select ifs.stmt#";
        string query_selectAttrRef_a_stmt = "assign a; Select a.stmt#";

        //query to test withEvaluator
        string query_selectWith = "assign a; Select a with a.stmt# = 4";
        
        string query_relCond_ModifiesP = "variable v; procedure p; Select p such that Modifies(p,v)";
        string query_relCond_ModifiesS = "variable v; assign a; Select a such that Modifies(a,v)";
        string query_relCond_UsesP = "variable v; procedure p; Select p such that Uses(p,v)";
        string query_relCond_UsesS = "variable v; assign a; Select a such that Uses(a,v)";
        string query_relCond_Calls = "procedure p1, p2; Select p1 such that Calls(p1, p2)";
        string query_relCond_Parent = "stmt s1, s2; Select s1 such that Parent(s1, s2)";
        string query_relCond_Follows = "stmt s1, s2; Select s1 such that Follows(s1, s2)";
        string query_relCond_Next = "prog_line n1, n2; Select n1 such that Next(n1, n2)";
        string query_relCond_Affects = "assign a1, a2; Select a1 such that Affects(a1, a2)";

        // string query_relCond_ModifiesP = "variable v; procedure p; Select p such that Modifies(p,v)";
        // string query_relCond_ModifiesS = "variable v; assign a; Select a such that Modifies(a,v)";
        // string query_relCond_UsesP = "variable v; procedure p; Select p such that Uses(p,v)";
        // string query_relCond_UsesS = "variable v; assign a; Select a such that Uses(a,v)";
        // string query_relCond_Calls = "procedure p1, p2; Select p1 such that Calls(p1, p2)";
        // string query_relCond_Parent = "stmt s1, s2; Select s1 such that Parent(s1, s2)";
        string query_relCond_FollowsStar = "stmt s; Select s such that Follows*(1, s)";
        // string query_relCond_Next = "prog_line n1, n2; Select n1 such that Next(n1, n2)";
        // string query_relCond_Affects = "assign a1, a2; Select a1 such that Affects(a1, a2)";

        string query_patternCond_Assign = "assign a; Select a pattern a (_, _\"a\"_)";
        string query_patternCond_If = "if ifs; variable v; Select v pattern ifs (v, _, _)";
        string query_patternCond_While = "while w; variable v; Select v pattern w (v, _)";

    TEST_CLASS_INITIALIZE(setup) {
        PKB::PKB pkb = PKB::PKB();

        pkb.varTable.insertVar("x"); // VarId = 1
        pkb.varTable.insertVar("y"); // VarId = 2
        pkb.varTable.insertVar("z"); // VarId = 3
        pkb.varTable.insertVar("a"); // VarId = 4

        pkb.constTable.insertConst("5"); // ConstId = 1
        pkb.constTable.insertConst("3"); // ConstId = 2
        pkb.constTable.insertConst("0"); // ConstId = 3

        Expression expr_x = Expression("x", 1, ExprType::VAR);
        Expression expr_y = Expression("y", 2, ExprType::VAR);
        Expression expr_z = Expression("z", 3, ExprType::VAR);
        Expression expr_a = Expression("a", 4, ExprType::VAR);
        Expression expr_5 = Expression("5", 1, ExprType::CONST);
        Expression expr_3 = Expression("3", 2, ExprType::CONST);
        Expression expr_0 = Expression("0", 3, ExprType::CONST);

        Expression expr_y_plus_5 = Expression(expr_y, expr_5, '+');
        Expression expr_x_plus_a = Expression(expr_x, expr_a, '+');

        CondExpr cond_x_greater_y = CondExpr(expr_x, expr_y);
        CondExpr cond_z_greater_0 = CondExpr(expr_z, expr_0);

        pkb.stmtListTable.insertStmtLst(StatementList(std::vector<StmtId>({ 1, 2, 3 }))); // StmtListId = 1
        pkb.stmtListTable.insertStmtLst(StatementList(std::vector<StmtId>({ 4 }))); // StmtListId = 2
        pkb.stmtListTable.insertStmtLst(StatementList(std::vector<StmtId>({ 5 }))); // StmtListId = 3
        pkb.stmtListTable.insertStmtLst(StatementList(std::vector<StmtId>({ 6, 7, 9}))); // StmtListId = 4
        pkb.stmtListTable.insertStmtLst(StatementList(std::vector<StmtId>({ 8 }))); // StmtListId = 5

        pkb.procTable.insertProc(Procedure("p", 1)); // ProcId = 1
        pkb.procTable.insertProc(Procedure("p2", 4)); // ProcId = 2

        pkb.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(1))); // StmtId = 1
        pkb.stmtTable.insertStmt(std::shared_ptr<PrintStmt>(new PrintStmt(2))); // StmtId = 2
        pkb.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond_x_greater_y, 2, 3))); // StmtId = 3
        pkb.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(2, expr_y_plus_5))); // StmtId = 4
        pkb.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("p2"))); // StmtId = 5
        pkb.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, expr_3))); // StmtId = 6
        pkb.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond_z_greater_0, 5))); // StmtId = 7
        pkb.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, expr_x_plus_a))); // StmtId = 8
        pkb.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(3, expr_z))); //StmtId = 9

        pkb.followsKB.addFollows(1, 2);
        pkb.followsKB.addFollows(2, 3);
        pkb.followsKB.addFollows(6, 7);
        pkb.followsKB.setAllFollowers(1, std::unordered_set<StmtId>({ 2, 3 }));
        pkb.followsKB.setAllFollowers(2, std::unordered_set<StmtId>({ 3 }));
        pkb.followsKB.setAllFollowers(6, std::unordered_set<StmtId>({ 7 }));
        pkb.followsKB.setAllFollowing(2, std::unordered_set<StmtId>({ 1 }));
        pkb.followsKB.setAllFollowing(3, std::unordered_set<StmtId>({ 1, 2 }));
        pkb.followsKB.setAllFollowing(7, std::unordered_set<StmtId>({ 6 }));

        pkb.parentKB.addParent(3, 4);
        pkb.parentKB.addParent(3, 5);
        pkb.parentKB.addParent(7, 8);

        pkb.usesKB.addStmtUses(2, 2);
        pkb.usesKB.addStmtUses(3, 1);
        pkb.usesKB.addStmtUses(3, 2);
        pkb.usesKB.addStmtUses(3, 4);
        pkb.usesKB.addStmtUses(4, 2);
        pkb.usesKB.addStmtUses(5, 1);
        pkb.usesKB.addStmtUses(5, 4);
        pkb.usesKB.addStmtUses(7, 1);
        pkb.usesKB.addStmtUses(7, 3);
        pkb.usesKB.addStmtUses(7, 4);
        pkb.usesKB.addStmtUses(8, 1);
        pkb.usesKB.addStmtUses(8, 4);
        pkb.usesKB.addStmtUses(9, 3);
        pkb.usesKB.addProcUses(1, 1);
        pkb.usesKB.addProcUses(1, 2);
        pkb.usesKB.addProcUses(1, 4);
        pkb.usesKB.addProcUses(2, 1);
        pkb.usesKB.addProcUses(2, 3);
        pkb.usesKB.addProcUses(2, 4);

        pkb.modifiesKB.addStmtModifies(1, 1);
        pkb.modifiesKB.addStmtModifies(3, 2);
        pkb.modifiesKB.addStmtModifies(3, 3);
        pkb.modifiesKB.addStmtModifies(4, 2);
        pkb.modifiesKB.addStmtModifies(5, 3);
        pkb.modifiesKB.addStmtModifies(6, 3);
        pkb.modifiesKB.addStmtModifies(7, 3);
        pkb.modifiesKB.addStmtModifies(8, 3);
        pkb.modifiesKB.addStmtModifies(9, 2);
        pkb.modifiesKB.addProcModifies(1, 1);
        pkb.modifiesKB.addProcModifies(1, 2);
        pkb.modifiesKB.addProcModifies(1, 3);
        pkb.modifiesKB.addProcModifies(2, 2);
        pkb.modifiesKB.addProcModifies(2, 3);

        pkb.addNext(1, 2);
        pkb.addNext(2, 3);
        pkb.addNext(3, 4);
        pkb.addNext(3, 5);
        pkb.addNext(6, 7);
        pkb.addNext(7, 8);
        pkb.addNext(7, 9);
        pkb.addNext(8, 7);

        pkb.addAffects(6, 9);
        pkb.addAffects(8, 9);

        pkb.callsKB.addCalls(1, 2);

        pkb.patternKB.addAssignPattern(expr_y_plus_5.getStr(), 4);
        pkb.patternKB.addAssignPattern("(y+5)", 4);
        pkb.patternKB.addAssignPattern("_(y+5)_", 4);
        pkb.patternKB.addAssignPattern("_5_", 4);
        pkb.patternKB.addAssignPattern("_y_", 4);
        pkb.patternKB.addAssignPattern(expr_3.getStr(), 6);
        pkb.patternKB.addAssignPattern("_3_", 6);
        pkb.patternKB.addAssignPattern(expr_x_plus_a.getStr(), 8);
        pkb.patternKB.addAssignPattern("(x+a)", 8);
        pkb.patternKB.addAssignPattern("_(x+a)_", 8);
        pkb.patternKB.addAssignPattern("_a_", 8);
        pkb.patternKB.addAssignPattern("_x_", 8);
        pkb.patternKB.addAssignPattern(expr_z.getStr(), 9);
        pkb.patternKB.addAssignPattern("_z_", 9);
        pkb.patternKB.addIfPattern(1, 3);
        pkb.patternKB.addIfPattern(2, 3);
        pkb.patternKB.addWhilePattern(3, 7);

        pql = PQL::PQLManager(pkb);
    }

    TEST_METHOD(evaluateQuery) {
        pql.evaluateQuery(query_selectDesignEntity_STMT, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "1", "2", "3", "4", "5", "6", "7", "8", "9" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectDesignEntity_READ, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "1" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectDesignEntity_PRINT, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "2" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectDesignEntity_CALL, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "5" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectDesignEntity_WHILE, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "7" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectDesignEntity_IF, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "3" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectDesignEntity_ASSIGN, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "4", "6", "8", "9" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectDesignEntity_VAR, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "x", "y", "a", "z" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectDesignEntity_CONST, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "0", "3", "5" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectDesignEntity_PROGLINE, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "1", "2", "3", "4", "5", "6", "7", "8", "9" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectDesignEntity_PROC, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "p", "p2"}).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_p_procName, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "p", "p2" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_c_procName, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "p2" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_v_varName, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "x", "y", "a", "z" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_r_varName, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "x" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_pn_varName, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "y" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_cn_value, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "0", "3", "5" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_s_stmt, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "1", "2", "3", "4", "5", "6", "7", "8", "9" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_r_stmt, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "1" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_pn_stmt, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "2" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_c_stmt, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "5" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_w_stmt, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "7" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_if_stmt, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "3" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectAttrRef_a_stmt, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "4", "6", "8", "9" }).begin()));
        results.clear();

        pql.evaluateQuery(query_selectWith, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "4" }).begin()));
        results.clear();
    }

    TEST_METHOD(evaluateQuery_relCond) {
        pql.evaluateQuery(query_relCond_ModifiesP, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "p", "p2" }).begin()));
        results.clear();

        pql.evaluateQuery(query_relCond_ModifiesS, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "4", "6", "8", "9" }).begin()));
        results.clear();

        pql.evaluateQuery(query_relCond_UsesP, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "p", "p2" }).begin()));
        results.clear();

        pql.evaluateQuery(query_relCond_UsesS, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "4", "8", "9" }).begin()));
        results.clear();

        pql.evaluateQuery(query_relCond_Calls, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "p" }).begin()));
        results.clear();

        pql.evaluateQuery(query_relCond_Parent, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "3", "7" }).begin()));
        results.clear();

        pql.evaluateQuery(query_relCond_Follows, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "1", "2", "6" }).begin()));
        results.clear();

        pql.evaluateQuery(query_relCond_Next, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "1", "2", "3", "6", "7", "8" }).begin()));
        results.clear();

        pql.evaluateQuery(query_relCond_Affects, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "6", "8" }).begin()));
        results.clear();

        pql.evaluateQuery(query_relCond_FollowsStar, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "2", "3" }).begin()));
        results.clear();
    }

    TEST_METHOD(evaluateQuery_patternCond) {
        pql.evaluateQuery(query_patternCond_Assign, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "8" }).begin()));
        results.clear();

        pql.evaluateQuery(query_patternCond_If, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "x", "y" }).begin()));
        results.clear();

        pql.evaluateQuery(query_patternCond_While, results);
        Assert::IsTrue(std::is_permutation(results.begin(), results.end(), std::list<string>({ "z" }).begin()));
        results.clear();
    }

  };
}
