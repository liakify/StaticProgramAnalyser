#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "ModifiesEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace ModifiesEvaluator {

        /**
        * Evaluates a single Modifies clause on the given PKB where the inputs one StmtId and one identifier.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateModifiesClauseIntIdId(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;

            if (argType1 == ArgType::INTEGER) {
                // Case 1: Statement number provided
                StmtId arg1 = std::stoi(args.first.value);
                VarId arg2 = database.varTable.getVarId(args.second.value);

                if (!database.modifiesKB.stmtModifies(arg1, arg2)) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            } else {
                // Case 2: Procedure name provided
                ProcId arg1 = database.procTable.getProcId(args.first.value);
                VarId arg2 = database.varTable.getVarId(args.second.value);

                if (!database.modifiesKB.procModifies(arg1, arg2)) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            }

        }

        /**
        * Evaluates a single Modifies clause on the given PKB where the input contains a StmtId and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateModifiesClauseIntIdWild(PKB::PKB& database, RelationClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;

            if (argType1 == ArgType::INTEGER) {
                // Case 1: Statement number provided
                StmtId arg1 = std::stoi(args.first.value);
                if (database.modifiesKB.getAllVarsModifiedByStmt(arg1).size() <= 0) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            } else {
                // Case 2: Procedure name provided
                ProcId arg1 = database.procTable.getProcId(args.first.value);
                if (database.modifiesKB.getAllVarsModifiedByProc(arg1).size() <= 0) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
    
            }
        }

        /**
        * Evaluates a single Modifies clause on the given PKB where the input contains a StmtId and a synonym.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateModifiesClauseIntIdSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;

            if (argType1 == ArgType::INTEGER) {
                // Case 1: Statement number
                StmtId arg1 = std::stoi(args.first.value);
                Synonym arg2 = args.second.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg2);
                    std::unordered_set<VarId> modifiedVars = database.modifiesKB.getAllVarsModifiedByStmt(arg1);
                    for (VarId var : modifiedVars) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(database.varTable.get(var));
                        intResult.rows.emplace_back(resultEntry);
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.modifiesKB.stmtModifies(arg1, std::stoi(resultEntry[index]))) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
    
            } else {
                // Case 2: Procedure name
                ProcId arg1 = database.procTable.getProcId(args.first.value);
                Synonym arg2 = args.second.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg2);
                    std::unordered_set<VarId> modifiedVars = database.modifiesKB.getAllVarsModifiedByProc(arg1);
                    for (VarId var : modifiedVars) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(database.varTable.get(var));
                        intResult.rows.emplace_back(resultEntry);
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.modifiesKB.procModifies(arg1, std::stoi(resultEntry[index]))) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
            }

            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        /**
        * Evaluates a single Modifies clause on the given PKB where the input contains one synonym and one identifier.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateModifiesClauseSynId(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            VarId arg2 = database.varTable.getVarId(args.second.value);

            if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                // Case 1: Synonym is a procedure
                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg1);
                    std::unordered_set<StmtId> modifyingProcs = database.modifiesKB.getAllProcModifyVar(arg2);
                    for (ProcId proc : modifyingProcs) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(database.procTable.get(proc).getName());
                        intResult.rows.emplace_back(resultEntry);
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.modifiesKB.procModifies(std::stoi(resultEntry[index]), arg2)) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
    
            } else {
                // Case 2: Synonym is a statement
                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg1);
                    std::unordered_set<StmtId> modifyingStmts = database.modifiesKB.getAllStmtsModifyVar(arg2);
                    for (StmtId stmt : modifyingStmts) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(stmt)->getType(), synonymTable[arg1])) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(std::to_string(stmt));
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.modifiesKB.stmtModifies(std::stoi(resultEntry[index]), arg2)) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
            }

            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        /**
        * Evaluates a single Modifies clause on the given PKB where the inputs contain a synonym and a wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateModifiesClauseSynWild(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            Synonym arg1 = clause.getArgs().first.value;

            if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                // Case 1: Synonym is a procedure
                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg1);
                    for (ProcId i = 1; i <= database.procTable.size(); i++) {
                        if (database.modifiesKB.getAllVarsModifiedByProc(i).size() > 0) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(database.procTable.get(i).getName());
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.modifiesKB.getAllVarsModifiedByProc(std::stoi(resultEntry[index])).size() > 0) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
    
            } else {
                // Case 2: Synonym is a statement
                if (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end()) {
                    intResult.syns.emplace_back(arg1);
                    for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                        if (database.modifiesKB.getAllVarsModifiedByStmt(i).size() > 0) {
                            if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1])) {
                                ClauseResultEntry resultEntry;
                                resultEntry.emplace_back(std::to_string(i));
                                intResult.rows.emplace_back(resultEntry);
                            }
                        }
                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.modifiesKB.getAllVarsModifiedByStmt(std::stoi(resultEntry[index])).size() > 0) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
            }
            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        /**
        * Evaluates a single Modifies clause on the given PKB where the input contains two synonyms.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateModifiesClauseSynSyn(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            Synonym arg2 = args.second.value;

            bool foundSyn1 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) == intResult.syns.end());
            bool foundSyn2 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg2) == intResult.syns.end());

            if (!foundSyn1 && !foundSyn2) {
                if (arg1 < arg2) {
                    intResult.syns.emplace_back(arg1);
                    intResult.syns.emplace_back(arg2);
                } else {
                    intResult.syns.emplace_back(arg2);
                    intResult.syns.emplace_back(arg1);
                }
                if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                    for (ProcId i = 1; i <= database.procTable.size(); i++) {
                        std::unordered_set<VarId> modifiedVars = database.modifiesKB.getAllVarsModifiedByProc(i);
                        for (VarId var : modifiedVars) {
                            ClauseResultEntry resultEntry;
                            if (arg1 < arg2) {
                                resultEntry.emplace_back(database.procTable.get(i).getName());
                                resultEntry.emplace_back(database.varTable.get(var));
                            } else {
                                resultEntry.emplace_back(database.varTable.get(var));
                                resultEntry.emplace_back(database.procTable.get(i).getName());
                            }
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }

                } else {
                    for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                        if (SPA::TypeUtils::isStmtTypeDesignEntity(database.stmtTable.get(i)->getType(), synonymTable[arg1])) {
                            std::unordered_set<VarId> modifiedVars = database.modifiesKB.getAllVarsModifiedByStmt(i);
                            for (VarId var : modifiedVars) {
                                ClauseResultEntry resultEntry;
                                if (arg1 < arg2) {
                                    resultEntry.emplace_back(std::to_string(i));
                                    resultEntry.emplace_back(database.varTable.get(var));
                                } else {
                                    resultEntry.emplace_back(database.varTable.get(var));
                                    resultEntry.emplace_back(std::to_string(i));
                                }
                                intResult.rows.emplace_back(resultEntry);
                            }
                        }
                    }
                }
            } else if (foundSyn1 && !foundSyn2) {
                if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    intResult.syns.emplace_back(arg2);
                    std::sort(intResult.syns.begin(), intResult.syns.end());
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        std::unordered_set<VarId> vars = database.modifiesKB.getAllVarsModifiedByProc(std::stoi(resultEntry[index1]));
                        for (VarId var : vars) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index2, database.varTable.get(var));
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                } else {
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    intResult.syns.emplace_back(arg2);
                    std::sort(intResult.syns.begin(), intResult.syns.end());
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        std::unordered_set<VarId> vars = database.modifiesKB.getAllVarsModifiedByStmt(std::stoi(resultEntry[index1]));
                        for (VarId var : vars) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index2, database.varTable.get(var));
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
            } else if (!foundSyn1 && foundSyn2) {
                if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    intResult.syns.emplace_back(arg1);
                    std::sort(intResult.syns.begin(), intResult.syns.end());
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        std::unordered_set<ProcId> procs = database.modifiesKB.getAllProcModifyVar(database.varTable.getVarId(resultEntry[index2]));
                        for (ProcId proc : procs) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index1, database.procTable.get(proc).getName());
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                } else {
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    intResult.syns.emplace_back(arg1);
                    std::sort(intResult.syns.begin(), intResult.syns.end());
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        std::unordered_set<StmtId> stmts = database.modifiesKB.getAllStmtsModifyVar(database.varTable.getVarId(resultEntry[index2]));
                        for (StmtId stmt : stmts) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index1, std::to_string(stmt));
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
            } else if (foundSyn1 && foundSyn2) {
                if (synonymTable[arg1] == DesignEntity::PROCEDURE) {
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.modifiesKB.procModifies(database.procTable.getProcId(resultEntry[index1]), database.varTable.getVarId(resultEntry[index2]))) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                } else {
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), arg2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (database.modifiesKB.stmtModifies(std::stoi(resultEntry[index1]), database.varTable.getVarId(resultEntry[index2]))) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
            }
            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        void evaluateModifiesClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if ((argType1 == ArgType::INTEGER || argType1 == ArgType::IDENTIFIER) && argType2 == ArgType::IDENTIFIER) {
                // One statement number/procedure name, one identifier supplied
                evaluateModifiesClauseIntIdId(database, clause, intResult);
            } else if ((argType1 == ArgType::INTEGER || argType1 == ArgType::IDENTIFIER) && argType2 == ArgType::WILDCARD) {
                // One statement number/procedure name, one wildcard supplied
                evaluateModifiesClauseIntIdWild(database, clause, intResult);
            } else if ((argType1 == ArgType::INTEGER || argType1 == ArgType::IDENTIFIER) && argType2 == ArgType::SYNONYM) {
                // One statement number/procedure name, one synonym
                evaluateModifiesClauseIntIdSyn(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::IDENTIFIER) {
                // One synonym, one identifier
                evaluateModifiesClauseSynId(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                evaluateModifiesClauseSynWild(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                evaluateModifiesClauseSynSyn(database, clause, synonymTable, intResult);
            } else {
                SPA::LoggingUtils::LogErrorMessage("ModifiesEvaluator::evaluateModifiesClause: Invalid ArgTypes for Modifies clause. argType1 = %d, argType2 = %d\n", argType1, argType2);

            }
        }

    }
}
