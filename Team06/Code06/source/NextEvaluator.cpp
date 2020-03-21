#include <string>
#include <unordered_map>
#include <unordered_set>

#include "NextEvaluator.h"
#include "LoggingUtils.h"
#include "TypeUtils.h"

namespace PQL {
    namespace NextEvaluator {

        /**
        * Evaluates a single Next clause on the given PKB where the inputs are two StmtIds.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateNextClauseIntInt(PKB::PKB& database, RelationClause clause) {
            StmtId arg1 = std::stoi(clause.getArgs().first.second);
            StmtId arg2 = std::stoi(clause.getArgs().second.second);

            if (database.next(arg1, arg2)) {
                ClauseResultEntry resultEntry;
                resultEntry["_RESULT"] = "TRUE";
                return { resultEntry };
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single Next clause on the given PKB where the inputs are two wildcards.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateNextClauseWildWild(PKB::PKB& database) {
            if (database.hasNextRelation()) {
                ClauseResultEntry resultEntry;
                resultEntry["_RESULT"] = "TRUE";
                return { resultEntry };
            } else {
                return {};
            }
        }

        /**
        * Evaluates a single Next clause on the given PKB where the inputs contain one StmtId and one wildcard.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateNextClauseIntWild(PKB::PKB& database, RelationClause clause) {

            ArgType argType1 = clause.firstStmt.first;
            ArgType argType2 = clause.secondStmt.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD) {
                // Case 1: Integer, Wildcard
                StmtId arg1 = std::stoi(clause.getArgs().first.second);
                if (database.hasNext(arg1)) {
                    ClauseResultEntry resultEntry;
                    resultEntry["_RESULT"] = "TRUE";
                    return { resultEntry };
                } else {
                    return {};
                }
            } else {
                // Case 2: Wildcard, Integer
                StmtId arg2 = std::stoi(clause.getArgs().second.second);
                if (database.hasPrev(arg2)) {
                    ClauseResultEntry resultEntry;
                    resultEntry["_RESULT"] = "TRUE";
                    return { resultEntry };
                } else {
                    return {};
                }
            }
        }

        ClauseResult evaluateNextClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable) {

            ArgType argType1 = clause.getArgs().first.first;
            ArgType argType2 = clause.getArgs().second.first;

            if (argType1 == ArgType::INTEGER && argType2 == ArgType::INTEGER) {
                // Two statement numbers supplied
                return evaluateNextClauseIntInt(database, clause);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::WILDCARD) {
                // Two wildcards supplied
                return evaluateNextClauseWildWild(database);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::WILDCARD ||
                argType1 == ArgType::WILDCARD && argType2 == ArgType::INTEGER) {
                // One statement number, one wildcard supplied
                return evaluateNextClauseIntWild(database, clause);
            } else if (argType1 == ArgType::INTEGER && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::INTEGER) {
                // One statement number, one synonym
                return evaluateNextClauseIntSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::WILDCARD && argType2 == ArgType::SYNONYM ||
                argType1 == ArgType::SYNONYM && argType2 == ArgType::WILDCARD) {
                // One synonym, one wildcard
                return evaluateNextClauseWildSyn(database, clause, synonymTable);
            } else if (argType1 == ArgType::SYNONYM && argType2 == ArgType::SYNONYM) {
                // Two synonyms
                return evaluateNextClauseSynSyn(database, clause, synonymTable);
            } else {
                SPA::LoggingUtils::LogErrorMessage("NextEvaluator::evaluateNextClause: Invalid ArgTypes for Next clause. argType1 = %d, argType2 = %d\n", argType1, argType2);
                return {};
            }
        }

    }
}
