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

        std::unordered_set<VarName> allModifiedVars = std::unordered_set<VarName>({"x", "y", "z"});
        std::unordered_map<VarName, std::unordered_set<StmtId>> allModifiedVarWithStmt =
        {
            std::make_pair("x", std::unordered_set<StmtId>({1})),
            std::make_pair("y", std::unordered_set<StmtId>({2, 6, 7})),
            std::make_pair("z", std::unordered_set<StmtId>({3, 4}))
        };

        std::unordered_set<VarName> allUsedVars = std::unordered_set<VarName>({"x", "y", "a"});
        std::unordered_map<VarName, std::unordered_set<StmtId>> allUsedVarWithStmt =
        {
            std::make_pair("x", std::unordered_set<StmtId>({3, 4})),
            std::make_pair("y", std::unordered_set<StmtId>({3, 5, 6})),
            std::make_pair("a", std::unordered_set<StmtId>({3, 4, 6}))
        };

        std::unordered_set<ConstValue> allConstants = {"7", "2"};

        std::string procedureName = "p";

        std::vector<std::vector<StmtId>> stmtLists =
        {
            std::vector<StmtId>({4}),
            std::vector<StmtId>({5}),
            std::vector<StmtId>({7}),
            std::vector<StmtId>({1, 2, 3, 6})
        };
        std::unordered_set<StmtId> stmts_ASSIGN = std::unordered_set<StmtId>({1, 2, 4});
        std::unordered_set<StmtId> stmts_READ = std::unordered_set<StmtId>({7});
        std::unordered_set<StmtId> stmts_PRINT = std::unordered_set<StmtId>({5});
        std::unordered_set<StmtId> stmts_CALL = std::unordered_set<StmtId>({});
        std::unordered_set<StmtId> stmts_IF = std::unordered_set<StmtId>({3});
        std::unordered_set<StmtId> stmts_WHILE = std::unordered_set<StmtId>({6});

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
            for (int i = 1; i <= pkb.constTable.size(); ++i) {
                constants.insert(pkb.constTable.get(i));
            }
            Assert::IsTrue(constants == allConstants);

            Assert::IsTrue(pkb.procTable.size() == 1);
            Assert::IsTrue(pkb.procTable.getProcId(procedureName) == 1);

            Assert::IsTrue(pkb.stmtListTable.size() == stmtLists.size());
            for (int i = 1; i <= pkb.stmtListTable.size(); ++i) {
                Assert::IsTrue(pkb.stmtListTable.get(i) == stmtLists[i - 1]);
            }

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

            std::unordered_set<VarId> modifiedVarIds = pkb.modifiesKB.getAllVarsModifiedByProc(pkb.procTable.getProcId(procedureName));
            std::unordered_set<VarName> modifiedVarNames;
            for (auto itr = modifiedVarIds.begin(); itr != modifiedVarIds.end(); itr++) {
                modifiedVarNames.insert(pkb.varTable.get(*itr));
            }
            Assert::IsTrue(modifiedVarNames == allModifiedVars);
            Assert::IsTrue(pkb.modifiesKB.getAllStmtsModifyVar(pkb.varTable.getVarId("x")) == allModifiedVarWithStmt.at("x"));
            Assert::IsTrue(pkb.modifiesKB.getAllStmtsModifyVar(pkb.varTable.getVarId("y")) == allModifiedVarWithStmt.at("y"));
            Assert::IsTrue(pkb.modifiesKB.getAllStmtsModifyVar(pkb.varTable.getVarId("z")) == allModifiedVarWithStmt.at("z"));


            std::unordered_set<VarId> usedVarIds = pkb.usesKB.getAllVarsUsedByProc(pkb.procTable.getProcId(procedureName));
            std::unordered_set<VarName> usedVarNames;
            for (auto itr = usedVarIds.begin(); itr != usedVarIds.end(); itr++) {
                usedVarNames.insert(pkb.varTable.get(*itr));
            }
            Assert::IsTrue(usedVarNames == allUsedVars);
            Assert::IsTrue(pkb.usesKB.getAllStmtsUsingVar(pkb.varTable.getVarId("x")) == allUsedVarWithStmt.at("x"));
            Assert::IsTrue(pkb.usesKB.getAllStmtsUsingVar(pkb.varTable.getVarId("y")) == allUsedVarWithStmt.at("y"));
            Assert::IsTrue(pkb.usesKB.getAllStmtsUsingVar(pkb.varTable.getVarId("a")) == allUsedVarWithStmt.at("a"));

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
