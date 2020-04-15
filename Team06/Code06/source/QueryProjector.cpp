#include "PQLTypes.h"
#include "QueryProjector.h"

namespace PQL {

    void QueryProjector::formatResult(ClauseResult& result, PQL::Query& query, std::list<std::string>& resultList) {
        // Use a set to remove duplicates
        std::unordered_set<std::string> uniqueResultSet;

        if (query.returnsBool) {
            resultList.push_back(result[0]["_BOOLEAN"]);
            return;
        }

        for (ClauseResultEntry& resultEntry : result) {
            std::stringstream s;
            bool first = true;
            for (ReturnType& targetEntity : query.targetEntities) {
                if (first) {
                    first = false;
                } else {
                    s << " ";
                }
                if (targetEntity.attrType == AttrType::PROC_NAME) {
                    s << resultEntry[targetEntity.synonym + ".procName"];
                } else if (targetEntity.attrType == AttrType::VAR_NAME) {
                    s << resultEntry[targetEntity.synonym + ".varName"];
                } else if (targetEntity.attrType == AttrType::STMT_NUM) {
                    s << resultEntry[targetEntity.synonym + ".stmt#"];
                } else if (targetEntity.attrType == AttrType::VALUE) {
                    s << resultEntry[targetEntity.synonym + ".value"];
                } else {
                    s << resultEntry[targetEntity.synonym];
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
