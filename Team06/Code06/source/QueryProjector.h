#pragma once

#include "Types.h"
#include "PQL.h"

namespace PQL {

    /**
     *  The QueryProjector takes the results from the QueryEvaluator
     *  and formats it into the desired output by the SPA. In this case,
     *  std::list<std::string>
     */
    class QueryProjector {
    public:
        /**
          *  Constructor for QueryProjector class to format result from QueryResult
          *
          *  @param      result    ClauseResult returned by QueryEvaluator.
          */
        QueryProjector(ClauseResult &result);

        /**
          *  Constructor for QueryProjector class to format result from QueryResult
          *
          *  @return      List of strings containing query result.
          */
        std::list<std::string> getFormattedResult();

    private:
        ClauseResult result;
    };

}
