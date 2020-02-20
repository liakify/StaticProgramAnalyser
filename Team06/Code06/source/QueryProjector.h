#pragma once

#include "Types.h"

namespace PQL {

    /**
     *  The QueryProjector takes the results from the QueryEvaluator and formats it
     *  into the required output by the SPA. In this case, a std::list<std::string>.
     */
    class QueryProjector {
    public:
        /**
         *  Populates an input list of strings with results from the ClauseResult held
         *  by QueryProjector.
         *
         *  @param      resultList  empty list of strings to populate with query results.
         */
        void formatResult(ClauseResult& results, std::list<std::string>& resultList);
    };

}
