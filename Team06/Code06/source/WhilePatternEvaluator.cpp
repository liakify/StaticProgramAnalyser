#include <string>
#include <unordered_map>
#include <unordered_set>

#include "WhilePatternEvaluator.h"
#include "LoggingUtils.h"

namespace PQL {
    namespace WhilePatternEvaluator {

        /**
        * Evaluates a single while pattern clause on the given PKB where the inputs is one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateWhilePatternClauseWild(PKB::PKB& database, PatternClause& clause) {
            std::unordered_set<StmtId> stmts = database.patternKB.getAllWhileStmtsWithCtrlVars();

            Synonym arg0 = clause.getSynonym().value;

            ClauseResult clauseResult;
            clauseResult.syns.emplace_back(arg0);
            for (StmtId stmt : stmts) {
                ClauseResultEntry resultEntry;
                resultEntry.emplace_back(std::to_string(stmt));
                clauseResult.rows.emplace_back(resultEntry);
            }

        }

        /**
        * Evaluates a single while pattern clause on the given PKB where the input is one identifier.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateWhilePatternClauseId(PKB::PKB& database, PatternClause &clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {

            Synonym arg0 = clause.getSynonym().value;
            VarId arg1 = database.varTable.getVarId(clause.getArgs().first.value);

            std::unordered_set<StmtId> stmts = database.patternKB.getWhilePatternStmts(arg1);

            ClauseResult clauseResult;
            clauseResult.syns.emplace_back(arg0);
            for (StmtId stmt : stmts) {
                ClauseResultEntry resultEntry;
                resultEntry.emplace_back(std::to_string(stmt));
                clauseResult.rows.emplace_back(resultEntry);
            }

        }

        /**
        * Evaluates a single while pattern clause on the given PKB where the input is one synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateWhilePatternClauseSyn(PKB::PKB& database, PatternClause& clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {

            Synonym arg0 = clause.getSynonym().value;
            Synonym arg1 = clause.getArgs().first.value;

            ClauseResult clauseResult;
            if (arg0 < arg1) {
                clauseResult.syns.emplace_back(arg0);
                clauseResult.syns.emplace_back(arg1);
            } else {
                clauseResult.syns.emplace_back(arg1);
                clauseResult.syns.emplace_back(arg0);
            }

            std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(StmtType::WHILE);
            for (StmtId stmt : stmts) {
                SIMPLE::WhileStmt *whileStmt = dynamic_cast<SIMPLE::WhileStmt*>(database.stmtTable.get(stmt).get());
                std::unordered_set<VarId> vars = whileStmt->getCondExpr().getVarIds();
                for (VarId var : vars) {
                    ClauseResultEntry resultEntry;
                    if (arg0 < arg1) {
                        resultEntry.emplace_back(std::to_string(stmt));
                        resultEntry.emplace_back(database.varTable.get(var));
                    } else {
                        resultEntry.emplace_back(database.varTable.get(var));
                        resultEntry.emplace_back(std::to_string(stmt));
                    }
                    clauseResult.rows.emplace_back(resultEntry);
                }
            }


        }

        void evaluateWhilePatternClause(PKB::PKB& database, PatternClause clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {

            ArgType argType1 = clause.getArgs().first.type;

            if (argType1 == ArgType::WILDCARD) {
                // 1 wildcard
                return evaluateWhilePatternClauseWild(database, clause);
            } else if (argType1 == ArgType::IDENTIFIER) {
                // 1 identifier
                return evaluateWhilePatternClauseId(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM) {
                // 1 synonym
                return evaluateWhilePatternClauseSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("WhilePatternEvaluator::evaluateWhilePatternClause: Invalid ArgTypes for While Pattern clause. argType1 = %d\n", argType1);
                return {};
            }
        }

    }
}