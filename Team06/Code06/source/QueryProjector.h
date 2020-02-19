#pragma once

#include "Types.h"

namespace PQL {

    /**
     *  The QueryProjector takes the results from the QueryEvaluator
     *  and formats it into the desired output by the SPA. In this case,
     *  std::list<std::string>
     */
    class QueryProjector {
    public:
        /**
         *  Empty constructor for the QueryProjector class.
         */
        QueryProjector();

        /**
         *  Update the query result being held by QueryProjector.
         * 
         *  @param      result      ClauseResult from QueryEvaluator.
         */
        void updateResult(ClauseResult &result);

        /**
          *  Format result (ClauseResult) received from QueryResult.
          *
          *  @return      List of strings containing query result.
          */
        std::list<std::string> getFormattedResult();

    private:
        ClauseResult result;
    };

}
