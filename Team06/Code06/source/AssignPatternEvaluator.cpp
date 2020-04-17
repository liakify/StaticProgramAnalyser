#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "AssignPatternEvaluator.h"
#include "LoggingUtils.h"

namespace PQL {
    namespace AssignPatternEvaluator {

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAssignPatternClauseWildWild(PKB::PKB& database, PatternClause &clause) {

            Synonym arg0 = clause.getSynonym().value;

            std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(StmtType::ASSIGN);

            ClauseResult clauseResult;
            clauseResult.syns.emplace_back(arg0);
            for (StmtId stmt : stmts) {
                ClauseResultEntry resultEntry;
                resultEntry.emplace_back(std::to_string(stmt));
                clauseResult.rows.emplace_back(resultEntry);
            }

            return clauseResult;
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one wildcard and one pattern.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAssignPatternClauseWildPtn(PKB::PKB& database, PatternClause &clause) {
            Synonym arg0 = clause.getSynonym().value;
            Pattern arg2 = clause.getArgs().second.value;

            std::unordered_set<StmtId> stmts = database.patternKB.getAssignPatternStmts(arg2);

            ClauseResult clauseResult;
            clauseResult.syns.emplace_back(arg0);
            for (StmtId stmt : stmts) {
                if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(std::to_string(stmt));
                    clauseResult.rows.emplace_back(resultEntry);
                }
            }
            return clauseResult;
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one identifier and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAssignPatternClauseIdWild(PKB::PKB& database, PatternClause& clause) {
            Synonym arg0 = clause.getSynonym().value;
            VarName arg1 = clause.getArgs().first.value;
            VarId arg1Id = database.varTable.getVarId(arg1);

            std::unordered_set<StmtId> stmts = database.modifiesKB.getAllStmtsModifyVar(arg1Id);
            ClauseResult clauseResult;
            clauseResult.syns.emplace_back(arg0);
            for (StmtId stmt : stmts) {
                if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(std::to_string(stmt));
                    clauseResult.rows.emplace_back(resultEntry);
                }
            }

            return clauseResult;
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one identifier and one pattern.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAssignPatternClauseIdPtn(PKB::PKB& database, PatternClause& clause) {
            Synonym arg0 = clause.getSynonym().value;

            std::pair<Argument, Argument> args = clause.getArgs();
            VarName arg1 = args.first.value;
            VarId arg1Id = database.varTable.getVarId(arg1);
            Pattern arg2 = args.second.value;

            std::unordered_set<StmtId> stmts1 = database.modifiesKB.getAllStmtsModifyVar(arg1Id);
            std::unordered_set<StmtId> stmts2 = database.patternKB.getAssignPatternStmts(arg2);

            // Find the intersection
            std::unordered_set<StmtId> stmts;
            for (StmtId stmt : stmts1) {
                if (stmts2.find(stmt) != stmts2.end()) {
                    stmts.insert(stmt);
                }
            }

            ClauseResult clauseResult;
            clauseResult.syns.emplace_back(arg0);
            for (StmtId stmt : stmts) {
                if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(std::to_string(stmt));
                    clauseResult.rows.emplace_back(resultEntry);
                }
            }

            return clauseResult;
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one synonym and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAssignPatternClauseSynWild(PKB::PKB& database, PatternClause& clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg0 = clause.getSynonym().value;
            Synonym arg1 = clause.getArgs().first.value;

            std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(StmtType::ASSIGN);

            // If synonym 'arg1' appeared on LHS, then it must have been modified by the assignment
            ClauseResult clauseResult;
            if (arg0 < arg1) {
                clauseResult.syns.emplace_back(arg0);
                clauseResult.syns.emplace_back(arg1);
            } else {
                clauseResult.syns.emplace_back(arg1);
                clauseResult.syns.emplace_back(arg0);
            }
            for (StmtId stmt : stmts) {
                if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                    std::unordered_set<VarId> vars = database.modifiesKB.getAllVarsModifiedByStmt(stmt);
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

            return clauseResult;
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one synonym and one pattern.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAssignPatternClauseSynPtn(PKB::PKB& database, PatternClause& clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {
            Synonym arg0 = clause.getSynonym().value;

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            Pattern arg2 = args.second.value;

            std::unordered_set<StmtId> stmts1 = database.stmtTable.getStmtsByType(StmtType::ASSIGN);
            std::unordered_set<StmtId> stmts2 = database.patternKB.getAssignPatternStmts(arg2);

            // Find the intersection
            std::unordered_set<StmtId> stmts;
            for (StmtId stmt : stmts1) {
                if (stmts2.find(stmt) != stmts2.end()) {
                    stmts.insert(stmt);
                }
            }

            ClauseResult clauseResult;
            if (arg0 < arg1) {
                clauseResult.syns.emplace_back(arg0);
                clauseResult.syns.emplace_back(arg1);
            } else {
                clauseResult.syns.emplace_back(arg1);
                clauseResult.syns.emplace_back(arg0);
            }
            for (StmtId stmt : stmts) {
                if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                    std::unordered_set<VarId> vars = database.modifiesKB.getAllVarsModifiedByStmt(stmt);
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

            return clauseResult;
        }

        void evaluateAssignPatternClause(PKB::PKB& database, PatternClause clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // 2 wildcards
                return evaluateAssignPatternClauseWildWild(database, clause);
            } else if (argType1 == ArgType::WILDCARD &&
                (argType2 == ArgType::EXACT_PATTERN || argType2 == ArgType::INCLUSIVE_PATTERN)) {
                // 1 wildcard, 1 pattern
                return evaluateAssignPatternClauseWildPtn(database, clause);
            } else if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::WILDCARD) {
                // 1 identifier, 1 wildcard
                return evaluateAssignPatternClauseIdWild(database, clause);
            } else if (argType1 == ArgType::IDENTIFIER &&
                (argType2 == ArgType::EXACT_PATTERN || argType2 == ArgType::INCLUSIVE_PATTERN)) {
                // 1 identifier, 1 pattern
                return evaluateAssignPatternClauseIdPtn(database, clause);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // 1 synonym, 1 wildcard
                return evaluateAssignPatternClauseSynWild(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM &&
                (argType2 == ArgType::EXACT_PATTERN || argType2 == ArgType::INCLUSIVE_PATTERN)) {
                // 1 synonym, 1 pattern
                return evaluateAssignPatternClauseSynPtn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("AssignPatternEvaluator::evaluateAssignPatternClause: Invalid ArgTypes for Assign Pattern clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}