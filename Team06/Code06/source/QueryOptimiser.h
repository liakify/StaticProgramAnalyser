#pragma once
#include "PQLTypes.h"

namespace PQL {

    namespace QueryOptimiser {
        
        /**
         * Optimises a given query for evaluation.
         * 
         * @param   query   The query to optimise
         * @return  A list of clause groups, where the clauses and groups are in the
         *          order to be evaluated.
         */
        std::vector<ClauseGroup> optimiseQuery(Query& query);

    };

}