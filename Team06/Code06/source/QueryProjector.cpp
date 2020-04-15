#include "PQLTypes.h"
#include "QueryProjector.h"

namespace PQL {

    void QueryProjector::formatResult(ClauseResult& result, PQL::Query& query, std::list<std::string>& resultList) {
        // Use a set to remove duplicates
        std::unordered_set<std::string> uniqueResultSet;

        if (query.returnsBool) {
            resultList.push_back(result.rows[0][0]);
            return;
        }

        for (ClauseResultEntry& resultEntry : result.rows) {
            std::stringstream s;
            bool first = true;
            for (std::string result : resultEntry) {
                if (first) {
                    first = false;
                } else {
                    s << " ";
                }
                s << result;
            }
            uniqueResultSet.insert(s.str());
        }

        for (auto itr3 = uniqueResultSet.begin(); itr3 != uniqueResultSet.end(); ++itr3) {
            resultList.push_back(*itr3);
        }

        return;
    }
}
