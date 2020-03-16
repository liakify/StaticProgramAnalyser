#include <string>
#include <unordered_map>
#include <unordered_set>

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
        std::unordered_set<std::pair<StmtId, std::string> > getStmtDataPairs(PKB::PKB database, StmtType stmtType) {
            std::unordered_set<std::pair<StmtId, std::string> > result;

            if (stmtType == StmtType::CALL) {
                std::unordered_set<StmtId> callStmts = database.stmtTable.getStmtsByType(StmtType::CALL);
                for (StmtId callStmtId : callStmts) {
                    SIMPLE::CallStmt* callStmt = dynamic_cast<SIMPLE::CallStmt*>(database.stmtTable.get(callStmtId).get());
                    result.insert(std::make_pair(callStmtId, callStmt->getProc()));
                }
                return result;
            } else if (stmtType == StmtType::READ) {
                std::unordered_set<StmtId> readStmts = database.stmtTable.getStmtsByType(StmtType::READ);
                for (StmtId readStmtId : readStmts) {
                    SIMPLE::ReadStmt* readStmt = dynamic_cast<SIMPLE::ReadStmt*>(database.stmtTable.get(readStmtId).get());
                    result.insert(std::make_pair(readStmtId, database.varTable.get(readStmt->getVar())));
                }
                return result;
            } else if (stmtType == StmtType::PRINT) {
                std::unordered_set<StmtId> printStmts = database.stmtTable.getStmtsByType(StmtType::PRINT);
                for (StmtId printStmtId : printStmts) {
                    SIMPLE::PrintStmt* printStmt = dynamic_cast<SIMPLE::PrintStmt*>(database.stmtTable.get(printStmtId).get());
                    result.insert(std::make_pair(printStmtId, database.varTable.get(printStmt->getVar())));
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
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateIdentifierEqual(PKB::PKB& database, WithClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            std::pair<ArgType, Ref> arg1 = clause.getArgs().first;
            std::pair<ArgType, Ref> arg2 = clause.getArgs().second;

            if (arg1.first == ArgType::IDENTIFIER && arg2.first == ArgType::ATTRIBUTE) {
                // Always ensure that arg1 is the attribute
                std::swap(arg1, arg2);
            }

            ArgType argType1 = arg1.first;
            ArgType argType2 = arg2.first;

            if (argType1 == ArgType::ATTRIBUTE && argType2 == ArgType::IDENTIFIER) {
                // Case 1: LHS is an attribute, RHS is an identifier
                
                Synonym syn1 = arg1.second.first;

                if (synonymTable[syn1] == DesignEntity::CALL || synonymTable[syn1] == DesignEntity::READ || synonymTable[syn1] == DesignEntity::PRINT) {
                    ClauseResult clauseResult;

                    std::unordered_set<std::pair<StmtId, ProcName> > stmts =
                        getStmtDataPairs(database, SPA::TypeUtils::getStmtTypeFromDesignEntity(synonymTable[syn1]));
                    for (std::pair<StmtId, std::string> stmt : stmts) {
                        if (stmt.second == arg2.second.first) {
                            ClauseResultEntry resultEntry;
                            resultEntry[syn1] = std::to_string(stmt.first);
                            clauseResult.emplace_back(resultEntry);
                        }
                    }
                    return clauseResult;

                } else {
                    Synonym syn1 = arg1.second.first;
                    ClauseResultEntry resultEntry;
                    resultEntry[syn1] = arg2.second.first;

                    return { resultEntry };
                }

            } else if (argType1 == ArgType::ATTRIBUTE && argType2 == ArgType::ATTRIBUTE) {
                // Case 2: Both LHS and RHS are attributes

                std::unordered_set<std::pair<StmtId, std::string> > result1;
                std::unordered_set<std::pair<StmtId, std::string> > result2;

            } else {
                SPA::LoggingUtils::LogErrorMessage("WithEvaluator::evaluateIdentifierEqual: Invalid ArgTypes for identifier With clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
            }
        }

        /**
        * Evaluates a single With clause where the inputs are two integers.
        *
        * @param    database        The PKB to evaluate the clause on.
        * @param    clause          The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateIntegerEqual(PKB::PKB& database, WithClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {
            
            std::pair<ArgType, Ref> arg1 = clause.getArgs().first;
            std::pair<ArgType, Ref> arg2 = clause.getArgs().second;

            if (arg1.first == ArgType::INTEGER && (arg2.first == ArgType::ATTRIBUTE || arg2.first == ArgType::SYNONYM)) {
                // Always ensure that arg1 is the attribute
                std::swap(arg1, arg2);
            }

            ArgType argType1 = arg1.first;
            ArgType argType2 = arg2.first;

            if ((argType1 == ArgType::ATTRIBUTE || argType1 == ArgType::SYNONYM) && argType2 == ArgType::INTEGER) {
                // Case 1: LHS is a synonym, RHS is an integer
                ClauseResultEntry resultEntry;
                resultEntry[arg1.second.first] = arg2.second.first;
                return { resultEntry };

            } else if ((argType1 == ArgType::ATTRIBUTE || argType1 == ArgType::SYNONYM) && (argType2 == ArgType::ATTRIBUTE || argType2 == ArgType::SYNONYM)) {
                // Case 2: Both LHS and RHS are synonyms
                std::unordered_set<StmtId> result1;
                std::unordered_set<StmtId> result2;

                if (synonymTable[arg1.second.first] == DesignEntity::STATEMENT || synonymTable[arg1.second.first] == DesignEntity::PROG_LINE) {
                    for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                        result1.insert(i);
                    }
                } else {
                    result1 = database.stmtTable.getStmtsByType(SPA::TypeUtils::getStmtTypeFromDesignEntity(synonymTable[arg1.second.first]));
                }
                if (synonymTable[arg1.second.first] == DesignEntity::STATEMENT || synonymTable[arg1.second.first] == DesignEntity::PROG_LINE) {
                    for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                        result2.insert(i);
                    }
                } else {
                    result2 = database.stmtTable.getStmtsByType(SPA::TypeUtils::getStmtTypeFromDesignEntity(synonymTable[arg2.second.first]));
                }

                ClauseResult clauseResult;
                for (StmtId stmt : result1) {
                    if (result2.find(stmt) != result2.end()) {
                        ClauseResultEntry resultEntry;
                        resultEntry[arg1.second.first] = std::to_string(stmt);
                        resultEntry[arg2.second.first] = std::to_string(stmt);
                        clauseResult.emplace_back(resultEntry);
                    }
                }

                return clauseResult;
            } else {
                SPA::LoggingUtils::LogErrorMessage("WithEvaluator::evaluateIntegerEqual: Invalid ArgTypes for integer With clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
            }
        }

        /**
        * Evaluates a single With clause where the inputs are two literals.
        *
        * @param    clause          The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateLiteralEqual(WithClause clause) {
            std::string arg1 = clause.getArgs().first.second.first;
            std::string arg2 = clause.getArgs().second.second.first;

            if (arg1 == arg2) {
                ClauseResultEntry resultEntry;
                resultEntry["_RESULT"] = "TRUE";
                return { resultEntry };
            } else {
                return {};
            }
        }

        ClauseResult evaluateWithClause(PKB::PKB& database, WithClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            switch (clause.type) {
            case WithType::IDENTIFIER_EQUAL:
                return evaluateIdentifierEqual(database, clause, synonymTable);
                break;
            case WithType::INTEGER_EQUAL:
                return evaluateIntegerEqual(database, clause, synonymTable);
                break;
            case WithType::LITERAL_EQUAL:
                return evaluateLiteralEqual(clause);
                break;
            }
        }

    }
}
