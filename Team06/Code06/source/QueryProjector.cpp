#include "PQLTypes.h"
#include "QueryProjector.h"

namespace PQL {

    void QueryProjector::formatResult(ClauseResult& result, PQL::Query& query, std::list<std::string>& resultList) {

        std::unordered_set<std::string> uniqueResultSet;

        if (query.returnsBool) {
            resultList.push_back(result.rows[0][0]);
            return;
        }

        for (ClauseResultEntry& resultEntry : result.rows) {
            std::string s;
            bool first = true;
            for (std::string result : resultEntry) {
                if (first) {
                    first = false;
                } else {
                    s.append(" ");
                }
                s.append(result);
            }
            uniqueResultSet.insert(s);
        }

        for (auto itr3 = uniqueResultSet.begin(); itr3 != uniqueResultSet.end(); ++itr3) {
            resultList.push_back(*itr3);
        }

        return;
    }
}
