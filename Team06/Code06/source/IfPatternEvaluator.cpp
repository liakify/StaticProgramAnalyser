#include <string>
#include <unordered_map>
#include <unordered_set>

#include "IfPatternEvaluator.h"
#include "LoggingUtils.h"

namespace PQL {
    namespace IfPatternEvaluator {

        /**
        * Evaluates a single if pattern clause on the given PKB where the inputs is one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateIfPatternClauseWild(PKB::PKB& database) {
            // Here, we just need to check if there is at least one if statement in the program
            if (database.stmtTable.getStmtsByType(StmtType::IF).size() > 0) {
                ClauseResultEntry resultEntry;
                resultEntry["_RESULT"] = "TRUE";
                return { resultEntry };
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single if pattern clause on the given PKB where the input is one identifier.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateIfPatternClauseId(PKB::PKB& database, PatternClause& clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {

            Synonym arg0 = clause.synonym;
            VarId arg1 = database.varTable.getVarId(clause.getArgs().first.second);

            std::unordered_set<StmtId> stmts = database.patternKB.getIfPatternStmts(arg1);

            ClauseResult clauseResult;
            for (StmtId stmt : stmts) {
                ClauseResultEntry resultEntry;
                resultEntry[arg0] = std::to_string(stmt);
                clauseResult.emplace_back(resultEntry);
            }
            return clauseResult;
        }

        /**
        * Evaluates a single if pattern clause on the given PKB where the input is one synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateIfPatternClauseSyn(PKB::PKB& database, PatternClause& clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {

            Synonym arg0 = clause.synonym;
            Synonym arg1 = clause.getArgs().first.second;

            ClauseResult clauseResult;

            std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(StmtType::IF);
            for (StmtId stmt : stmts) {
                SIMPLE::IfStmt* ifStmt = (SIMPLE::IfStmt*)database.stmtTable.get(stmt);
                std::unordered_set<VarId> vars = ifStmt->getCondExpr().getVarIds();
                for (VarId var : vars) {
                    ClauseResultEntry resultEntry;
                    resultEntry[arg0] = std::to_string(stmt);
                    resultEntry[arg1] = database.varTable.get(var);
                    clauseResult.emplace_back(resultEntry);
                }
            }

            return clauseResult;
        }

        ClauseResult evaluateIfPatternClause(PKB::PKB& database, PatternClause clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {

            ArgType argType1 = clause.getArgs().first.first;

            if (argType1 == ArgType::WILDCARD) {
                // 1 wildcard
                return evaluateIfPatternClauseWild(database);
            } else if (argType1 == ArgType::IDENTIFIER) {
                // 1 identifier
                return evaluateIfPatternClauseId(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM) {
                // 1 synonym
                return evaluateIfPatternClauseSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("IfPatternEvaluator::evaluateIfPatternClause: Invalid ArgTypes for If Pattern clause. argType1 = %d\n", argType1);
                return {};
            }
        }

    }
}