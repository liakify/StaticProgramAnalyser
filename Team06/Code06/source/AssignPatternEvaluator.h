#pragma once

#include <string>
#include <unordered_map>

#include "Types.h"
#include "PKB.h"
#include "PQL.h"

namespace PQL {
    namespace AssignPatternEvaluator {

        /**
        * Evaluates a single assign pattern clause on the given PKB.
        *
        * @param    database    The PKB to evaluate the clause on.
        * @param    clause      The clause to evaluate.
        * @param    synonymTable    The synonym table associated with the query containing the clause.
        * @param    intResult       The intermediate result table.
        * @return   The result of the evaluation.
        */
        ClauseResult evaluateAssignPatternClause(PKB::PKB& database, PatternClause clause,
            std::unordered_map<std::string, DesignEntity>& synonymTable, ClauseResult& intResult);

    }
}