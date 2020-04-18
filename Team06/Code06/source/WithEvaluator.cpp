#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "WithEvaluator.h"
#include "LoggingUtils.h"
#include "Simple.h"
#include "TypeUtils.h"

namespace PQL {
    namespace WithEvaluator {

        /**
        * Extracts data value pairs for a given StmtType.
        *
        * @param    stmtType    StmtType to extract data value pairs for.
        * @return   The data value pairs extracted.
        */
        std::unordered_multimap<std::string, StmtId> getStmtDataPairs(PKB::PKB database, StmtType stmtType) {
            std::unordered_multimap<std::string, StmtId> result;

            if (stmtType == StmtType::CALL) {
                std::unordered_set<StmtId> callStmts = database.stmtTable.getStmtsByType(StmtType::CALL);
                for (StmtId callStmtId : callStmts) {
                    SIMPLE::CallStmt* callStmt = dynamic_cast<SIMPLE::CallStmt*>(database.stmtTable.get(callStmtId).get());
                    result.insert(std::make_pair(callStmt->getProc(), callStmtId));
                }
                return result;
            } else if (stmtType == StmtType::READ) {
                std::unordered_set<StmtId> readStmts = database.stmtTable.getStmtsByType(StmtType::READ);
                for (StmtId readStmtId : readStmts) {
                    SIMPLE::ReadStmt* readStmt = dynamic_cast<SIMPLE::ReadStmt*>(database.stmtTable.get(readStmtId).get());
                    result.insert(std::make_pair(database.varTable.get(readStmt->getVar()), readStmtId));
                }
                return result;
            } else if (stmtType == StmtType::PRINT) {
                std::unordered_set<StmtId> printStmts = database.stmtTable.getStmtsByType(StmtType::PRINT);
                for (StmtId printStmtId : printStmts) {
                    SIMPLE::PrintStmt* printStmt = dynamic_cast<SIMPLE::PrintStmt*>(database.stmtTable.get(printStmtId).get());
                    result.insert(std::make_pair(database.varTable.get(printStmt->getVar()), printStmtId));
                }
                return result;
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single With clause where the inputs are two identifiers.
        *
        * @param    database        The PKB to evaluate the clause on.
        * @param    clause          The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateIdentifierEqual(PKB::PKB& database, WithClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            Argument arg1 = args.first;
            Argument arg2 = args.second;

            if (arg1.type == ArgType::IDENTIFIER && arg2.type == ArgType::ATTRIBUTE) {
                // Always ensure that arg1 is the attribute
                std::swap(arg1, arg2);
            }

            ArgType argType1 = arg1.type;
            ArgType argType2 = arg2.type;

            if (argType1 == ArgType::ATTRIBUTE && argType2 == ArgType::IDENTIFIER) {
                // Case 1: LHS is an attribute, RHS is an identifier

                Synonym syn1 = arg1.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), syn1) == intResult.syns.end()) {

                    if (synonymTable[syn1] == DesignEntity::CALL || synonymTable[syn1] == DesignEntity::READ || synonymTable[syn1] == DesignEntity::PRINT) {
                        intResult.syns.emplace_back(syn1);
                        std::unordered_multimap<ProcName, StmtId> stmts =
                            getStmtDataPairs(database, SPA::TypeUtils::getStmtTypeFromDesignEntity(synonymTable[syn1]));
                        for (std::pair<std::string, StmtId> stmt : stmts) {
                            if (stmt.first == arg2.value) {
                                ClauseResultEntry resultEntry;
                                resultEntry.emplace_back(std::to_string(stmt.second));
                                intResult.rows.emplace_back(resultEntry);
                            }
                        }


                    } else if (synonymTable[syn1] == DesignEntity::PROCEDURE) {
                        intResult.syns.emplace_back(syn1);
                        if (database.procTable.getProcId(arg2.value) != -1) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(arg2.value);
                            intResult.rows.emplace_back(resultEntry);
                        }

                    } else if (synonymTable[syn1] == DesignEntity::VARIABLE) {
                        intResult.syns.emplace_back(syn1);
                        if (database.varTable.getVarId(arg2.value) != -1) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(arg2.value);
                            intResult.rows.emplace_back(resultEntry);
                        }

                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), syn1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (resultEntry[index] == arg2.value) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }

            } else if (argType1 == ArgType::ATTRIBUTE && argType2 == ArgType::ATTRIBUTE) {
                // Case 2: Both LHS and RHS are attributes

                std::unordered_multimap<std::string, StmtId> result1;
                std::unordered_multimap<std::string, StmtId> result2;

                Synonym syn1 = arg1.value;
                Synonym syn2 = arg2.value;

                if (synonymTable[syn1] == DesignEntity::CALL || synonymTable[syn1] == DesignEntity::READ || synonymTable[syn1] == DesignEntity::PRINT) {
                    result1 = getStmtDataPairs(database, SPA::TypeUtils::getStmtTypeFromDesignEntity(synonymTable[syn1]));
                } else if (synonymTable[syn1] == DesignEntity::VARIABLE) {
                    for (VarId i = 1; i <= database.varTable.size(); i++) {
                        result1.insert(std::make_pair(database.varTable.get(i), 0));
                    }
                } else if (synonymTable[syn1] == DesignEntity::PROCEDURE) {
                    for (ProcId i = 1; i <= database.procTable.size(); i++) {
                        result1.insert(std::make_pair(database.procTable.get(i).getName(), 0));
                    }
                }

                if (synonymTable[syn2] == DesignEntity::CALL || synonymTable[syn2] == DesignEntity::READ || synonymTable[syn2] == DesignEntity::PRINT) {
                    result2 = getStmtDataPairs(database, SPA::TypeUtils::getStmtTypeFromDesignEntity(synonymTable[syn2]));
                } else if (synonymTable[syn2] == DesignEntity::VARIABLE) {
                    for (VarId i = 1; i <= database.varTable.size(); i++) {
                        result2.insert(std::make_pair(database.varTable.get(i), 0));
                    }
                } else if (synonymTable[syn2] == DesignEntity::PROCEDURE) {
                    for (ProcId i = 1; i <= database.procTable.size(); i++) {
                        result2.insert(std::make_pair(database.procTable.get(i).getName(), 0));
                    }
                }

                bool foundSyn1 = (std::find(intResult.syns.begin(), intResult.syns.end(), syn1) != intResult.syns.end());
                bool foundSyn2 = (std::find(intResult.syns.begin(), intResult.syns.end(), syn2) != intResult.syns.end());

                if (!foundSyn1 && !foundSyn2) {
                    bool singleSynonym = (syn1 == syn2);
                    if (singleSynonym) {
                        intResult.syns.emplace_back(syn1);
                    } else if (syn1 < syn2) {
                        intResult.syns.emplace_back(syn1);
                        intResult.syns.emplace_back(syn2);
                    } else {
                        intResult.syns.emplace_back(syn2);
                        intResult.syns.emplace_back(syn1);
                    }

                    for (std::pair<std::string, StmtId> entry1 : result1) {
                        auto entry2Range = result2.equal_range(entry1.first);
                        if (entry2Range.first != result2.end()) {
                            for (auto entry2 = entry2Range.first; entry2 != entry2Range.second; entry2++) {
                                ClauseResultEntry resultEntry;
                                std::string val1, val2;
                                if (entry1.second != 0) {
                                    val1 = std::to_string(entry1.second);
                                } else {
                                    val1 = entry1.first;
                                }
                                if (entry2->second != 0) {
                                    val2 = std::to_string(entry2->second);
                                } else {
                                    val2 = entry2->first;
                                }

                                if (singleSynonym) {
                                    if (val1 == val2) {
                                        resultEntry.emplace_back(val1);
                                    }
                                } else {
                                    if (syn1 < syn2) {
                                        resultEntry.emplace_back(val1);
                                        resultEntry.emplace_back(val2);
                                    } else {
                                        resultEntry.emplace_back(val2);
                                        resultEntry.emplace_back(val1);
                                    }
                                }
                                intResult.rows.emplace_back(resultEntry);
                            }
                        }
                    }
                } else if (foundSyn1 && !foundSyn2) {
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), syn1) - intResult.syns.begin();
                    intResult.syns.emplace_back(syn2);
                    std::sort(intResult.syns.begin(), intResult.syns.end());
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), syn2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        std::string value = resultEntry[index1];
                        if (synonymTable[syn1] == DesignEntity::CALL) {
                            value = dynamic_cast<CallStmt*>(database.stmtTable.get(std::stoi(value)).get())->getProc();
                        } else if (synonymTable[syn1] == DesignEntity::READ) {
                            value = database.varTable.get(dynamic_cast<ReadStmt*>(database.stmtTable.get(std::stoi(value)).get())->getVar());
                        } else if (synonymTable[syn1] == DesignEntity::PRINT) {
                            value = database.varTable.get(dynamic_cast<PrintStmt*>(database.stmtTable.get(std::stoi(value)).get())->getVar());
                        }
                        auto entry2Range = result2.equal_range(value);
                        if (entry2Range.first != result2.end()) {
                            for (auto entry2 = entry2Range.first; entry2 != entry2Range.second; entry2++) {
                                std::string val2;
                                if (entry2->second != 0) {
                                    val2 = std::to_string(entry2->second);
                                } else {
                                    val2 = entry2->first;
                                }
                                ClauseResultEntry newResultEntry(resultEntry);
                                newResultEntry.insert(newResultEntry.begin() + index2, val2);
                                updatedResult.emplace_back(newResultEntry);
                            }
                        }
                    }
                    intResult.rows = updatedResult;
                } else if (!foundSyn1 && foundSyn2) {
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), syn2) - intResult.syns.begin();
                    intResult.syns.emplace_back(syn1);
                    std::sort(intResult.syns.begin(), intResult.syns.end());
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), syn1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        std::string value = resultEntry[index2];
                        if (synonymTable[syn2] == DesignEntity::CALL) {
                            value = dynamic_cast<CallStmt*>(database.stmtTable.get(std::stoi(value)).get())->getProc();
                        } else if (synonymTable[syn2] == DesignEntity::READ) {
                            value = database.varTable.get(dynamic_cast<ReadStmt*>(database.stmtTable.get(std::stoi(value)).get())->getVar());
                        } else if (synonymTable[syn2] == DesignEntity::PRINT) {
                            value = database.varTable.get(dynamic_cast<PrintStmt*>(database.stmtTable.get(std::stoi(value)).get())->getVar());
                        }
                        auto entry1Range = result1.equal_range(value);
                        if (entry1Range.first != result1.end()) {
                            for (auto entry1 = entry1Range.first; entry1 != entry1Range.second; entry1++) {
                                std::string val1;
                                if (entry1->second != 0) {
                                    val1 = std::to_string(entry1->second);
                                } else {
                                    val1 = entry1->first;
                                }
                                ClauseResultEntry newResultEntry(resultEntry);
                                newResultEntry.insert(newResultEntry.begin() + index1, val1);
                                updatedResult.emplace_back(newResultEntry);
                            }
                        }
                    }
                    intResult.rows = updatedResult;
                } else if (foundSyn1 && foundSyn2) {
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), syn1) - intResult.syns.begin();
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), syn2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        std::string value1 = resultEntry[index1];
                        std::string value2 = resultEntry[index2];
                        if (synonymTable[syn1] == DesignEntity::CALL) {
                            value1 = dynamic_cast<CallStmt*>(database.stmtTable.get(std::stoi(value1)).get())->getProc();
                        } else if (synonymTable[syn1] == DesignEntity::READ) {
                            value1 = database.varTable.get(dynamic_cast<ReadStmt*>(database.stmtTable.get(std::stoi(value1)).get())->getVar());
                        } else if (synonymTable[syn1] == DesignEntity::PRINT) {
                            value1 = database.varTable.get(dynamic_cast<PrintStmt*>(database.stmtTable.get(std::stoi(value1)).get())->getVar());
                        }
                        if (synonymTable[syn2] == DesignEntity::CALL) {
                            value2 = dynamic_cast<CallStmt*>(database.stmtTable.get(std::stoi(value2)).get())->getProc();
                        } else if (synonymTable[syn2] == DesignEntity::READ) {
                            value2 = database.varTable.get(dynamic_cast<ReadStmt*>(database.stmtTable.get(std::stoi(value2)).get())->getVar());
                        } else if (synonymTable[syn2] == DesignEntity::PRINT) {
                            value2 = database.varTable.get(dynamic_cast<PrintStmt*>(database.stmtTable.get(std::stoi(value2)).get())->getVar());
                        }
                        if (value1 == value2) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }

            } else {
                SPA::LoggingUtils::LogErrorMessage("WithEvaluator::evaluateIdentifierEqual: Invalid ArgTypes for identifier With clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
            }
            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        /**
        * Evaluates a single With clause where the inputs are two integers.
        *
        * @param    database        The PKB to evaluate the clause on.
        * @param    clause          The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateIntegerEqual(PKB::PKB& database, WithClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            std::pair<Argument, Argument> args = clause.getArgs();
            Argument arg1 = args.first;
            Argument arg2 = args.second;

            if (arg1.type == ArgType::INTEGER && (arg2.type == ArgType::ATTRIBUTE || arg2.type == ArgType::SYNONYM)) {
                // Always ensure that arg1 is the attribute
                std::swap(arg1, arg2);
            }

            ArgType argType1 = arg1.type;
            ArgType argType2 = arg2.type;

            if ((argType1 == ArgType::ATTRIBUTE || argType1 == ArgType::SYNONYM) && argType2 == ArgType::INTEGER) {
                Synonym syn1 = arg1.value;

                if (std::find(intResult.syns.begin(), intResult.syns.end(), syn1) == intResult.syns.end()) {
                    intResult.syns.emplace_back(syn1);
                    // Case 1: LHS is a synonym, RHS is an integer
                    if (synonymTable[syn1] == DesignEntity::CONSTANT) {
                        if (database.constTable.getConstId(arg2.value) != -1) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(arg2.value);
                            intResult.rows.emplace_back(resultEntry);
                        }

                    } else if (synonymTable[syn1] == DesignEntity::STATEMENT || synonymTable[syn1] == DesignEntity::PROG_LINE) {
                        StmtId stmtNumber = std::stoi(arg2.value);
                        if (1 <= stmtNumber && stmtNumber <= database.stmtTable.size()) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(arg2.value);
                            intResult.rows.emplace_back(resultEntry);
                        }

                    } else {
                        std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(SPA::TypeUtils::getStmtTypeFromDesignEntity(synonymTable[syn1]));
                        if (stmts.find(std::stoi(arg2.value)) != stmts.end()) {
                            ClauseResultEntry resultEntry;
                            resultEntry.emplace_back(arg2.value);
                            intResult.rows.emplace_back(resultEntry);
                        }

                    }
                } else {
                    int index = std::find(intResult.syns.begin(), intResult.syns.end(), syn1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (resultEntry[index] == arg2.value) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }

            } else if ((argType1 == ArgType::ATTRIBUTE || argType1 == ArgType::SYNONYM) && (argType2 == ArgType::ATTRIBUTE || argType2 == ArgType::SYNONYM)) {
                // Case 2: Both LHS and RHS are synonyms
                std::unordered_set<std::string> result1;
                std::unordered_set<std::string> result2;

                Synonym syn1 = arg1.value;
                Synonym syn2 = arg2.value;

                if (synonymTable[syn1] == DesignEntity::STATEMENT || synonymTable[syn1] == DesignEntity::PROG_LINE) {
                    for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                        result1.insert(std::to_string(i));
                    }
                } else if (synonymTable[syn1] == DesignEntity::CONSTANT) {
                    for (ConstId i = 1; i <= database.constTable.size(); i++) {
                        result1.insert(database.constTable.get(i));
                    }
                } else {
                    std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(SPA::TypeUtils::getStmtTypeFromDesignEntity(synonymTable[syn1]));
                    for (StmtId stmt : stmts) {
                        result1.insert(std::to_string(stmt));
                    }
                }

                if (synonymTable[syn2] == DesignEntity::STATEMENT || synonymTable[syn2] == DesignEntity::PROG_LINE) {
                    for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                        result2.insert(std::to_string(i));
                    }
                } else if (synonymTable[syn2] == DesignEntity::CONSTANT) {
                    for (ConstId i = 1; i <= database.constTable.size(); i++) {
                        result2.insert(database.constTable.get(i));
                    }
                } else {
                    std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(SPA::TypeUtils::getStmtTypeFromDesignEntity(synonymTable[syn2]));
                    for (StmtId stmt : stmts) {
                        result2.insert(std::to_string(stmt));
                    }
                }

                bool foundSyn1 = (std::find(intResult.syns.begin(), intResult.syns.end(), syn1) != intResult.syns.end());
                bool foundSyn2 = (std::find(intResult.syns.begin(), intResult.syns.end(), syn2) != intResult.syns.end());

                if (!foundSyn1 && !foundSyn2) {

                    bool singleSynonym = (syn1 == syn2);

                    if (singleSynonym) {
                        intResult.syns.emplace_back(syn1);
                    } else if (syn1 < syn2) {
                        intResult.syns.emplace_back(syn1);
                        intResult.syns.emplace_back(syn2);
                    } else {
                        intResult.syns.emplace_back(syn2);
                        intResult.syns.emplace_back(syn1);
                    }

                    for (std::string result : result1) {
                        if (result2.find(result) != result2.end()) {
                            ClauseResultEntry resultEntry;
                            if (singleSynonym) {
                                resultEntry.emplace_back(result);
                            } else {
                                resultEntry.emplace_back(result);
                                resultEntry.emplace_back(result);
                            }
                            intResult.rows.emplace_back(resultEntry);
                        }
                    }
                } else if (foundSyn1 && !foundSyn2) {
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), syn1) - intResult.syns.begin();
                    intResult.syns.emplace_back(syn2);
                    std::sort(intResult.syns.begin(), intResult.syns.end());
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), syn2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        std::string value = resultEntry[index1];
                        if (result2.find(value) != result2.end()) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index2, value);
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                } else if (!foundSyn1 && foundSyn2) {
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), syn2) - intResult.syns.begin();
                    intResult.syns.emplace_back(syn1);
                    std::sort(intResult.syns.begin(), intResult.syns.end());
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), syn1) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        std::string value = resultEntry[index2];
                        if (result1.find(value) != result1.end()) {
                            ClauseResultEntry newResultEntry(resultEntry);
                            newResultEntry.insert(newResultEntry.begin() + index1, value);
                            updatedResult.emplace_back(newResultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                } else if (foundSyn1 && foundSyn2) {
                    int index1 = std::find(intResult.syns.begin(), intResult.syns.end(), syn1) - intResult.syns.begin();
                    int index2 = std::find(intResult.syns.begin(), intResult.syns.end(), syn2) - intResult.syns.begin();
                    std::vector<ClauseResultEntry> updatedResult;
                    for (ClauseResultEntry& resultEntry : intResult.rows) {
                        if (resultEntry[index1] == resultEntry[index2]) {
                            updatedResult.emplace_back(resultEntry);
                        }
                    }
                    intResult.rows = updatedResult;
                }
            } else {
                SPA::LoggingUtils::LogErrorMessage("WithEvaluator::evaluateIntegerEqual: Invalid ArgTypes for integer With clause. argType1 = %d, argType2 = %d\n", argType1, argType2);

            }
            if (intResult.rows.empty()) {
                intResult.trueResult = false;
            }
        }

        /**
        * Evaluates a single With clause where the inputs are two literals.
        *
        * @param    clause          The clause to evaluate.
        * @param    intResult   The intermediate result table for the group that the clause belongs to.
        */
        void evaluateLiteralEqual(WithClause clause, ClauseResult& intResult) {
            std::pair<Argument, Argument> args = clause.getArgs();
            std::string arg1 = args.first.value;
            std::string arg2 = args.second.value;

            if (args.first.type == ArgType::IDENTIFIER) {
                if (arg1 != arg2) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
            } else {
                if (std::stoi(arg1) != std::stoi(arg2)) {
                    intResult.rows.clear();
                    intResult.trueResult = false;
                }
            }

        }

        void evaluateWithClause(PKB::PKB& database, WithClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult) {

            switch (clause.getWithType()) {
            case WithType::IDENTIFIER_EQUAL:
                evaluateIdentifierEqual(database, clause, synonymTable, intResult);
                break;
            case WithType::INTEGER_EQUAL:
                evaluateIntegerEqual(database, clause, synonymTable, intResult);
                break;
            case WithType::LITERAL_EQUAL:
                evaluateLiteralEqual(clause, intResult);
                break;
            }
        }

    }
}
