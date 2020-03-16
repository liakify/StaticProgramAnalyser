#include <string>
#include <unordered_map>
#include <unordered_set>

#include "WithEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace WithEvaluator {

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

            if (arg1.first == ArgType::ATTRIBUTE && arg2.first == ArgType::SYNONYM) {
                // Case 1: LHS is a synonym, RHS is an integer

            } else if (arg1.first == ArgType::SYNONYM && arg2.first == ArgType::SYNONYM) {
                // Case 2: Both LHS and RHS are synonyms

            } else {
                SPA::LoggingUtils::LogErrorMessage("WithEvaluator::evaluateIntegerEqual: Invalid ArgTypes for integer With clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
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
            
            if ((arg1.first == ArgType::ATTRIBUTE || arg1.first == ArgType::SYNONYM) && arg2.first == ArgType::INTEGER) {
                // Case 1: LHS is a synonym, RHS is an integer
                ClauseResultEntry resultEntry;
                resultEntry[arg1.second.first] = arg2.second.first;
                return { resultEntry };

            } else if ((arg1.first == ArgType::ATTRIBUTE || arg1.first == ArgType::SYNONYM) && (arg2.first == ArgType::ATTRIBUTE || arg2.first == ArgType::SYNONYM)) {
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
