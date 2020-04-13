#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting
{
    PKB::PKB pkbNext;
    FrontEnd::DesignExtractor DE_next;

    bool expectedNext[17][17] = {
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1,   0, 0, 1, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   1, 1, 0, 0, 0,   0, 0, 0, 0, 0},

        {0, 0, 0, 1, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 1, 0,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 1,   0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   1, 0, 0, 1, 0},

        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 1,   0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 1, 0}
    };

    TEST_CLASS(TestDesignExtractor_next)
    {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            /*
            procedure a {
            1   call b;
            }

            procedure b {
            2   read;
            3   while (cond) {
            4       if (cond) then {
            5           read;
                    } else {
            6           read;
                    }
                }
            7   read;
            8   if (cond) then {
            9       while (cond) {
            10          read;
            11          read;
                    }
                } else {
            12      read;
                }
            13  while (cond) {
            14      read;
                }
            }
            */
            pkbNext = PKB::PKB();
            pkbNext.procTable.insertProc(Procedure("a", 1));
            pkbNext.procTable.insertProc(Procedure("b", 2));

            StatementList sl1 = StatementList(std::vector<StmtId>{1});
            pkbNext.stmtListTable.insertStmtLst(sl1);
            StatementList sl2 = StatementList(std::vector<StmtId>{2, 3, 7, 8, 13});
            pkbNext.stmtListTable.insertStmtLst(sl2);
            StatementList sl3 = StatementList(std::vector<StmtId>{4});
            pkbNext.stmtListTable.insertStmtLst(sl3);
            StatementList sl4 = StatementList(std::vector<StmtId>{5});
            pkbNext.stmtListTable.insertStmtLst(sl4);
            StatementList sl5 = StatementList(std::vector<StmtId>{6});
            pkbNext.stmtListTable.insertStmtLst(sl5);
            StatementList sl6 = StatementList(std::vector<StmtId>{9});
            pkbNext.stmtListTable.insertStmtLst(sl6);
            StatementList sl7 = StatementList(std::vector<StmtId>{10, 11});
            pkbNext.stmtListTable.insertStmtLst(sl7);
            StatementList sl8 = StatementList(std::vector<StmtId>{12});
            pkbNext.stmtListTable.insertStmtLst(sl8);
            StatementList sl9 = StatementList(std::vector<StmtId>{14});
            pkbNext.stmtListTable.insertStmtLst(sl9);

            CondExpr cond = CondExpr(Expression("1", 1, ExprType::CONST), Expression("2", 2, ExprType::CONST));

            pkbNext.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("b")));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(2)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 3)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond, 4, 5)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(5)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(6)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(7)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond, 6, 8)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 7)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(10)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(11)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(12)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 9)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(14)));

            pkbNext = DE_next.run(pkbNext);
        }

        TEST_METHOD(updateLastStmtId) {
            std::vector<StmtId> expectedFirst = std::vector<StmtId>({ 1, 2, 4, 5, 6, 9, 10, 12, 14 });
            std::vector<StmtId> expectedMaxLast = std::vector<StmtId>({ 1, 14, 6, 5, 6, 11, 11, 12, 14 });
            Assert::IsTrue(pkbNext.stmtListTable.get(1).getFirst() == 1);
            Assert::IsTrue(pkbNext.stmtListTable.get(1).getMaxLast() == 1);

            std::vector<std::unordered_set<StmtId>> expectedResult = std::vector<std::unordered_set<StmtId>>({
                std::unordered_set<StmtId>({ 1 }),
                std::unordered_set<StmtId>({ 13 }),
                std::unordered_set<StmtId>({ 5, 6 }),
                std::unordered_set<StmtId>({ 5 }),
                std::unordered_set<StmtId>({ 6 }),
                std::unordered_set<StmtId>({ 9 }),
                std::unordered_set<StmtId>({ 11 }),
                std::unordered_set<StmtId>({ 12 }),
                std::unordered_set<StmtId>({ 14 }),
            });
            for (unsigned int i = 1; i <= pkbNext.stmtListTable.size(); i++) {
                StatementList sl = pkbNext.stmtListTable.get(i);
                Assert::IsTrue(sl.getFirst() == expectedFirst[i - 1]);
                Assert::IsTrue(sl.getMaxLast() == expectedMaxLast[i - 1]);
                std::unordered_set<StmtId>& current = sl.getAllEnds();
                Assert::IsTrue(current.size() == expectedResult[i - 1].size());
                Assert::IsTrue(current == expectedResult[i - 1]);
            }
        }

        TEST_METHOD(updateStmtContainerId) {
            std::vector<StmtId> expectedResult = std::vector<StmtListId>({ 1, 2, 2, 3, 4, 5, 2, 2, 6, 7, 7, 8, 2, 9 });
            for (StmtId i = 1; i <= pkbNext.stmtTable.size(); i++) {
                Assert::IsTrue(pkbNext.stmtTable.get(i)->getContainerId() == expectedResult[i - 1]);
            }
        }

        TEST_METHOD(populateNext) {
            for (StmtId id1 = 1; id1 <= pkbNext.stmtTable.size(); id1++) {
                for (StmtId id2 = 1; id2 <= pkbNext.stmtTable.size(); id2++) {
                    Assert::IsTrue(pkbNext.next(id1, id2) == expectedNext[id1][id2]);
                }
            }
        }
    };
}
