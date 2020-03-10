#pragma once

#include "Types.h"
#include "PKB.h"
#include "PQL.h"

namespace PQL {
    namespace WhilePatternEvaluator {

        /**
        * Evaluates a single while pattern clause on the given PKB.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateWhilePatternClause(PKB::PKB& database, PatternClause clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable);

    }
}