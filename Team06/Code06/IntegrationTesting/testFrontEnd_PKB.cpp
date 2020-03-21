#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"
#include "FrontEnd.h"
#include "Types.h"
#include "Simple.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {

    PKB::PKB pkb;

    TEST_CLASS(TestFrontEnd_PKB) {
    public:

        std::unordered_set<VarName> allVars = std::unordered_set<VarName>({"x", "y", "z", "a"});

        std::unordered_set<VarName> allModifiedVars_p = std::unordered_set<VarName>({"x", "y", "z"});
        std::unordered_set<VarName> allModifiedVars_p2 = std::unordered_set<VarName>({"y"});
        std::unordered_map<VarName, std::unordered_set<StmtId>> allModifiedVarWithStmt =
        {
            std::make_pair("x", std::unordered_set<StmtId>({1})),
            std::make_pair("y", std::unordered_set<StmtId>({2, 6, 7, 8})),
            std::make_pair("z", std::unordered_set<StmtId>({3, 4}))
        };

        std::unordered_set<VarName> allUsedVars_p = std::unordered_set<VarName>({"x", "y", "a"});
        std::unordered_set<VarName> allUsedVars_p2 = std::unordered_set<VarName>({});
        std::unordered_map<VarName, std::unordered_set<StmtId>> allUsedVarWithStmt =
        {
            std::make_pair("x", std::unordered_set<StmtId>({3, 4})),
            std::make_pair("y", std::unordered_set<StmtId>({3, 5, 6})),
            std::make_pair("a", std::unordered_set<StmtId>({3, 4, 6}))
        };

        std::unordered_set<ConstValue> allConstants = {"7", "2"};

        int procCount = 2;
        std::string procName_p = "p";
        std::string procName_p2 = "p2";

        std::vector<std::vector<StmtId>> stmtLists =
        {
            std::vector<StmtId>({4}),
            std::vector<StmtId>({5}),
            std::vector<StmtId>({7}),
            std::vector<StmtId>({1, 2, 3, 6}),
            std::vector<StmtId>({8})
        };

        int stmts_ALL = 8;
        std::unordered_set<StmtId> stmts_ASSIGN = std::unordered_set<StmtId>({1, 4, 8});
        std::unordered_set<StmtId> stmts_READ = std::unordered_set<StmtId>({7});
        std::unordered_set<StmtId> stmts_PRINT = std::unordered_set<StmtId>({5});
        std::unordered_set<StmtId> stmts_CALL = std::unordered_set<StmtId>({2});
        std::unordered_set<StmtId> stmts_IF = std::unordered_set<StmtId>({3});
        std::unordered_set<StmtId> stmts_WHILE = std::unordered_set<StmtId>({6});

        TEST_CLASS_INITIALIZE(setup) {
            FrontEnd::FrontEndManager frontEnd = FrontEnd::FrontEndManager();
            pkb = PKB::PKB();
            std::string src = "procedure p{x=7; call p2; if(x>y) then {z=x+a;} else {print y;} while (y>a) {read y;} } procedure p2{ y=2; }";
            pkb = frontEnd.parseSimple(src);
        }

        TEST_METHOD(parseSimple) {
            Assert::IsTrue(pkb.varTable.getAllVars() == allVars);

            Assert::IsTrue(pkb.constTable.size() == allConstants.size());
            std::unordered_set<ConstValue> constants;
            for (int i = 1; i <= pkb.constTable.size(); ++i) {
                constants.insert(pkb.constTable.get(i));
            }
            Assert::IsTrue(constants == allConstants);

            Assert::IsTrue(pkb.procTable.size() == procCount);
            Assert::IsTrue(pkb.procTable.getProcId(procName_p) == 1);
            Assert::IsTrue(pkb.procTable.getProcId(procName_p2) == 2);

            Assert::IsTrue(pkb.stmtListTable.size() == stmtLists.size());
            for (int i = 1; i <= pkb.stmtListTable.size(); ++i) {
                Assert::IsTrue(pkb.stmtListTable.get(i) == stmtLists[i - 1]);
            }

            Assert::IsTrue(pkb.stmtTable.size() == stmts_ALL);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::ASSIGN) == stmts_ASSIGN);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::READ) == stmts_READ);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::PRINT) == stmts_PRINT);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::CALL) == stmts_CALL);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::IF) == stmts_IF);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::WHILE) == stmts_WHILE);

            Assert::IsTrue(pkb.followsKB.getFollower(1) == 2);
            Assert::IsTrue(pkb.followsKB.getFollower(2) == 3);
            Assert::IsTrue(pkb.followsKB.getFollower(3) == 6);
            Assert::IsTrue(pkb.followsKB.getFollower(4) == 0);
            Assert::IsTrue(pkb.followsKB.getFollower(5) == 0);
            Assert::IsTrue(pkb.followsKB.getFollower(6) == 0);
            Assert::IsTrue(pkb.followsKB.getFollower(7) == 0);
            Assert::IsTrue(pkb.followsKB.getAllFollowers(1) == std::unordered_set<StmtId>({2, 3, 6}));
            Assert::IsTrue(pkb.followsKB.getAllFollowers(2) == std::unordered_set<StmtId>({3, 6}));
            Assert::IsTrue(pkb.followsKB.getAllFollowers(3) == std::unordered_set<StmtId>({6}));
            Assert::IsTrue(pkb.followsKB.getAllFollowers(4) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.followsKB.getAllFollowers(5) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.followsKB.getAllFollowers(6) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.followsKB.getAllFollowers(7) == std::unordered_set<StmtId>({}));

            Assert::IsTrue(pkb.parentKB.getDirectChildren(1) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.parentKB.getDirectChildren(2) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.parentKB.getDirectChildren(3) == std::unordered_set<StmtId>({4, 5}));
            Assert::IsTrue(pkb.parentKB.getDirectChildren(4) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.parentKB.getDirectChildren(5) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.parentKB.getDirectChildren(6) == std::unordered_set<StmtId>({7}));
            Assert::IsTrue(pkb.parentKB.getDirectChildren(7) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.parentKB.getAllChildren(1) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.parentKB.getAllChildren(2) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.parentKB.getAllChildren(3) == std::unordered_set<StmtId>({4, 5}));
            Assert::IsTrue(pkb.parentKB.getAllChildren(4) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.parentKB.getAllChildren(5) == std::unordered_set<StmtId>({}));
            Assert::IsTrue(pkb.parentKB.getAllChildren(6) == std::unordered_set<StmtId>({7}));
            Assert::IsTrue(pkb.parentKB.getAllChildren(7) == std::unordered_set<StmtId>({}));

            std::unordered_set<VarName> modifiedVarNames;
            std::unordered_set<VarId> modifiedVarIds = pkb.modifiesKB.getAllVarsModifiedByProc(pkb.procTable.getProcId(procName_p));
            for (auto itr = modifiedVarIds.begin(); itr != modifiedVarIds.end(); itr++) {
                modifiedVarNames.insert(pkb.varTable.get(*itr));
            }
            Assert::IsTrue(modifiedVarNames == allModifiedVars_p);
            modifiedVarNames.clear();
            modifiedVarIds = pkb.modifiesKB.getAllVarsModifiedByProc(pkb.procTable.getProcId(procName_p2));
            for (auto itr = modifiedVarIds.begin(); itr != modifiedVarIds.end(); itr++) {
                modifiedVarNames.insert(pkb.varTable.get(*itr));
            }
            Assert::IsTrue(modifiedVarNames == allModifiedVars_p2);
            Assert::IsTrue(pkb.modifiesKB.getAllStmtsModifyVar(pkb.varTable.getVarId("x")) == allModifiedVarWithStmt.at("x"));
            Assert::IsTrue(pkb.modifiesKB.getAllStmtsModifyVar(pkb.varTable.getVarId("y")) == allModifiedVarWithStmt.at("y"));
            Assert::IsTrue(pkb.modifiesKB.getAllStmtsModifyVar(pkb.varTable.getVarId("z")) == allModifiedVarWithStmt.at("z"));

            std::unordered_set<VarName> usedVarNames;
            std::unordered_set<VarId> usedVarIds = pkb.usesKB.getAllVarsUsedByProc(pkb.procTable.getProcId(procName_p));
            for (auto itr = usedVarIds.begin(); itr != usedVarIds.end(); itr++) {
                usedVarNames.insert(pkb.varTable.get(*itr));
            }
            Assert::IsTrue(usedVarNames == allUsedVars_p);
            usedVarNames.clear();
            usedVarIds = pkb.usesKB.getAllVarsUsedByProc(pkb.procTable.getProcId(procName_p2));
            for (auto itr = usedVarIds.begin(); itr != usedVarIds.end(); itr++) {
                usedVarNames.insert(pkb.varTable.get(*itr));
            }
            Assert::IsTrue(usedVarNames == allUsedVars_p2);
            Assert::IsTrue(pkb.usesKB.getAllStmtsUsingVar(pkb.varTable.getVarId("x")) == allUsedVarWithStmt.at("x"));
            Assert::IsTrue(pkb.usesKB.getAllStmtsUsingVar(pkb.varTable.getVarId("y")) == allUsedVarWithStmt.at("y"));
            Assert::IsTrue(pkb.usesKB.getAllStmtsUsingVar(pkb.varTable.getVarId("a")) == allUsedVarWithStmt.at("a"));

            Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("7") == std::unordered_set<StmtId>({1}));
            Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("2") == std::unordered_set<StmtId>({8}));
            Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("_x_") == std::unordered_set<StmtId>({4}));
            Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("_a_") == std::unordered_set<StmtId>({4}));
            Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("(x+a)") == std::unordered_set<StmtId>({4}));
            Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("_(x+a)_") == std::unordered_set<StmtId>({4}));
            Assert::IsTrue(pkb.patternKB.getIfPatternStmts(pkb.varTable.getVarId("x")) == std::unordered_set<StmtId>({3}));
            Assert::IsTrue(pkb.patternKB.getIfPatternStmts(pkb.varTable.getVarId("y")) == std::unordered_set<StmtId>({3}));
            Assert::IsTrue(pkb.patternKB.getWhilePatternStmts(pkb.varTable.getVarId("a")) == std::unordered_set<StmtId>({6}));
            Assert::IsTrue(pkb.patternKB.getWhilePatternStmts(pkb.varTable.getVarId("y")) == std::unordered_set<StmtId>({6}));
        }
    };
}
