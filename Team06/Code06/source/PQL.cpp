#include "LoggingUtils.h"
#include "PQL.h"

namespace PQL {

    PQLManager::PQLManager(PKB::PKB& database) 
        : evaluator(QueryEvaluator(database)) {
        this->parser = QueryParser();
        this->formatter = QueryProjector();
    }

    void PQLManager::evaluateQuery(string queryString, list<string>& resultList) {

        // Validate then parse PQL query string into its query struct representation
        Query query = this->parser.parseQuery(queryString);
        if (query.status != "success") {
            // Terminate early if parsing or validation failed
            SPA::LoggingUtils::LogErrorMessage(query.status);
            return;
        }

        // Evaluate the given query struct against the PKB
        ClauseResult rawResults = this->evaluator.evaluateQuery(query);

        // Process the list of results into a single output string
        this->formatter.formatResult(rawResults, resultList);

        return;
    }
    
}