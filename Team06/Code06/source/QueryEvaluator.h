#pragma once

#include "PQLTypes.h"

#include "PKB.h"

namespace PQL {

    class QueryEvaluator {
    public:
        QueryEvaluator(PKB::PKB database);
    private:
        PKB::PKB database;
    };
    
}
