#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

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
        void evaluateAssignPatternClauseWildWild(PKB::PKB& database, PatternClause &clause, ClauseResult& intResult) {

            Synonym arg0 = clause.getSynonym().value;

            std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(StmtType::ASSIGN);

            if (std::find(intResult.syns.begin(), intResult.syns.end(), arg0) == intResult.syns.end()) {
                intResult.syns.emplace_back(arg0);
                for (StmtId stmt : stmts) {
                    ClauseResultEntry resultEntry;
                    resultEntry.emplace_back(std::to_string(stmt));
                    intResult.rows.emplace_back(resultEntry);
                }
            } else {
                int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    if (stmts.find(std::stoi(resultEntry[index])) != stmts.end()) {
                        updatedResult.emplace_back(resultEntry);
                    }
                }
                intResult.rows = updatedResult;
            }

            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one wildcard and one pattern.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAssignPatternClauseWildPtn(PKB::PKB& database, PatternClause &clause, ClauseResult& intResult) {
            Synonym arg0 = clause.getSynonym().value;
            Pattern arg2 = clause.getArgs().second.value;

            std::unordered_set<StmtId> stmts = database.patternKB.getAssignPatternStmts(arg2);

            if (std::find(intResult.syns.begin(), intResult.syns.end(), arg0) == intResult.syns.end()) {
                intResult.syns.emplace_back(arg0);
                for (StmtId stmt : stmts) {
                    if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(stmt));
                        intResult.rows.emplace_back(resultEntry);
                    }
                }
            } else {
                int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    if (stmts.find(std::stoi(resultEntry[index])) != stmts.end()) {
                        updatedResult.emplace_back(resultEntry);
                    }
                }
                intResult.rows = updatedResult;
            }

            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }

        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one identifier and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAssignPatternClauseIdWild(PKB::PKB& database, PatternClause& clause, ClauseResult& intResult) {
            Synonym arg0 = clause.getSynonym().value;
            VarName arg1 = clause.getArgs().first.value;
            VarId arg1Id = database.varTable.getVarId(arg1);

            std::unordered_set<StmtId> stmts = database.modifiesKB.getAllStmtsModifyVar(arg1Id);

            if (std::find(intResult.syns.begin(), intResult.syns.end(), arg0) == intResult.syns.end()) {
                intResult.syns.emplace_back(arg0);
                for (StmtId stmt : stmts) {
                    if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(stmt));
                        intResult.rows.emplace_back(resultEntry);
                    }
                }
            } else {
                int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    if (stmts.find(std::stoi(resultEntry[index])) != stmts.end()) {
                        if (database.stmtTable.get(std::stoi(resultEntry[index]))->getType() == StmtType::ASSIGN) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            }

            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        /**
        * Evaluates a single assign pattern clause on the given PKB where the inputs are one identifier and one pattern.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateAssignPatternClauseIdPtn(PKB::PKB& database, PatternClause& clause, ClauseResult& intResult) {
            Synonym arg0 = clause.getSynonym().value;

            std::pair<Argument, Argument> args = clause.getArgs();
            VarName arg1 = args.first.value;
            VarId arg1Id = database.varTable.getVarId(arg1);
            Pattern arg2 = args.second.value;

            std::unordered_set<StmtId> stmts1 = database.modifiesKB.getAllStmtsModifyVar(arg1Id);
            std::unordered_set<StmtId> stmts2 = database.patternKB.getAssignPatternStmts(arg2);

            if (std::find(intResult.syns.begin(), intResult.syns.end(), arg0) == intResult.syns.end()) {
                // Find the intersection
                std::unordered_set<StmtId> stmts;
                for (StmtId stmt : stmts1) {
                    if (stmts2.find(stmt) != stmts2.end()) {
                        stmts.insert(stmt);
                    }
                }

                intResult.syns.emplace_back(arg0);
                for (StmtId stmt : stmts) {
                    if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                        ClauseResultEntry resultEntry;
                        resultEntry.emplace_back(std::to_string(stmt));
                        intResult.rows.emplace_back(resultEntry);
                    }
                }
            } else {
                int index = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    if (stmts1.find(std::stoi(resultEntry[index])) != stmts1.end() && stmts2.find(std::stoi(resultEntry[index])) != stmts2.end()) {
                        updatedResult.emplace_back(resultEntry);
                    }
                }
                intResult.rows = updatedResult;
            }

            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }

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
            std::unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {
            Synonym arg0 = clause.getSynonym().value;
            Synonym arg1 = clause.getArgs().first.value;

            std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(StmtType::ASSIGN);

            bool foundSyn0 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg0) != intResult.syns.end());
            bool foundSyn1 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) != intResult.syns.end());

            if (!foundSyn0 && !foundSyn1) {
                // If synonym 'arg1' appeared on LHS, then it must have been modified by the assignment
                if (arg0 < arg1) {
                    intResult.syns.emplace_back(arg0);
                    intResult.syns.emplace_back(arg1);
                } else {
                    intResult.syns.emplace_back(arg1);
                    intResult.syns.emplace_back(arg0);
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
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }
                }
            } else if (foundSyn0 && !foundSyn1) {
                int index0 = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                intResult.syns.emplace_back(arg1);
                std::sort(intResult.syns.begin(), intResult.syns.end());
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    StmtId stmt = std::stoi(resultEntry[index0]);
                    if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                        std::unordered_set<VarId> vars = database.modifiesKB.getAllVarsModifiedByStmt(stmt);
                        for (VarId var : vars) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index1, database.varTable.get(var));
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            } else if (!foundSyn0 && foundSyn1) {
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                intResult.syns.emplace_back(arg0);
                std::sort(intResult.syns.begin(), intResult.syns.end());
                int index0 = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    VarId var = database.varTable.getVarId(resultEntry[index1]);
                    std::unordered_set<StmtId> stmts = database.modifiesKB.getAllStmtsModifyVar(var);
                    for (StmtId stmt : stmts) {
                        if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index0, std::to_string(stmt));
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            } else if (foundSyn0 && foundSyn1) {
                int index0 = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    StmtId stmt = std::stoi(resultEntry[index0]);
                    VarId var = database.varTable.getVarId(resultEntry[index1]);
                    if (database.modifiesKB.stmtModifies(stmt, var)) {
                        if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            }

            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }

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
            std::unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {
            Synonym arg0 = clause.getSynonym().value;

            std::pair<Argument, Argument> args = clause.getArgs();
            Synonym arg1 = args.first.value;
            Pattern arg2 = args.second.value;

            bool foundSyn0 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg0) != intResult.syns.end());
            bool foundSyn1 = (std::find(intResult.syns.begin(), intResult.syns.end(), arg1) != intResult.syns.end());

            std::unordered_set<StmtId> stmts1 = database.stmtTable.getStmtsByType(StmtType::ASSIGN);
            std::unordered_set<StmtId> stmts2 = database.patternKB.getAssignPatternStmts(arg2);

            if (!foundSyn0 && !foundSyn1) {
                // Find the intersection
                std::unordered_set<StmtId> stmts;
                for (StmtId stmt : stmts1) {
                    if (stmts2.find(stmt) != stmts2.end()) {
                        stmts.insert(stmt);
                    }
                }

                if (arg0 < arg1) {
                    intResult.syns.emplace_back(arg0);
                    intResult.syns.emplace_back(arg1);
                } else {
                    intResult.syns.emplace_back(arg1);
                    intResult.syns.emplace_back(arg0);
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
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }
                }
            } else if (foundSyn0 && !foundSyn1) {
                int index0 = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                intResult.syns.emplace_back(arg1);
                std::sort(intResult.syns.begin(), intResult.syns.end());
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    StmtId stmt = std::stoi(resultEntry[index0]);
                    if (database.stmtTable.get(stmt)->getType() == StmtType::ASSIGN && stmts2.find(stmt) != stmts2.end()) {
                        std::unordered_set<VarId> vars = database.modifiesKB.getAllVarsModifiedByStmt(stmt);
                        for (VarId var : vars) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index1, database.varTable.get(var));
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            } else if (!foundSyn0 && foundSyn1) {
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                intResult.syns.emplace_back(arg0);
                std::sort(intResult.syns.begin(), intResult.syns.end());
                int index0 = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    VarId var = database.varTable.getVarId(resultEntry[index1]);
                    std::unordered_set<StmtId> stmts = database.modifiesKB.getAllStmtsModifyVar(var);
                    for (StmtId stmt : stmts) {
                        if (stmts2.find(stmt) != stmts2.end()) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index0, std::to_string(stmt));
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                }
                intResult.rows = updatedResult;
            } else if (foundSyn0 && foundSyn1) {
                int index0 = std::find(intResult.syns.begin(), intResult.syns.end(), arg0) - intResult.syns.begin();
                int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), arg1) - intResult.syns.begin();
                std::vector<ClauseResultEntry> updatedResult;
                for (ClauseResultEntry& resultEntry : intResult.rows) {
                    StmtId stmt = std::stoi(resultEntry[index0]);
                    VarId var = database.varTable.getVarId(resultEntry[index1]);
                    if (database.modifiesKB.stmtModifies(stmt, var) && stmts2.find(stmt) != stmts2.end()) {
                        updatedResult.emplace_back(resultEntry);
                    }
                }
                intResult.rows = updatedResult;
            }

            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        void evaluateAssignPatternClause(PKB::PKB& database, PatternClause clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            ArgType argType1 = args.first.type;
            ArgType argType2 = args.second.type;

            if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // 2 wildcards
                evaluateAssignPatternClauseWildWild(database, clause, intResult);
            } else if (argType1 == ArgType::WILDCARD &&
                (argType2 == ArgType::EXACT_PATTERN || argType2 == ArgType::INCLUSIVE_PATTERN)) {
                // 1 wildcard, 1 pattern
                evaluateAssignPatternClauseWildPtn(database, clause, intResult);
            } else if (argType1 == ArgType::IDENTIFIER && argType2 == ArgType::WILDCARD) {
                // 1 identifier, 1 wildcard
                evaluateAssignPatternClauseIdWild(database, clause, intResult);
            } else if (argType1 == ArgType::IDENTIFIER &&
                (argType2 == ArgType::EXACT_PATTERN || argType2 == ArgType::INCLUSIVE_PATTERN)) {
                // 1 identifier, 1 pattern
                evaluateAssignPatternClauseIdPtn(database, clause, intResult);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // 1 synonym, 1 wildcard
                evaluateAssignPatternClauseSynWild(database, clause, synonymTable, intResult);
            } else if (argType1 == ArgType::SYNONYM &&
                (argType2 == ArgType::EXACT_PATTERN || argType2 == ArgType::INCLUSIVE_PATTERN)) {
                // 1 synonym, 1 pattern
                evaluateAssignPatternClauseSynPtn(database, clause, synonymTable, intResult);
            } else {
                SPA::LoggingUtils::LogErrorMessage("AssignPatternEvaluator::evaluateAssignPatternClause: Invalid ArgTypes for Assign Pattern clause. argType1 = %d, argType2 = %d\n", argType1, argType2);

            }
        }

    }
}