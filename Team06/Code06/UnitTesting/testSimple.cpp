#include "stdafx.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting {
    TEST_CLASS(TestSimple) {
    public:
        ProcName PROC_NAME_1 = "pRoCc";
        ProcName PROC_NAME_2 = "proCc";
        StmtListId STMTLIST_ID_1 = 2;
        StmtListId STMTLIST_ID_2 = 5;
        StmtId STMT_ID_1 = 1;
        StmtId STMT_ID_2 = 2;
        VarName VAR_NAME_1 = "qwerty";
        VarName VAR_NAME_2 = "asdf";
        char op = '%';
        VarId VAR_ID_1 = 1;
        VarId VAR_ID_2 = 3;
        ConstValue CONST_VALUE = "999";
        ConstId CONST_ID = 1;
        std::string PATTERN_1 = "_" + VAR_NAME_1 + "_";
        std::string PATTERN_2 = "_" + VAR_NAME_2 + "_";
        std::string PATTERN_3 = "_" + CONST_VALUE + "_";

        Expression varExp = Expression(VAR_NAME_1, VAR_ID_1, ExprType::VAR);
        Expression varExp2 = Expression(VAR_NAME_2, VAR_ID_2, ExprType::VAR);
        Expression constExp = Expression(CONST_VALUE, CONST_ID, ExprType::CONST);
        Expression exp = Expression(constExp, varExp, op);
        Expression exp2 = Expression(varExp2, exp, op);

        CondExpr cond1 = CondExpr(varExp2, varExp2);
        CondExpr cond2 = CondExpr(constExp, exp);
        CondExpr cond3 = CondExpr(cond1, cond2);

        TEST_METHOD(TestExpression) {
            std::unordered_set<VarId> varSet;
            std::unordered_set<ConstId> constSet;
            std::unordered_set<std::string> patterns;

            Assert::AreEqual(VAR_NAME_1, varExp.getStr());
            Assert::IsTrue(varExp.getConstIds() == std::unordered_set<ConstId>());
            varSet.insert(VAR_ID_1);
            Assert::IsTrue(varExp.getVarIds() == varSet);
            patterns.insert(PATTERN_1);
            Assert::IsTrue(varExp.getPatterns() == patterns);

            Assert::AreEqual(CONST_VALUE, constExp.getStr());
            Assert::IsTrue(constExp.getVarIds() == std::unordered_set<VarId>());
            constSet.insert(CONST_ID);
            Assert::IsTrue(constExp.getConstIds() == constSet);
            patterns = std::unordered_set<std::string>();
            patterns.insert(PATTERN_3);
            Assert::IsTrue(constExp.getPatterns() == patterns);

            std::string expStr = "(" + CONST_VALUE + op + VAR_NAME_1 + ")";
            Assert::AreEqual(expStr, exp.getStr());
            Assert::AreEqual(op, exp.getOp());
            Assert::IsTrue(exp.getVarIds() == varSet);
            Assert::IsTrue(exp.getConstIds() == constSet);
            patterns.insert(PATTERN_1);
            patterns.insert("_" + expStr + "_");
            Assert::IsTrue(exp.getPatterns() == patterns);

            std::string exp2Str =
                "(" + VAR_NAME_2 + op + "(" + CONST_VALUE + op + VAR_NAME_1 + "))";
            varSet.insert(VAR_ID_2);
            Assert::IsTrue(exp2.getVarIds() == varSet);
            Assert::IsTrue(exp2.getConstIds() == constSet);
            patterns.insert(PATTERN_2);
            patterns.insert("_" + exp2Str + "_");
            Assert::IsTrue(exp2.getPatterns() == patterns);
        }

        TEST_METHOD(TestCondExpr) {
            std::unordered_set<VarId> varSet, varSet2;
            std::unordered_set<ConstId> constSet;

            varSet.insert(VAR_ID_2);
            Assert::IsTrue(cond1.getVarIds() == varSet);
            Assert::IsTrue(cond1.getConstIds() == constSet);

            varSet2.insert(VAR_ID_1);
            constSet.insert(CONST_ID);
            Assert::IsTrue(cond2.getVarIds() == varSet2);
            Assert::IsTrue(cond2.getConstIds() == constSet);

            varSet.insert(VAR_ID_1);
            Assert::IsTrue(cond3.getVarIds() == varSet);
            Assert::IsTrue(cond3.getConstIds() == constSet);
        }

        TEST_METHOD(TestStatements) {
            PrintStmt PRINT_STMT = PrintStmt(VAR_ID_1);
            Assert::IsTrue(StmtType::PRINT == PRINT_STMT.getType());
            Assert::AreEqual(VAR_ID_1, PRINT_STMT.getVar());

            ReadStmt READ_STMT = ReadStmt(VAR_ID_2);
            Assert::IsTrue(StmtType::READ == READ_STMT.getType());
            Assert::AreEqual(VAR_ID_2, READ_STMT.getVar());

            CallStmt CALL_STMT = CallStmt(PROC_NAME_1);
            Assert::IsTrue(StmtType::CALL == CALL_STMT.getType());
            Assert::AreEqual(PROC_NAME_1, CALL_STMT.getProc());

            IfStmt IF_STMT = IfStmt(cond2, STMTLIST_ID_1, STMTLIST_ID_2);
            Assert::IsTrue(StmtType::IF == IF_STMT.getType());
            Assert::AreEqual(STMTLIST_ID_1, IF_STMT.getThenStmtLstId());
            Assert::AreEqual(STMTLIST_ID_2, IF_STMT.getElseStmtLstId());
            Assert::IsTrue(cond2.getVarIds() == IF_STMT.getCondExpr().getVarIds());
            Assert::IsTrue(cond2.getConstIds() == IF_STMT.getCondExpr().getConstIds());

            WhileStmt WHILE_STMT = WhileStmt(cond2, STMTLIST_ID_1);
            Assert::IsTrue(StmtType::WHILE == WHILE_STMT.getType());
            Assert::AreEqual(STMTLIST_ID_1, WHILE_STMT.getStmtLstId());
            Assert::IsTrue(cond2.getVarIds() == WHILE_STMT.getCondExpr().getVarIds());
            Assert::IsTrue(cond2.getConstIds() == WHILE_STMT.getCondExpr().getConstIds());

            AssignStmt ASSIGN_STMT = AssignStmt(VAR_ID_2, exp2);
            Assert::IsTrue(StmtType::ASSIGN == ASSIGN_STMT.getType());
            Assert::AreEqual(VAR_ID_2, ASSIGN_STMT.getVar());
            Assert::AreEqual(exp2.getStr(), ASSIGN_STMT.getExpr().getStr());
            Assert::IsTrue(exp2.getVarIds() == ASSIGN_STMT.getExpr().getVarIds());
            Assert::IsTrue(exp2.getConstIds() == ASSIGN_STMT.getExpr().getConstIds());
        }

        TEST_METHOD(TestStmtLst) {
            std::vector<StmtId> sid1, sid2;
            sid1.push_back(STMT_ID_1);
            sid1.push_back(STMT_ID_2);
            sid2.push_back(STMT_ID_2);
            sid2.push_back(STMT_ID_1);
            StatementList STATEMENT_LIST_1 = StatementList(sid1);
            Assert::IsTrue(STMT_ID_1 == STATEMENT_LIST_1.getFirst());
            Assert::IsTrue(STMT_ID_2 == STATEMENT_LIST_1.getLast());
            StatementList STATEMENT_LIST_2 = StatementList(sid1);
            Assert::IsTrue(sid1 == STATEMENT_LIST_1.getStmtIds());
            Assert::IsTrue(STATEMENT_LIST_1 == STATEMENT_LIST_2);


            Assert::ExpectException<std::invalid_argument>([&STATEMENT_LIST_2] { STATEMENT_LIST_2.addLast(0); });
            std::unordered_set<StmtId> expectedSet = std::unordered_set<StmtId>();
            Assert::IsTrue(STATEMENT_LIST_2.getAllLast().size() == expectedSet.size());
            Assert::IsTrue(STATEMENT_LIST_2.getLast() == 2);
            for (StmtId i : expectedSet) {
                Assert::IsTrue(STATEMENT_LIST_2.getAllLast().find(i) != STATEMENT_LIST_2.getAllLast().end());
            }
            for (StmtId i : STATEMENT_LIST_2.getAllLast()) {
                Assert::IsTrue(expectedSet.find(i) != expectedSet.end());
            }

            STATEMENT_LIST_2.addLast(3);
            expectedSet.erase(2);
            expectedSet.insert(3);
            Assert::IsTrue(STATEMENT_LIST_2.getAllLast().size() == expectedSet.size());
            Assert::IsTrue(STATEMENT_LIST_2.getLast() == 3);
            for (StmtId i : expectedSet) {
                Assert::IsTrue(STATEMENT_LIST_2.getAllLast().find(i) != STATEMENT_LIST_2.getAllLast().end());
            }
            for (StmtId i : STATEMENT_LIST_2.getAllLast()) {
                Assert::IsTrue(expectedSet.find(i) != expectedSet.end());
            }

            STATEMENT_LIST_2.addLast(5);
            expectedSet.insert(5);
            Assert::IsTrue(STATEMENT_LIST_2.getAllLast().size() == expectedSet.size());
            Assert::IsTrue(STATEMENT_LIST_2.getLast() == 5);
            for (StmtId i : expectedSet) {
                Assert::IsTrue(STATEMENT_LIST_2.getAllLast().find(i) != STATEMENT_LIST_2.getAllLast().end());
            }
            for (StmtId i : STATEMENT_LIST_2.getAllLast()) {
                Assert::IsTrue(expectedSet.find(i) != expectedSet.end());
            }

            STATEMENT_LIST_2.addLast(4);
            expectedSet.insert(4);
            Assert::IsTrue(STATEMENT_LIST_2.getAllLast().size() == expectedSet.size());
            Assert::IsTrue(STATEMENT_LIST_2.getLast() == 5);
            for (StmtId i : expectedSet) {
                Assert::IsTrue(STATEMENT_LIST_2.getAllLast().find(i) != STATEMENT_LIST_2.getAllLast().end());
            }
            for (StmtId i : STATEMENT_LIST_2.getAllLast()) {
                Assert::IsTrue(expectedSet.find(i) != expectedSet.end());
            }

            Assert::IsTrue(STATEMENT_LIST_1 == STATEMENT_LIST_2);
        }

        TEST_METHOD(TestProcedure) {
            Procedure PROC_1 = Procedure(PROC_NAME_1, STMTLIST_ID_1);
            Procedure PROC_2 = Procedure(PROC_NAME_1, STMTLIST_ID_2);
            Assert::AreEqual(PROC_NAME_1, PROC_1.getName());
            Assert::AreEqual(STMTLIST_ID_2, PROC_2.getStmtLstId());
            Assert::IsTrue(PROC_1 == PROC_2);

            PROC_2 = Procedure(PROC_NAME_2, STMTLIST_ID_1);
            Assert::IsFalse(PROC_1 == PROC_2);
        }
    };
}