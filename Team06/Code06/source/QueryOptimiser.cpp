#include "QueryOptimiser.h"

namespace PQL {

    namespace QueryOptimiser {

        std::vector<ClauseGroup> optimiseQuery(Query& query) {

            // Create list of all clauses
            std::vector<Clause&> clauses;
            for (Clause& clause : query.relations) {
                clauses.emplace_back(clause);
            }
            for (Clause& clause : query.patterns) {
                clauses.emplace_back(clause);
            }
            for (Clause& clause : query.equalities) {
                clauses.emplace_back(clause);
            }

            // Construct adjacency list
            std::vector<std::vector<Clause&> > graph(clauses.size(), std::vector<Clause&>());
            
            for (int i = 0; i < static_cast<int>(clauses.size()); i++) {
                for (int j = 0; j < static_cast<int>(clauses.size()); j++) {
                    // Connect with an edge if the two clauses share any synonyms
                    
                }
            }

            
            
            

        }

    }
}