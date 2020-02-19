#include "QueryProjector.h"
#include <set>
#include <sstream>

namespace PQL {

    void QueryProjector::updateResult(ClauseResult &result) {
        this->result = result;
    }
    
    std::list<std::string> QueryProjector::getFormattedResult() {
        //Use set to remove duplicates
        std::set<std::string> uniqueResultSet;

        for (auto itr1 = this->result.begin(); itr1 != this->result.end(); ++itr1) {
            ClauseResultEntry clauseResultEntry = *itr1;
            for (auto itr2 = clauseResultEntry.begin(); itr2 != clauseResultEntry.end(); ++itr2) {
                std::stringstream s;
                s << itr2->second;
                uniqueResultSet.insert(s.str());
            }
        }

        std::list<std::string> formattedResult;
        for (auto itr3 = uniqueResultSet.begin(); itr3 != uniqueResultSet.end(); ++itr3){
            formattedResult.push_back(*itr3);
        }
        return formattedResult;
    }
}
