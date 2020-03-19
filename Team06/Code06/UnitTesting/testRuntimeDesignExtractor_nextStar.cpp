#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"
#include "Simple.h"
#include "Constants.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SIMPLE;

namespace UnitTesting {
    PKB::PKB pkbNextStar;
    FrontEnd::DesignExtractor DE_nextStar;

    TEST_CLASS(TestRuntimeDesignExtractor_nextStar) {
    public:
        TEST_CLASS_INITIALIZE(setup) {
            /*
            procedure a {
            1.    call b;
            }

            procedure b {
            2.    read;
            3.    while (cond) {
            4.        read;
            5.        if (cond) then {
            6.            read;
            7.            if (cond) then {
            8.                read;
                          } else {
            9.                read;
                          }
            10.           read;
                      } else {
            11.           read;
            12.           if (cond) then {
            13.               read;
                          } else {
            14.               read;
                          }
            15.           read;
                      }
            16.       read;
                  }
            17.   read;
            }

            procedure c {
            18.    if (cond) {
            19.        while (cond) {
            20.            while (cond) {
            21.                read;
                           }
                       }
                   } else {
            22.        while (cond) {
            23.            while (cond) {
            24.                read;
                           }
                       }
                   }
            }
            */
            pkbNextStar = PKB::PKB();
            pkbNextStar.procTable.insertProc(Procedure("a", 1));
            pkbNextStar.procTable.insertProc(Procedure("b", 2));
            pkbNextStar.procTable.insertProc(Procedure("c", 3));

            StatementList sl1 = StatementList(std::vector<StmtId>{1});
            pkbNextStar.stmtListTable.insertStmtLst(sl1);
            StatementList sl2 = StatementList(std::vector<StmtId>{2, 3, 17});
            pkbNextStar.stmtListTable.insertStmtLst(sl2);
            StatementList sl3 = StatementList(std::vector<StmtId>{4, 5, 16});
            pkbNextStar.stmtListTable.insertStmtLst(sl3);
            StatementList sl4 = StatementList(std::vector<StmtId>{6, 7, 10});
            pkbNextStar.stmtListTable.insertStmtLst(sl4);
            StatementList sl5 = StatementList(std::vector<StmtId>{11, 12, 15});
            pkbNextStar.stmtListTable.insertStmtLst(sl5);
            StatementList sl6 = StatementList(std::vector<StmtId>{8});
            pkbNextStar.stmtListTable.insertStmtLst(sl6);
            StatementList sl7 = StatementList(std::vector<StmtId>{9});
            pkbNextStar.stmtListTable.insertStmtLst(sl7);
            StatementList sl8 = StatementList(std::vector<StmtId>{13});
            pkbNextStar.stmtListTable.insertStmtLst(sl8);
            StatementList sl9 = StatementList(std::vector<StmtId>{14});
            pkbNextStar.stmtListTable.insertStmtLst(sl9);
            StatementList sl10 = StatementList(std::vector<StmtId>{18});
            pkbNextStar.stmtListTable.insertStmtLst(sl10);
            StatementList sl11 = StatementList(std::vector<StmtId>{19});
            pkbNextStar.stmtListTable.insertStmtLst(sl11);
            StatementList sl12 = StatementList(std::vector<StmtId>{20});
            pkbNextStar.stmtListTable.insertStmtLst(sl12);
            StatementList sl13 = StatementList(std::vector<StmtId>{21});
            pkbNextStar.stmtListTable.insertStmtLst(sl13);
            StatementList sl14 = StatementList(std::vector<StmtId>{22});
            pkbNextStar.stmtListTable.insertStmtLst(sl14);
            StatementList sl15 = StatementList(std::vector<StmtId>{23});
            pkbNextStar.stmtListTable.insertStmtLst(sl15);
            StatementList sl16 = StatementList(std::vector<StmtId>{24});
            pkbNextStar.stmtListTable.insertStmtLst(sl16);

            CondExpr cond = CondExpr(Expression("1", 1, ExprType::CONST), Expression("2", 2, ExprType::CONST));

            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<CallStmt>(new CallStmt("b")));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(2)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 3)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(4)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond, 4, 5)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(6)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond, 6, 7)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(8)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(9)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(10)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(11)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond, 8, 9)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(13)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(14)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(15)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(16)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(17)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond, 11, 14)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 12)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 13)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(21)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 15)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 16)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(24)));

            pkbNextStar = DE_nextStar.run(pkbNextStar);
        }

        TEST_METHOD(nextStar) {
            //Assert::IsTrue(pkbNextStar.nextStar(2, 4));
            //Assert::IsTrue(pkbNextStar.nextStar(2, 3));
            //Assert::IsTrue(pkbNextStar.nextStar(3, 4));
            //Assert::IsTrue(pkbNextStar.nextStar(3, 7));
            //Assert::IsTrue(pkbNextStar.nextStar(4, 5));
            //Assert::IsTrue(pkbNextStar.nextStar(4, 6));
            //Assert::IsTrue(pkbNextStar.nextStar(5, 3));
            //Assert::IsTrue(pkbNextStar.nextStar(6, 3));

            //Assert::IsFalse(pkbNextStar.nextStar(1, 1));
            //Assert::IsFalse(pkbNextStar.nextStar(2, 2));
            //Assert::IsFalse(pkbNextStar.nextStar(3, 3));
            //Assert::IsFalse(pkbNextStar.nextStar(4, 4));
            //Assert::IsFalse(pkbNextStar.nextStar(5, 5));
            //Assert::IsFalse(pkbNextStar.nextStar(6, 6));
            //Assert::IsFalse(pkbNextStar.nextStar(7, 7));

            //Assert::IsFalse(pkbNextStar.nextStar(1, 2));
            //Assert::IsFalse(pkbNextStar.nextStar(1, 3));
            //Assert::IsFalse(pkbNextStar.nextStar(1, 4));
            //Assert::IsFalse(pkbNextStar.nextStar(1, 5));
            //Assert::IsFalse(pkbNextStar.nextStar(1, 6));
            //Assert::IsFalse(pkbNextStar.nextStar(1, 7));
            //Assert::IsFalse(pkbNextStar.nextStar(2, 1));
            //Assert::IsFalse(pkbNextStar.nextStar(3, 1));
            //Assert::IsFalse(pkbNextStar.nextStar(4, 1));
            //Assert::IsFalse(pkbNextStar.nextStar(5, 1));
            //Assert::IsFalse(pkbNextStar.nextStar(6, 1));
            //Assert::IsFalse(pkbNextStar.nextStar(7, 1));

            //Assert::IsFalse(pkbNextStar.nextStar(2, 4));
            //Assert::IsFalse(pkbNextStar.nextStar(2, 5));
            //Assert::IsFalse(pkbNextStar.nextStar(2, 6));
            //Assert::IsFalse(pkbNextStar.nextStar(2, 7));
            //Assert::IsFalse(pkbNextStar.nextStar(3, 2));
            //Assert::IsFalse(pkbNextStar.nextStar(4, 2));
            //Assert::IsFalse(pkbNextStar.nextStar(5, 2));
            //Assert::IsFalse(pkbNextStar.nextStar(6, 2));
            //Assert::IsFalse(pkbNextStar.nextStar(7, 2));

            //Assert::IsFalse(pkbNextStar.nextStar(3, 5));
            //Assert::IsFalse(pkbNextStar.nextStar(3, 6));
            //Assert::IsFalse(pkbNextStar.nextStar(4, 3));
            //Assert::IsFalse(pkbNextStar.nextStar(7, 3));

            //Assert::IsFalse(pkbNextStar.nextStar(4, 7));
            //Assert::IsFalse(pkbNextStar.nextStar(5, 4));
            //Assert::IsFalse(pkbNextStar.nextStar(6, 4));
            //Assert::IsFalse(pkbNextStar.nextStar(7, 4));

            //Assert::IsFalse(pkbNextStar.nextStar(5, 6));
            //Assert::IsFalse(pkbNextStar.nextStar(5, 7));
            //Assert::IsFalse(pkbNextStar.nextStar(6, 5));
            //Assert::IsFalse(pkbNextStar.nextStar(7, 5));

            //Assert::IsFalse(pkbNextStar.nextStar(6, 7));
            //Assert::IsFalse(pkbNextStar.nextStar(7, 6));
        }

        TEST_METHOD(processStmtAllNodes) {
            std::unordered_set<StmtId> whileLoopAndAft { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
            std::unordered_set<StmtId> whileLoopAndBef { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(1, NodeType::SUCCESSOR) == EMPTY_RESULT);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(1, NodeType::PREDECESSOR) == EMPTY_RESULT);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(2, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(2, NodeType::PREDECESSOR) == EMPTY_RESULT);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(3, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(3, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(4, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(4, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(5, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(5, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(6, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(6, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(7, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(7, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(8, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(8, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(9, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(9, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(10, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(10, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(11, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(11, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(12, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(12, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(13, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(13, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(14, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(14, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(15, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(15, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(16, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(16, NodeType::PREDECESSOR) == whileLoopAndBef);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(17, NodeType::SUCCESSOR) == EMPTY_RESULT);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(17, NodeType::PREDECESSOR) == whileLoopAndBef);

            //std::unordered_set<StmtId> allAftIf = { 19, 20, 21 };
            //std::unordered_set<StmtId> allAftIfInclIf = { 18, 19, 20, 21 };

            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(18, NodeType::SUCCESSOR) == allAftIf);
            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(18, NodeType::PREDECESSOR) == EMPTY_RESULT);

            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(19, NodeType::SUCCESSOR) == allAftIf);
            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(19, NodeType::PREDECESSOR) == allAftIfInclIf);

            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(20, NodeType::SUCCESSOR) == allAftIf);
            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(20, NodeType::PREDECESSOR) == allAftIfInclIf);

            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(21, NodeType::SUCCESSOR) == allAftIf);
            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(21, NodeType::PREDECESSOR) == allAftIfInclIf);

            //std::unordered_set<StmtId> allAftElse = { 22, 23, 24 };
            //std::unordered_set<StmtId> allAftElseInclIf = { 18, 22, 23, 24 };

            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(22, NodeType::SUCCESSOR) == allAftElse);
            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(22, NodeType::PREDECESSOR) == allAftElseInclIf);

            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(23, NodeType::SUCCESSOR) == allAftElse);
            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(23, NodeType::PREDECESSOR) == allAftElseInclIf);

            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(24, NodeType::SUCCESSOR) == allAftElse);
            //Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(24, NodeType::PREDECESSOR) == allAftElseInclIf);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(0, NodeType::SUCCESSOR) == EMPTY_RESULT);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(0, NodeType::PREDECESSOR) == EMPTY_RESULT);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(-1, NodeType::SUCCESSOR) == EMPTY_RESULT);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(-1, NodeType::PREDECESSOR) == EMPTY_RESULT);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(25, NodeType::SUCCESSOR) == EMPTY_RESULT);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(25, NodeType::PREDECESSOR) == EMPTY_RESULT);
        }
    };
}
