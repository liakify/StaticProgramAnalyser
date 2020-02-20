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
         *  Accepts an empty list and a PQL query string for parsing and evaluation against the populated
         *  PKB for a parsed SIMPLE program. The query results are inserted into the empty list.
         *
         *  @param      queryString     input PQL query string.
         *  @param      resultList      list to populate the query results with.
         */
        void evaluateQuery(string queryString, std::list<string>& resultList);
    private:
        QueryParser parser;
        QueryEvaluator evaluator;
        QueryProjector formatter;
    };

}