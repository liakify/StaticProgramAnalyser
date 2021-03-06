#include "LoggingUtils.h"
#include "PQL.h"

using std::string;
using std::list;

namespace PQL {

    PQLManager::PQLManager(PKB::PKB& database)
        : parser(), evaluator(QueryEvaluator(database)), formatter() {
    }

    PQLManager::PQLManager(PQL::QueryParser& parserStub, PQL::QueryEvaluator& evalStub)
        : parser(parserStub), evaluator(evalStub), formatter() {
    }

    void PQLManager::evaluateQuery(string queryString, std::list<string>& resultList) {

        // Validate then parse PQL query string into its query struct representation
        Query query = this->parser.parseQuery(queryString);
        if (query.status != STATUS_SUCCESS) {
            // Terminate early if parsing or validation failed
            SPA::LoggingUtils::LogErrorMessage(query.status);

            // Return FALSE for queries with BOOLEAN return type
            if (query.returnsBool && query.status.find("semantic error") != string::npos) {
                resultList.push_back("FALSE");
            }
            return;
        }

        // Evaluate the given query struct against the PKB
        ClauseResult rawResults = this->evaluator.evaluateQuery(query);

        // Process the list of results into a single output string
        this->formatter.formatResult(rawResults, query, resultList);

        return;
    }

}