#pragma once

#include "Types.h"
#include "PKB.h"
#include "PQL.h"

namespace PQL {
    namespace CallsStarEvaluator {

        /**
        * Evaluates a single Calls* clause on the given PKB.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateCallsStarClause(PKB::PKB& database, RelationClause clause,
            unordered_map<std::string, DesignEntity>& synonymTable);

    }
}