#include "QueryProjector.h"
#include <string>
#include "assert.h"

namespace PQL {

    void QueryProjector::updateResult(ClauseResult &result) {
        this->result = result;
    }
    
    std::list<std::string> QueryProjector::getFormattedResult() {
        std::list<std::string> formattedResult;

        for (auto itr1 = this->result.begin(); itr1 != this->result.end(); ++itr1) {
            ClauseResultEntry clauseResultEntry = *itr1;
            for (auto itr2 = clauseResultEntry.begin(); itr2 != clauseResultEntry.end(); ++itr2) {
              auto value = itr2->second;
              formattedResult.push_back(std::to_string(value));
            }
        }

        formattedResult.unique();
        return formattedResult;
    }
}
