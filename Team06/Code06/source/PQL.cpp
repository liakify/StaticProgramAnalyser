#include "PQL.h"

namespace PQL {

    PQLManager::PQLManager(PKB::PKB& database) {
        this->parser = QueryParser();
        this->evaluator = QueryEvaluator(database);
        this->formatter = QueryProjector();
    }

    string PQLManager::evaluateQuery(string queryString) {

        // Validate then parse PQL query string into its query struct representation
        Query query = this->parser.parseQuery(queryString);
        if (query.status != "success") {
            return query.status;
        }

        // Evaluate the given query struct against the PKB
        // vector<string> rawResults = this->evaluator.parseQuery(query);

        // Process the list of results into a single output string
        // return this->formatter.format(rawResults);

        return "None";
    }
    
}
