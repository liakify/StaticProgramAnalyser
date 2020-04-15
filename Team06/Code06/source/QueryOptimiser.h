#pragma once
#include "PQLTypes.h"

namespace PQL {

    namespace QueryOptimiser {

        /**
         * Optimises a given query for evaluation.
         * 
         * @param   query   The query to optimise
         * @return  The optimised query.
         */
        OptimisedQuery optimiseQuery(Query& query);

    };

}