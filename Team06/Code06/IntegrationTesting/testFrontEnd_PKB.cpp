#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"
#include "FrontEnd.h"
#include "Types.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {

    PKB::PKB pkb;

    TEST_CLASS(TestFrontEnd_PKB) {
    public:

        std::unordered_set<VarName> allVars = std::unordered_set<VarName>({ "x", "y", "z", "a" });

        std::unordered_set<VarName> allModifiedVars = std::unordered_set<VarName>({ "x", "y", "z" });

        std::unordered_set<StmtId> allModifiedStmts = std::unordered_set<StmtId>({ 1, 2, 4 });

        std::unordered_set<VarName> allUsedVars = std::unordered_set<VarName>({ "x", "y", "a" });

        std::unordered_set<StmtId> allUsedStmts = std::unordered_set<StmtId>({ 4, 5, 6});

        std::unordered_set<ConstValue> allConstants = { "7", "2" };

        std::string procedureName = "p";

        int stmtListCount = 4;
        int stmtCount_ASSIGN = 3;
        int stmtCount_READ = 1;
        int stmtCount_PRINT = 1;
        int stmtCount_CALL = 0;
        int stmtCount_IF = 1;
        int stmtCount_WHILE = 1;

        TEST_CLASS_INITIALIZE(setup) {
            FrontEnd::FrontEndManager frontEnd = FrontEnd::FrontEndManager();
            pkb = PKB::PKB();
            std::string src = "procedure p{x=7; y=2; if(x>y) then {z=x+a;} else {print y;} while (y>a) {read y;} }";
            pkb = frontEnd.parseSimple(src);
        }

        TEST_METHOD(parseSimple_Invalid) {
            wchar_t* errorMessage = L"Parsing invalid SIMPLE source.";
            auto lambda = [] { 
                FrontEnd::FrontEndManager frontEnd = FrontEnd::FrontEndManager();
                PKB::PKB pkbTemp = PKB::PKB();
                std::string src = "INVALID SIMPLE SOURCE";
                pkbTemp = frontEnd.parseSimple(src); };
            Assert::ExpectException<std::invalid_argument>(lambda, errorMessage);
        }

        TEST_METHOD(parseSimple) {
            Assert::IsTrue(pkb.varTable.getAllVars() == allVars);

            Assert::IsTrue(pkb.constTable.size() == allConstants.size());
            std::unordered_set<ConstValue> constants;
            for (auto itr = allConstants.begin(); itr != allConstants.end(); ++itr) {
                try {
                    constants.insert(pkb.constTable.get(pkb.constTable.getConstId(*itr)));
                }
                catch (std::exception&) {
                    Assert::IsTrue(false);
                }
            }
            Assert::IsTrue(constants == allConstants);

            Assert::IsTrue(pkb.procTable.size() == 1);
            Assert::IsTrue(pkb.procTable.getProcId(procedureName) == 1);

            Assert::IsTrue(pkb.stmtListTable.size() == stmtListCount);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::ASSIGN).size() == stmtCount_ASSIGN);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::READ).size() == stmtCount_READ);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::PRINT).size() == stmtCount_PRINT);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::CALL).size() == stmtCount_CALL);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::IF).size() == stmtCount_IF);
            Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::WHILE).size() == stmtCount_WHILE);

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

            std::unordered_set<VarId> modifiedVarIds = pkb.modifiesKB.getAllVarsModifiedByProc(pkb.procTable.getProcId(procedureName));
            std::unordered_set<VarName> modifiedVarNames;
            for (auto itr = modifiedVarIds.begin(); itr != modifiedVarIds.end(); itr++) {
                modifiedVarNames.insert(pkb.varTable.get(*itr));
            }
            Assert::IsTrue(modifiedVarNames == allModifiedVars);

            std::unordered_set<VarId> usedVarIds = pkb.usesKB.getAllVarsUsedByProc(pkb.procTable.getProcId(procedureName));
            std::unordered_set<VarName> usedVarNames;
            for (auto itr = usedVarIds.begin(); itr != usedVarIds.end(); itr++) {
                usedVarNames.insert(pkb.varTable.get(*itr));
            }
            Assert::IsTrue(usedVarNames == allUsedVars);

            Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("7") == std::unordered_set<StmtId>({1}));
            Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("2") == std::unordered_set<StmtId>({2}));
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
