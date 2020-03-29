#include <algorithm>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "PQL.h"
#include "QueryParserStub.h"

using std::pair;
using std::string;
using std::unordered_set;
using std::vector;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

    PQL::QueryParser parserStub = QueryParserStub();
    PQL::QueryEvaluator evalStub = PQL::QueryEvaluator(PKB::PKB());

    PQL::PQLManager facade = PQL::PQLManager(parserStub, evalStub);

    TEST_CLASS(TestPQL) {
    public:
        TEST_CLASS_INITIALIZE(initPQL) {

            PKB::PKB dummyPKB = PKB::PKB();

            /** Sample SIMPLE program to ensure valid queries return valid results:
              *
              * procedure f {
              *     x = 0;
              *     y = x + 1;
              *     z = x - y;
              * }
              *
              */

            // Populate all entity tables (constants, variable, statements, statement lists)
            ConstId zeroId = dummyPKB.constTable.insertConst("0");
            ConstId oneId = dummyPKB.constTable.insertConst("1");
            VarId xId = dummyPKB.varTable.insertVar("x");
            VarId yId = dummyPKB.varTable.insertVar("y");
            VarId zId = dummyPKB.varTable.insertVar("z");

            Expression zeroConst = Expression("0", zeroId, ExprType::CONST);
            Expression oneConst = Expression("1", oneId, ExprType::CONST);
            Expression xVar = Expression("x", xId, ExprType::VAR);
            Expression yVar = Expression("y", yId, ExprType::VAR);
            Expression zVar = Expression("z", zId, ExprType::VAR);

            Expression yRHS = Expression(xVar, oneConst, '+');
            Expression zRHS = Expression(xVar, yVar, '-');

            StmtId stmt1 = dummyPKB.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(xId, zeroConst)));
            StmtId stmt2 = dummyPKB.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(yId, yRHS)));
            StmtId stmt3 = dummyPKB.stmtTable.insertStmt(std::shared_ptr<AssignStmt>(new AssignStmt(zId, zRHS)));

            vector<StmtId> stmts = { stmt1, stmt2, stmt3 };
            StmtListId procFStmts = dummyPKB.stmtListTable.insertStmtLst(stmts);

            ProcId procFId = dummyPKB.procTable.insertProc(Procedure("f", procFStmts));

            // Populate all relation and pattern tables
            dummyPKB.followsKB.addFollows(stmt1, stmt2);
            dummyPKB.followsKB.addFollows(stmt2, stmt3);
            dummyPKB.followsKB.setAllFollowers(stmt1, { stmt2, stmt3 });
            dummyPKB.followsKB.setAllFollowers(stmt2, { stmt3 });
            dummyPKB.followsKB.setAllFollowing(stmt2, { stmt1 });
            dummyPKB.followsKB.setAllFollowing(stmt3, { stmt1, stmt2 });

            dummyPKB.usesKB.addStmtUses(stmt2, xId);
            dummyPKB.usesKB.addStmtUses(stmt3, xId);
            dummyPKB.usesKB.addStmtUses(stmt3, yId);
            dummyPKB.usesKB.addProcUses(procFId, xId);
            dummyPKB.usesKB.addProcUses(procFId, yId);

            dummyPKB.modifiesKB.addStmtModifies(stmt1, xId);
            dummyPKB.modifiesKB.addStmtModifies(stmt2, yId);
            dummyPKB.modifiesKB.addStmtModifies(stmt3, zId);
            dummyPKB.modifiesKB.addProcModifies(procFId, xId);
            dummyPKB.modifiesKB.addProcModifies(procFId, yId);
            dummyPKB.modifiesKB.addProcModifies(procFId, zId);

            dummyPKB.addNext(stmt1, stmt2);
            dummyPKB.addNext(stmt2, stmt3);

            dummyPKB.patternKB.addAssignPattern("0", stmt1);
            dummyPKB.patternKB.addAssignPattern("_0_", stmt1);
            dummyPKB.patternKB.addAssignPattern(yRHS.getStr(), stmt2);
            dummyPKB.patternKB.addAssignPattern("_x_", stmt2);
            dummyPKB.patternKB.addAssignPattern("_1_", stmt2);
            dummyPKB.patternKB.addAssignPattern(zRHS.getStr(), stmt3);
            dummyPKB.patternKB.addAssignPattern("_x_", stmt3);
            dummyPKB.patternKB.addAssignPattern("_y_", stmt3);

            evalStub = PQL::QueryEvaluator(dummyPKB);
            facade = PQL::PQLManager(parserStub, evalStub);
        }

        TEST_METHOD(evaluateQuery) {
            // List of test queries and corresponding expected results
            // After each query is handled by PQLManager::evaluateQuery
            vector<pair<string, std::list<string>>> TEST_CASES = {
                { StubQueries::EMPTY_QUERY, { } },
                { StubQueries::BOOLEAN_VALID_QUERY, { "TRUE" } },
                { StubQueries::BOOLEAN_INVALID_DECL_SYNTAX_QUERY, { } },
                { StubQueries::BOOLEAN_UNKNOWN_DECL_KEYWORD_QUERY, { } },
                { StubQueries::BOOLEAN_CONFLICTING_DECLARATION_QUERY, { "FALSE" } },
                { StubQueries::TUPLE_VALID_QUERY, { "1 2", "1 3", "2 3" } },
                { StubQueries::TUPLE_INVALID_DECL_SYNTAX_QUERY, { } },
                { StubQueries::TUPLE_UNKNOWN_DECL_KEYWORD_QUERY, { } },
                { StubQueries::TUPLE_CONFLICTING_DECLARATION_QUERY, { } },
                { StubQueries::INVALID_RETURN_TYPE_QUERY, { } }
            };

            for (auto testcase : TEST_CASES) {
                // Compare the contents of the result list and the expected result list
                std::list<string> output;
                facade.evaluateQuery(testcase.first, output);
                Assert::IsTrue(testcase.second.size() == output.size());
                Assert::IsTrue(std::equal(testcase.second.begin(), testcase.second.end(), output.begin()));
            }
        }

    };
}
