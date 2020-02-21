#include "QueryProjector.h"
#include <set>
#include <sstream>

namespace PQL {
    
    void QueryProjector::formatResult(ClauseResult& result, std::list<std::string>& resultList) {
        // Use a set to remove duplicates
        std::set<std::string> uniqueResultSet;

        for (auto itr1 = result.begin(); itr1 != result.end(); ++itr1) {
            ClauseResultEntry clauseResultEntry = *itr1;
            for (auto itr2 = clauseResultEntry.begin(); itr2 != clauseResultEntry.end(); ++itr2) {
                std::stringstream s;
                s << itr2->second;
                uniqueResultSet.insert(s.str());
            }
        }

        for (auto itr3 = uniqueResultSet.begin(); itr3 != uniqueResultSet.end(); ++itr3){
            resultList.push_back(*itr3);
        }
        
        return;
    }
}
