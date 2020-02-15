#pragma once

#include "PQLTypes.h"

#include "PKB.h"
#include "QueryParser.h"
#include "QueryEvaluator.h"
#include "QueryProjector.h"

namespace PQL {

    class PQLManager {
    public:
        PQLManager(PKB::PKB database);
        string evaluateQuery(string queryString);
    private:
        static QueryParser parser;
        static QueryEvaluator evaluator;
        static QueryProjector formatter;
    };

}
