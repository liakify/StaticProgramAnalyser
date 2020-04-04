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
            18.    if (cond) then {
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

            procedure d {
            25.    while (cond) {
            26.        read;
            27.        while (cond) {
            28.            read;
                       }
            29.        read;
                   }
            }

            procedure e {
            30.    read;
            31.    if (cond) then {
            32.        read;
            33.        if (cond) then {
            34.            read;
                       } else {
            35.            read;
                       }
                   } else {
            36.        if (cond) then {
            37.            read;
                       } else {
            38.            read;
                       }
            39.        read;
                   }
            40.    read;
            }
            */
            pkbNextStar = PKB::PKB();
            pkbNextStar.procTable.insertProc(Procedure("a", 1));
            pkbNextStar.procTable.insertProc(Procedure("b", 2));
            pkbNextStar.procTable.insertProc(Procedure("c", 10));
            pkbNextStar.procTable.insertProc(Procedure("d", 17));
            pkbNextStar.procTable.insertProc(Procedure("e", 20));

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
            StatementList sl17 = StatementList(std::vector<StmtId>{25});
            pkbNextStar.stmtListTable.insertStmtLst(sl17);
            StatementList sl18 = StatementList(std::vector<StmtId>{26, 27, 29});
            pkbNextStar.stmtListTable.insertStmtLst(sl18);
            StatementList sl19 = StatementList(std::vector<StmtId>{28});
            pkbNextStar.stmtListTable.insertStmtLst(sl19);
            StatementList sl20 = StatementList(std::vector<StmtId>{30, 31, 40});
            pkbNextStar.stmtListTable.insertStmtLst(sl20);
            StatementList sl21 = StatementList(std::vector<StmtId>{32, 33});
            pkbNextStar.stmtListTable.insertStmtLst(sl21);
            StatementList sl22 = StatementList(std::vector<StmtId>{34});
            pkbNextStar.stmtListTable.insertStmtLst(sl22);
            StatementList sl23 = StatementList(std::vector<StmtId>{35});
            pkbNextStar.stmtListTable.insertStmtLst(sl23);
            StatementList sl24 = StatementList(std::vector<StmtId>{36, 39});
            pkbNextStar.stmtListTable.insertStmtLst(sl24);
            StatementList sl25 = StatementList(std::vector<StmtId>{37});
            pkbNextStar.stmtListTable.insertStmtLst(sl25);
            StatementList sl26 = StatementList(std::vector<StmtId>{38});
            pkbNextStar.stmtListTable.insertStmtLst(sl26);

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
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 18)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(26)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<WhileStmt>(new WhileStmt(cond, 19)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(28)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(29)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(30)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond, 21, 24)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(32)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond, 22, 23)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(34)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(35)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<IfStmt>(new IfStmt(cond, 25, 26)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(37)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(38)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(39)));
            pkbNextStar.stmtTable.insertStmt(std::shared_ptr<ReadStmt>(new ReadStmt(40)));

            pkbNextStar = DE_nextStar.run(pkbNextStar);
        }

        TEST_METHOD(nextStar) {
            int numStmts = pkbNextStar.stmtTable.size();

            for (int i = 1; i <= numStmts; i++) {
                Assert::IsFalse(pkbNextStar.nextStar(1, i));
                Assert::IsFalse(pkbNextStar.nextStar(i, 1));
            }

            StmtId procBLastStmt = 17;
            StmtId whileLastStmt = 16;

            Assert::IsFalse(pkbNextStar.nextStar(2, 2));
            for (int i = 3; i <= numStmts; i++) {
                if (i <= procBLastStmt) {
                    Assert::IsTrue(pkbNextStar.nextStar(2, i));
                } else {
                    Assert::IsFalse(pkbNextStar.nextStar(2, i));
                }
                Assert::IsFalse(pkbNextStar.nextStar(i, 2));
            }

            for (int j = 3; j <= whileLastStmt; j++) {
                for (int i = j; i <= numStmts; i++) {
                    if (i <= procBLastStmt) {
                        Assert::IsTrue(pkbNextStar.nextStar(j, i));
                        if (i <= whileLastStmt) {
                            Assert::IsTrue(pkbNextStar.nextStar(i, j));
                        } else {
                            Assert::IsFalse(pkbNextStar.nextStar(i, j));
                        }
                    } else {
                        Assert::IsFalse(pkbNextStar.nextStar(j, i));
                        Assert::IsFalse(pkbNextStar.nextStar(i, j));
                    }
                }
            }

            for (int i = 17; i <= numStmts; i++) {
                Assert::IsFalse(pkbNextStar.nextStar(17, i));
                Assert::IsFalse(pkbNextStar.nextStar(i, 17));
            }

            StmtId procCFirstStmt = 18;
            StmtId procCLastStmt = 24;

            Assert::IsFalse(pkbNextStar.nextStar(18, 18));
            for (int i = procCFirstStmt + 1; i <= numStmts; i++) {
                if (i <= procCLastStmt) {
                    Assert::IsTrue(pkbNextStar.nextStar(18, i));
                } else {
                    Assert::IsFalse(pkbNextStar.nextStar(18, i));
                }
                Assert::IsFalse(pkbNextStar.nextStar(i, 18));
            }

            for (int j = 19; j <= 21; j++) {
                for (int i = j; i <= numStmts; i++) {
                    if (i <= 21) {
                        Assert::IsTrue(pkbNextStar.nextStar(j, i));
                        Assert::IsTrue(pkbNextStar.nextStar(i, j));
                    } else {
                        Assert::IsFalse(pkbNextStar.nextStar(j, i));
                        Assert::IsFalse(pkbNextStar.nextStar(i, j));
                    }
                }
            }

            for (int j = 22; j <= 24; j++) {
                for (int i = j; i <= numStmts; i++) {
                    if (i <= 24) {
                        Assert::IsTrue(pkbNextStar.nextStar(j, i));
                        Assert::IsTrue(pkbNextStar.nextStar(i, j));
                    } else {
                        Assert::IsFalse(pkbNextStar.nextStar(j, i));
                        Assert::IsFalse(pkbNextStar.nextStar(i, j));
                    }
                }
            }

            for (int j = 25; j <= 29; j++) {
                for (int i = j; i <= numStmts; i++) {
                    if (i <= 29) {
                        Assert::IsTrue(pkbNextStar.nextStar(j, i));
                        Assert::IsTrue(pkbNextStar.nextStar(i, j));
                    } else {
                        Assert::IsFalse(pkbNextStar.nextStar(j, i));
                        Assert::IsFalse(pkbNextStar.nextStar(i, j));
                    }
                }
            }

            StmtId procEFirstStmt = 30;
            StmtId procELastStmt = 40;

            for (int j = 30; j <= 31; j++) {
                Assert::IsFalse(pkbNextStar.nextStar(j, j));
                for (int i = j + 1; i <= numStmts; i++) {
                    if (i <= procELastStmt) {
                        Assert::IsTrue(pkbNextStar.nextStar(j, i));
                    } else {
                        Assert::IsFalse(pkbNextStar.nextStar(j, i));
                    }
                    Assert::IsFalse(pkbNextStar.nextStar(i, j));
                }
            }

            for (int j = 32; j <= 33; j++) {
                Assert::IsFalse(pkbNextStar.nextStar(j, j));
                for (int i = j + 1; i <= numStmts; i++) {
                    if (i <= 35 || i == 40) {
                        Assert::IsTrue(pkbNextStar.nextStar(j, i));
                    } else {
                        Assert::IsFalse(pkbNextStar.nextStar(j, i));
                    }
                    Assert::IsFalse(pkbNextStar.nextStar(i, j));
                }
            }

            for (int j = 34; j <= 35; j++) {
                Assert::IsFalse(pkbNextStar.nextStar(j, j));
                for (int i = j + 1; i <= numStmts; i++) {
                    if (i == procELastStmt) {
                        Assert::IsTrue(pkbNextStar.nextStar(j, i));
                    } else {
                        Assert::IsFalse(pkbNextStar.nextStar(j, i));
                    }
                    Assert::IsFalse(pkbNextStar.nextStar(i, j));
                }
            }

            Assert::IsFalse(pkbNextStar.nextStar(36, 36));
            for (int i = 37; i <= numStmts; i++) {
                if (i <= procELastStmt) {
                    Assert::IsTrue(pkbNextStar.nextStar(36, i));
                } else {
                    Assert::IsFalse(pkbNextStar.nextStar(36, i));
                }
                Assert::IsFalse(pkbNextStar.nextStar(i, 36));
            }

            for (int j = 37; j <= 38; j++) {
                Assert::IsFalse(pkbNextStar.nextStar(j, j));
                for (int i = j + 1; i <= numStmts; i++) {
                    if (i == procELastStmt || i == procELastStmt - 1) {
                        Assert::IsTrue(pkbNextStar.nextStar(j, i));
                    } else {
                        Assert::IsFalse(pkbNextStar.nextStar(j, i));
                    }
                    Assert::IsFalse(pkbNextStar.nextStar(i, j));
                }
            }

            Assert::IsTrue(pkbNextStar.nextStar(39, 40));
            Assert::IsFalse(pkbNextStar.nextStar(40, 39));

            Assert::IsFalse(pkbNextStar.nextStar(40, 40));

            Assert::IsFalse(pkbNextStar.nextStar(0, 40));
            Assert::IsFalse(pkbNextStar.nextStar(40, -1));
            Assert::IsFalse(pkbNextStar.nextStar(41, -1));
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

            std::unordered_set<StmtId> allInIf { 19, 20, 21, 22, 23, 24 };
            std::unordered_set<StmtId> allAftIf { 19, 20, 21 };
            std::unordered_set<StmtId> allAftIfInclIf { 18, 19, 20, 21 };

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(18, NodeType::SUCCESSOR) == allInIf);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(18, NodeType::PREDECESSOR) == EMPTY_RESULT);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(19, NodeType::SUCCESSOR) == allAftIf);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(19, NodeType::PREDECESSOR) == allAftIfInclIf);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(20, NodeType::SUCCESSOR) == allAftIf);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(20, NodeType::PREDECESSOR) == allAftIfInclIf);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(21, NodeType::SUCCESSOR) == allAftIf);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(21, NodeType::PREDECESSOR) == allAftIfInclIf);

            std::unordered_set<StmtId> allAftElse { 22, 23, 24 };
            std::unordered_set<StmtId> allAftElseInclIf { 18, 22, 23, 24 };

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(22, NodeType::SUCCESSOR) == allAftElse);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(22, NodeType::PREDECESSOR) == allAftElseInclIf);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(23, NodeType::SUCCESSOR) == allAftElse);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(23, NodeType::PREDECESSOR) == allAftElseInclIf);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(24, NodeType::SUCCESSOR) == allAftElse);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(24, NodeType::PREDECESSOR) == allAftElseInclIf);

            std::unordered_set<StmtId> procDWhile = { 25, 26, 27, 28, 29 };

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(25, NodeType::SUCCESSOR) == procDWhile);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(25, NodeType::PREDECESSOR) == procDWhile);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(26, NodeType::SUCCESSOR) == procDWhile);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(26, NodeType::PREDECESSOR) == procDWhile);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(27, NodeType::SUCCESSOR) == procDWhile);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(27, NodeType::PREDECESSOR) == procDWhile);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(28, NodeType::SUCCESSOR) == procDWhile);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(28, NodeType::PREDECESSOR) == procDWhile);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(29, NodeType::SUCCESSOR) == procDWhile);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(29, NodeType::PREDECESSOR) == procDWhile);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(30, NodeType::SUCCESSOR) == std::unordered_set<StmtId> {31, 32, 33, 34, 35, 36, 37, 38, 39, 40});
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(30, NodeType::PREDECESSOR) == EMPTY_RESULT);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(31, NodeType::SUCCESSOR) == std::unordered_set<StmtId> {32, 33, 34, 35, 36, 37, 38, 39, 40});
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(31, NodeType::PREDECESSOR) == std::unordered_set<StmtId> {30});

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(32, NodeType::SUCCESSOR) == std::unordered_set<StmtId> {33, 34, 35, 40});
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(32, NodeType::PREDECESSOR) == std::unordered_set<StmtId> {30, 31});

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(33, NodeType::SUCCESSOR) == std::unordered_set<StmtId> {34, 35, 40});
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(33, NodeType::PREDECESSOR) == std::unordered_set<StmtId> {30, 31, 32});

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(34, NodeType::SUCCESSOR) == std::unordered_set<StmtId> {40});
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(34, NodeType::PREDECESSOR) == std::unordered_set<StmtId> {30, 31, 32, 33});

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(35, NodeType::SUCCESSOR) == std::unordered_set<StmtId> {40});
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(35, NodeType::PREDECESSOR) == std::unordered_set<StmtId> {30, 31, 32, 33});

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(36, NodeType::SUCCESSOR) == std::unordered_set<StmtId> {37, 38, 39, 40});
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(36, NodeType::PREDECESSOR) == std::unordered_set<StmtId> {30, 31});

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(37, NodeType::SUCCESSOR) == std::unordered_set<StmtId> {39, 40});
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(37, NodeType::PREDECESSOR) == std::unordered_set<StmtId> {30, 31, 36});

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(38, NodeType::SUCCESSOR) == std::unordered_set<StmtId> {39, 40});
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(38, NodeType::PREDECESSOR) == std::unordered_set<StmtId> {30, 31, 36});

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(39, NodeType::SUCCESSOR) == std::unordered_set<StmtId> {40});
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(39, NodeType::PREDECESSOR) == std::unordered_set<StmtId> {30, 31, 36, 37, 38});

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(40, NodeType::SUCCESSOR) == EMPTY_RESULT);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(40, NodeType::PREDECESSOR) == std::unordered_set<StmtId> {30, 31, 32, 33, 34, 35, 36, 37, 38, 39});

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(0, NodeType::SUCCESSOR) == EMPTY_RESULT);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(0, NodeType::PREDECESSOR) == EMPTY_RESULT);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(-1, NodeType::SUCCESSOR) == EMPTY_RESULT);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(-1, NodeType::PREDECESSOR) == EMPTY_RESULT);

            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(41, NodeType::SUCCESSOR) == EMPTY_RESULT);
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(41, NodeType::PREDECESSOR) == EMPTY_RESULT);
        }

        TEST_METHOD(caching) {
            /*
                Currently can only be manually verified that previous clauses are cached by stepping into the functions.
                Cache currently only saves relations that are TRUE i.e. can be represented by a bi-directional edge.
            */
            pkbNextStar.clear();
            Assert::IsTrue(pkbNextStar.nextStar(2, 4));
            Assert::IsTrue(pkbNextStar.nextStar(2, 4));

            std::unordered_set<StmtId> whileLoopAndAft{ 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(2, NodeType::SUCCESSOR) == whileLoopAndAft);
            Assert::IsTrue(pkbNextStar.nextStar(2, 3));
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(2, NodeType::SUCCESSOR) == whileLoopAndAft);

            std::unordered_set<StmtId> procDWhile = { 25, 26, 27, 28, 29 };
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(26, NodeType::PREDECESSOR) == procDWhile);
            Assert::IsTrue(pkbNextStar.nextStar(25, 26));
            Assert::IsTrue(pkbNextStar.nextStarGetAllNodes(26, NodeType::PREDECESSOR) == procDWhile);

            Assert::IsFalse(pkbNextStar.nextStar(40, 30));
            Assert::IsFalse(pkbNextStar.nextStar(40, 30));
        }
    };
}
