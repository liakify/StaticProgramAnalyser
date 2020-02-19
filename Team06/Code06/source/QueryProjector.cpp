#include "QueryProjector.h"
#include <sstream>

namespace PQL {

    void QueryProjector::updateResult(ClauseResult &result) {
        this->result = result;
    }
    
    std::list<std::string> QueryProjector::getFormattedResult() {
        std::list<std::string> formattedResult;

        for (auto itr1 = this->result.begin(); itr1 != this->result.end(); ++itr1) {
            ClauseResultEntry clauseResultEntry = *itr1;
            for (auto itr2 = clauseResultEntry.begin(); itr2 != clauseResultEntry.end(); ++itr2) {
              std::stringstream s;
              s << itr2->second;
              formattedResult.push_back(s.str());
            }
        }

        formattedResult.unique();
        return formattedResult;
    }
}
