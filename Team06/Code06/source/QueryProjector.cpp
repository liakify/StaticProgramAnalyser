#include "QueryProjector.h"
#include <string>

namespace PQL {

    QueryProjector::QueryProjector(ClauseResult &result) {
        this->result = result;
    }
    
    std::list<std::string> QueryProjector::getFormattedResult() {
        std::list<std::string> formattedResult;

        for (auto itr1 = this->result.begin();
              itr1 != this->result.end(); ++itr1) {
            for (auto itr2 =*itr1.begin(); itr2 != *itr1.end(); ++ itr2) {
              formattedResult.push_back(std::to_string(*itr2->second));
            }
        }

        formattedResult.unique();
        return formattedResult;
    }
}
