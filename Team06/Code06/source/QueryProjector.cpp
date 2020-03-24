#include "PQLTypes.h"
#include "QueryProjector.h"

namespace PQL {

    void QueryProjector::formatResult(ClauseResult& result, PQL::Query& query, std::list<std::string>& resultList) {
        // Use a set to remove duplicates
        std::unordered_set<std::string> uniqueResultSet;

        for (ClauseResultEntry& resultEntry : result) {
            std::stringstream s;
            bool first = true;
            for (Ref& targetEntitity : query.targetEntities) {
                if (first) {
                    first = false;
                } else {
                    s << " ";
                }
                if (targetEntitity.second == AttrType::PROC_NAME) {
                    s << resultEntry[targetEntitity.first + ".procName"];
                } else if (targetEntitity.second == AttrType::VAR_NAME) {
                    s << resultEntry[targetEntitity.first + ".varName"];
                } else if (targetEntitity.second == AttrType::STMT_NUM) {
                    s << resultEntry[targetEntitity.first + ".stmt#"];
                } else if (targetEntitity.second == AttrType::VALUE) {
                    s << resultEntry[targetEntitity.first + ".value"];
                } else {
                    s << resultEntry[targetEntitity.first];
                }
            }
            uniqueResultSet.insert(s.str());
        }

        for (auto itr3 = uniqueResultSet.begin(); itr3 != uniqueResultSet.end(); ++itr3) {
            resultList.push_back(*itr3);
        }

        return;
    }
}
