#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "UsesEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace UsesEvaluator {

        /**
        * Evaluates a single Uses clause on the given PKB where the inputs one StmtId and one identifier.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateUsesClauseIntIdId(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;

            if (argType1 == ArgType::INTEGER) {
                // Case 1: Statement number provided
                StmtId arg1 = std::stoi(args.first.value);
                VarId arg2 = database.varTable.getVarId(args.second.value);

                ClauseResult clauseResult;
                if (database.usesKB.stmtUses(arg1, arg2)) {
                    clauseResult.trueResult = true;
                }
    
            } else {
                // Case 2: Procedure name provided
                ProcId arg1 = database.procTable.getProcId(args.first.value);
                VarId arg2 = database.varTable.getVarId(args.second.value);

                ClauseResult clauseResult;
                if (database.usesKB.procUses(arg1, arg2)) {
                    clauseResult.trueResult = true;
                }
    
            }
        }

        /**
        * Evaluates a single Uses clause on the given PKB where the input contains a StmtId and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateUsesClauseIntIdWild(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;

            if (argType1 == ArgType::INTEGER) {
                // Case 1: Statement number provided
                StmtId arg1 = std::stoi(args.first.value);
                ClauseResult clauseResult;
                if (database.usesKB.getAllVarsUsedByStmt(arg1).size() > 0) {
                    clauseResult.trueResult = true;
                }
    
            } else {
                // Case 2: Procedure name provided
                ProcId arg1 = database.procTable.getProcId(args.first.value);
                ClauseResult clauseResult;
                if (database.usesKB.getAllVarsUsedByProc(arg1).size() > 0) {
                    clauseResult.trueResult = true;
                }
    
            }
        }

        /**
        * Evaluates a single Uses clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateUsesClauseIntIdSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;

            if (argType1 == ArgType::INTEGER) {
                // Case 1: Statement number provided
                StmtId arg1 = std::stoi(args.first.value);
                Synonym arg2 = args.second.value;

                std::unordered_set<VarId> usedVars = database.usesKB.getAllVarsUsedByStmt(arg1);
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);
                for (VarId var : usedVars) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(database.varTable.get(var));
                    clauseResult.rows.emplace_back(resultEntry);
                }

    
            } else {
                // Case 2: Procedure name provided
                ProcId arg1 = database.procTable.getProcId(args.first.value);
                Synonym arg2 = args.second.value;

                std::unordered_set<VarId> usedVars = database.usesKB.getAllVarsUsedByProc(arg1);
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg2);
                for (VarId var : usedVars) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(database.varTable.get(var));
                    clauseResult.rows.emplace_back(resultEntry);
                }

    
            }
        }

        /**
        * Evaluates a single Uses clause on the given PKB where the input contains one synonym and one identifier.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateUsesClauseSynId(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            VarId arg2 = database.varTable.getVarId(args.second.value);

            if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                // Case 1: Synonym is a procedure
                std::unordered_set<StmtId> usingProcs = database.usesKB.getAllProcUsingVar(arg2);
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                for (ProcId proc : usingProcs) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(database.procTable.get(proc).getName());
                    clauseResult.rows.emplace_back(resultEntry);
                }
    
            } else {
                // Case 2: Synonym is a statement
                std::unordered_set<StmtId> usingStmts = database.usesKB.getAllStmtsUsingVar(arg2);
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                for (StmtId stmt : usingStmts) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg1])) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(stmt));
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
    
            }
        }

        /**
        * Evaluates a single Uses clause on the given PKB where the inputs contain a synonym and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateUsesClauseSynWild(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            Synonym arg1 = clause.getArgs().first.value;

            if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                // Case 1: Synonym is a procedure
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                for (ProcId i = 1; i <= database.procTable.size(); i++) {
                    if (database.usesKB.getAllVarsUsedByProc(i).size() > 0) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(database.procTable.get(i).getName());
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
    
            } else {
                // Case 2: Synonym is a statement
                ClauseResult clauseResult;
                clauseResult.syns.emplace_back(arg1);
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (database.usesKB.getAllVarsUsedByStmt(i).size() > 0) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1])) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(i));
                            clauseResult.rows.emplace_back(resultEntry);
                        }
                    }
                }
    
            }
        }

        /**
        * Evaluates a single Uses clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateUsesClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            Synonym arg2 = args.second.value;

            ClauseResult clauseResult;
            if (arg1 < arg2) {
                clauseResult.syns.emplace_back(arg1);
                clauseResult.syns.emplace_back(arg2);
            } else {
                clauseResult.syns.emplace_back(arg2);
                clauseResult.syns.emplace_back(arg1);
            }
            if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                for (StmtId i = 1; i <= database.procTable.size(); i++) {
                    std::unordered_set<VarId> usedVars = database.usesKB.getAllVarsUsedByProc(i);
                    for (VarId var : usedVars) {
                        ClauseResultEntry resultEntry;
                        if (arg1 < arg2) {
                            resultEntry.emplace_back(database.procTable.get(i).getName());
                            resultEntry.emplace_back(database.varTable.get(var));
                        } else {
                            resultEntry.emplace_back(database.varTable.get(var));
                            resultEntry.emplace_back(database.procTable.get(i).getName());
                        }
                        clauseResult.rows.emplace_back(resultEntry);
                    }
                }
    
            } else {
                for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                    if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1])) {
                        std::unordered_set<VarId> usedVars = database.usesKB.getAllVarsUsedByStmt(i);
                        for (VarId var : usedVars) {
                            ClauseResultEntry resultEntry;
                            if (arg1 < arg2) {
                                resultEntry.emplace_back(std::to_string(i));
                                resultEntry.emplace_back(database.varTable.get(var));
                            } else {
                                resultEntry.emplace_back(database.varTable.get(var));
                                resultEntry.emplace_back(std::to_string(i));
                            }
                            clauseResult.rows.emplace_back(resultEntry);
                        }
                    }
                }
    
            }
        }

        void evaluateUsesClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if ((argType1 == ArgType::INTEGER || argType1 == ArgType::IDENTIFIER) && argType2 == ArgType::IDENTIFIER) {
                // One statement number/procedure name, one identifier supplied
                evaluateUsesClauseIntIdId(database, clause, intResult);
            } else if ((argType1 == ArgType::INTEGER || argType1 == ArgType::IDENTIFIER) && argType2 == ArgType::WILDCARD) {
                // One statement number/procedure name, one wildcard supplied
                evaluateUsesClauseIntIdWild(database, clause, intResult);
            } else if ((argType1 == ArgType::INTEGER || argType1 == ArgType::IDENTIFIER) && argType2 == ArgType::SYNONYM) {
                // One statement number/procedure name, one synonym
                evaluateUsesClauseIntIdSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::IDENTIFIER) {
                // One synonym, one identifier
                evaluateUsesClauseSynId(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                evaluateUsesClauseSynWild(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                evaluateUsesClauseSynSyn(database, clause, synonymTable, intResult);
            } else {
                SPA::LoggingUtils::LogErrorMessage("UsesEvaluator::evaluateUsesClause: Invalid ArgTypes for Uses clause. argType1 = %d, argType2 = %d\n", argType1, argType2);

            }
        }

    }
}
