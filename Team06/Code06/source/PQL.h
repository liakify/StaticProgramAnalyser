#pragma once

#include "PQLTypes.h"

#include "PKB.h"
#include "QueryParser.h"
#include "QueryEvaluator.h"
#include "QueryProjector.h"

namespace PQL {

    /**
     *  The PQLManager class is the facade for the entire Query Procesor component.
     *  It relies on its sub-components (Query Parser, Query Evaluator and Query
     *  Projector) in order to serve incoming PQL queries.
     */
    class PQLManager {
    public:
        /**
         *  Constructor for PQLManager facade class to handle PQL queries.
         *
         *  @param      database    populated PKB data store after SIMPLE program parsing.
         */
        PQLManager(PKB::PKB& database);

        /**
         *  Accepts a PQL query string for parsing and evaluation against the populated
         *  PKB for a parsed SIMPLE progrma.
         *
         *  @param      queryString     input PQL query string.
         *  @return     string of space-delimited query results.
         */
        string evaluateQuery(string queryString);
    private:
        static QueryParser parser;
        static QueryEvaluator evaluator;
        static QueryProjector formatter;
    };

}
