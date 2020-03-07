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

    std::unordered_set<StmtId> allUsedStmts = std::unordered_set<StmtId>({ 4, 5 });

    std::unordered_set<ConstValue> allConstants = { "7", "2" };

    std::string procedureName = "p";

    int stmtListCount = 3;
    int stmtCount_ASSIGN = 3;
    int stmtCount_READ = 1;
    int stmtCount_PRINT = 1;
    int stmtCount_CALL = 0;
    int stmtCount_IF = 1;
    int stmtCount_WHILE = 0;

    TEST_CLASS_INITIALIZE(setup) {
        try {
            FrontEnd::FrontEndManager frontEnd = FrontEnd::FrontEndManager();
            pkb = PKB::PKB();
            std::string src = "procedure p{x=7; y=2; if(x>y) then {z=x+a;} else {print y;} read y; }";
            pkb = frontEnd.parseSimple(src);
        }
        catch (std::invalid_argument&) {
            Assert::IsTrue(false);
        }
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
        Assert::IsFalse(pkb.procTable.getProcId(procedureName) == -1);

        Assert::IsTrue(pkb.stmtListTable.size() == stmtListCount);
        Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::ASSIGN).size() == stmtCount_ASSIGN);
        Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::READ).size() == stmtCount_READ);
        Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::PRINT).size() == stmtCount_PRINT);
        Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::CALL).size() == stmtCount_CALL);
        Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::IF).size() == stmtCount_IF);
        Assert::IsTrue(pkb.stmtTable.getStmtsByType(StmtType::WHILE).size() == stmtCount_WHILE);

        Assert::IsTrue(pkb.followsKB.follows(1, 2));
        Assert::IsTrue(pkb.followsKB.follows(2, 3));
        Assert::IsTrue(pkb.followsKB.follows(3, 6));
        Assert::IsTrue(pkb.followsKB.followStar(1, 2));
        Assert::IsTrue(pkb.followsKB.followStar(1, 3));
        Assert::IsTrue(pkb.followsKB.followStar(1, 6));
        Assert::IsTrue(pkb.followsKB.followStar(2, 3));
        Assert::IsTrue(pkb.followsKB.followStar(2, 6));
        Assert::IsTrue(pkb.followsKB.followStar(3, 6));

        Assert::IsFalse(pkb.followsKB.follows(3, 4));
        Assert::IsFalse(pkb.followsKB.follows(4, 5));
        Assert::IsFalse(pkb.followsKB.follows(5, 6));
        Assert::IsFalse(pkb.followsKB.followStar(1, 4));
        Assert::IsFalse(pkb.followsKB.followStar(1, 5));
        Assert::IsFalse(pkb.followsKB.followStar(2, 4));
        Assert::IsFalse(pkb.followsKB.followStar(2, 5));
        Assert::IsFalse(pkb.followsKB.followStar(3, 4));
        Assert::IsFalse(pkb.followsKB.followStar(3, 5));
        Assert::IsFalse(pkb.followsKB.followStar(4, 5));
        Assert::IsFalse(pkb.followsKB.followStar(4, 6));
        Assert::IsFalse(pkb.followsKB.followStar(5, 6));

        Assert::IsTrue(pkb.parentKB.parent(3, 4));
        Assert::IsTrue(pkb.parentKB.parent(3, 5));
        Assert::IsTrue(pkb.parentKB.parentStar(3, 4));
        Assert::IsTrue(pkb.parentKB.parentStar(3, 5));

        Assert::IsFalse(pkb.parentKB.parent(1, 2));
        Assert::IsFalse(pkb.parentKB.parent(2, 3));
        Assert::IsFalse(pkb.parentKB.parent(5, 6));
        Assert::IsFalse(pkb.parentKB.parentStar(1, 2));
        Assert::IsFalse(pkb.parentKB.parentStar(1, 3));
        Assert::IsFalse(pkb.parentKB.parentStar(1, 4));
        Assert::IsFalse(pkb.parentKB.parentStar(1, 5));
        Assert::IsFalse(pkb.parentKB.parentStar(1, 6));
        Assert::IsFalse(pkb.parentKB.parentStar(2, 3));
        Assert::IsFalse(pkb.parentKB.parentStar(2, 4));
        Assert::IsFalse(pkb.parentKB.parentStar(2, 5));
        Assert::IsFalse(pkb.parentKB.parentStar(2, 6));
        Assert::IsFalse(pkb.parentKB.parentStar(3, 6));
        Assert::IsFalse(pkb.parentKB.parentStar(4, 5));
        Assert::IsFalse(pkb.parentKB.parentStar(4, 6));
        Assert::IsFalse(pkb.parentKB.parentStar(5, 6));


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

        Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("x") == std::unordered_set<StmtId>({}));
        Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("y") == std::unordered_set<StmtId>({}));
        Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("z") == std::unordered_set<StmtId>({}));
        Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("7") == std::unordered_set<StmtId>({ 1 }));
        Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("2") == std::unordered_set<StmtId>({ 2 }));
        Assert::IsTrue(pkb.patternKB.getAssignPatternStmts("x+a") == std::unordered_set<StmtId>({}));
        Assert::IsTrue(pkb.patternKB.getIfPatternStmts(pkb.varTable.getVarId("x")).size() == 0);
        Assert::IsTrue(pkb.patternKB.getIfPatternStmts(pkb.varTable.getVarId("y")).size() == 0);
    }
    };
}
