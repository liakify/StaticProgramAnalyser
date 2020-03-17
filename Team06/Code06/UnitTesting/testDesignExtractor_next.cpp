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

    TEST_CLASS(TestDesignExtractor_next)
    {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            /*
            procedure a {
                call b;
            }

            procedure b {
                read;
                while (cond) {
                    if (cond) then {
                        read;
                    } else {
                        read;
                    }
                }
                read;
            }
            */
            pkbNext = PKB::PKB();
            pkbNext.procTable.insertProc(Procedure("a", 1));
            pkbNext.procTable.insertProc(Procedure("b", 2));

            StatementList sl1 = StatementList(std::vector<StmtId>{1});
            pkbNext.stmtListTable.insertStmtLst(sl1);
            StatementList sl2 = StatementList(std::vector<StmtId>{2, 3, 7});
            pkbNext.stmtListTable.insertStmtLst(sl2);
            StatementList sl3 = StatementList(std::vector<StmtId>{4});
            pkbNext.stmtListTable.insertStmtLst(sl3);
            StatementList sl4 = StatementList(std::vector<StmtId>{5});
            pkbNext.stmtListTable.insertStmtLst(sl4);
            StatementList sl5 = StatementList(std::vector<StmtId>{6});
            pkbNext.stmtListTable.insertStmtLst(sl5);

            CondExpr cond = CondExpr(Expression("1", 1, ExprType::CONST), Expression("2", 2, ExprType::CONST));

            pkbNext.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("b")));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(2)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 3)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond, 4, 5)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(5)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(6)));
            pkbNext.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(7)));

            pkbNext = DE_next.run(pkbNext);
        }

        TEST_METHOD(updateLastStmtId) {
            Assert::IsTrue(pkbNext.stmtListTable.get(1).getFirst() == 1);
            Assert::IsTrue(pkbNext.stmtListTable.get(1).getLast() == 1);
            Assert::IsTrue(pkbNext.stmtListTable.get(2).getFirst() == 2);
            Assert::IsTrue(pkbNext.stmtListTable.get(2).getLast() == 7);
            Assert::IsTrue(pkbNext.stmtListTable.get(3).getFirst() == 4);
            Assert::IsTrue(pkbNext.stmtListTable.get(3).getLast() == 6);
            Assert::IsTrue(pkbNext.stmtListTable.get(4).getFirst() == 5);
            Assert::IsTrue(pkbNext.stmtListTable.get(4).getLast() == 5);
            Assert::IsTrue(pkbNext.stmtListTable.get(5).getFirst() == 6);
            Assert::IsTrue(pkbNext.stmtListTable.get(5).getLast() == 6);

            std::vector<std::unordered_set<StmtId>> expectedResult = std::vector<std::unordered_set<StmtId>>({
                std::unordered_set<StmtId>({ 1 }),
                std::unordered_set<StmtId>({ 7 }),
                std::unordered_set<StmtId>({ 5, 6 }),
                std::unordered_set<StmtId>({ 5 }),
                std::unordered_set<StmtId>({ 6 })
            });
            for (int i = 1; i <= expectedResult.size(); i++) {
                std::unordered_set<StmtId>& current = pkbNext.stmtListTable.get(i).getAllEnds();
                Assert::IsTrue(current.size() == expectedResult[i - 1].size());
                for (StmtId sid : current) {
                    Assert::IsTrue(expectedResult[i - 1].find(sid) != expectedResult[i - 1].end());
                }
            }
        }

        TEST_METHOD(updateStmtContainerId) {
            Assert::IsTrue(pkbNext.stmtTable.get(1)->getContainerId() == 1);
            Assert::IsTrue(pkbNext.stmtTable.get(2)->getContainerId() == 2);
            Assert::IsTrue(pkbNext.stmtTable.get(3)->getContainerId() == 2);
            Assert::IsTrue(pkbNext.stmtTable.get(4)->getContainerId() == 3);
            Assert::IsTrue(pkbNext.stmtTable.get(5)->getContainerId() == 4);
            Assert::IsTrue(pkbNext.stmtTable.get(6)->getContainerId() == 5);
            Assert::IsTrue(pkbNext.stmtTable.get(7)->getContainerId() == 2);
        }

        TEST_METHOD(populateNext) {
            Assert::IsTrue(pkbNext.nextKB.next(2, 3));
            Assert::IsTrue(pkbNext.nextKB.next(3, 4));
            Assert::IsTrue(pkbNext.nextKB.next(3, 7));
            Assert::IsTrue(pkbNext.nextKB.next(4, 5));
            Assert::IsTrue(pkbNext.nextKB.next(4, 6));
            Assert::IsTrue(pkbNext.nextKB.next(5, 3));
            Assert::IsTrue(pkbNext.nextKB.next(6, 3));

            Assert::IsFalse(pkbNext.nextKB.next(1, 1));
            Assert::IsFalse(pkbNext.nextKB.next(2, 2));
            Assert::IsFalse(pkbNext.nextKB.next(3, 3));
            Assert::IsFalse(pkbNext.nextKB.next(4, 4));
            Assert::IsFalse(pkbNext.nextKB.next(5, 5));
            Assert::IsFalse(pkbNext.nextKB.next(6, 6));
            Assert::IsFalse(pkbNext.nextKB.next(7, 7));

            Assert::IsFalse(pkbNext.nextKB.next(1, 2));
            Assert::IsFalse(pkbNext.nextKB.next(1, 3));
            Assert::IsFalse(pkbNext.nextKB.next(1, 4));
            Assert::IsFalse(pkbNext.nextKB.next(1, 5));
            Assert::IsFalse(pkbNext.nextKB.next(1, 6));
            Assert::IsFalse(pkbNext.nextKB.next(1, 7));
            Assert::IsFalse(pkbNext.nextKB.next(2, 1));
            Assert::IsFalse(pkbNext.nextKB.next(3, 1));
            Assert::IsFalse(pkbNext.nextKB.next(4, 1));
            Assert::IsFalse(pkbNext.nextKB.next(5, 1));
            Assert::IsFalse(pkbNext.nextKB.next(6, 1));
            Assert::IsFalse(pkbNext.nextKB.next(7, 1));

            Assert::IsFalse(pkbNext.nextKB.next(2, 4));
            Assert::IsFalse(pkbNext.nextKB.next(2, 5));
            Assert::IsFalse(pkbNext.nextKB.next(2, 6));
            Assert::IsFalse(pkbNext.nextKB.next(2, 7));
            Assert::IsFalse(pkbNext.nextKB.next(3, 2));
            Assert::IsFalse(pkbNext.nextKB.next(4, 2));
            Assert::IsFalse(pkbNext.nextKB.next(5, 2));
            Assert::IsFalse(pkbNext.nextKB.next(6, 2));
            Assert::IsFalse(pkbNext.nextKB.next(7, 2));

            Assert::IsFalse(pkbNext.nextKB.next(3, 5));
            Assert::IsFalse(pkbNext.nextKB.next(3, 6));
            Assert::IsFalse(pkbNext.nextKB.next(4, 3));
            Assert::IsFalse(pkbNext.nextKB.next(7, 3));

            Assert::IsFalse(pkbNext.nextKB.next(4, 7));
            Assert::IsFalse(pkbNext.nextKB.next(5, 4));
            Assert::IsFalse(pkbNext.nextKB.next(6, 4));
            Assert::IsFalse(pkbNext.nextKB.next(7, 4));

            Assert::IsFalse(pkbNext.nextKB.next(5, 6));
            Assert::IsFalse(pkbNext.nextKB.next(5, 7));
            Assert::IsFalse(pkbNext.nextKB.next(6, 5));
            Assert::IsFalse(pkbNext.nextKB.next(7, 5));

            Assert::IsFalse(pkbNext.nextKB.next(6, 7));
            Assert::IsFalse(pkbNext.nextKB.next(7, 6));
        }
    };
}
