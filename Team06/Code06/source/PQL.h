#pragma once

#include <list>
#include <string>

#include "PKB.h"
#include "PQLTypes.h"
#include "QueryEvaluator.h"
#include "QueryParser.h"
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
         *  Constructor for PQLManager facade class.
         *
         *  @param      database    populated PKB data store after SIMPLE program parsing.
         */
        PQLManager(PKB::PKB& database);

        /**
         *  Overloaded constructor for PQLManager facade class, for unit testing purposes.
         *
         *  @param      parserStub  reference to a QueryParser instance or a stub.
         *  @param      evalStub    reference to a QueryEvaluator instance or a stub.
         */
        PQLManager(PQL::QueryParser& parserStub, PQL::QueryEvaluator& evalStub);

        /**
         *  Accepts an empty list and a PQL query string for parsing and evaluation against the populated
         *  PKB for a parsed SIMPLE program. The query results are inserted into the empty list.
         *
         *  @param      queryString     input PQL query string.
         *  @param      resultList      list to populate the query results with.
         */
        void evaluateQuery(std::string queryString, std::list<std::string>& resultList);
     private:
        QueryParser parser;
        QueryEvaluator evaluator;
        QueryProjector formatter;
    };

}