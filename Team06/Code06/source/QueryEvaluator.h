#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "PKB.h"
#include "PQLTypes.h"
#include "Types.h"

namespace PQL {

    class RelationClause;
    class PatternClause;
    struct Query;

    class QueryEvaluator {

     public:
        /**
        * Constructor for a QueryEvaluator.
        * The QueryEvaluator will evaluate queries based on the given PKB.
        *
        * @param    database    The PKB to evaluate queries on
        */
        QueryEvaluator(PKB::PKB &database);

        /**
        * Evaluates a single PQL query.
        *
        * @param    query   The query to be evaluated.
        * @return   The results of the evaluation
        */
        ClauseResult evaluateQuery(Query &query);

     private:
        PKB::PKB database;

        /**
        * Evaluates a single relation clause.
        *
        * @param    relationClause  The relation clause to evaluate
        * @param    synonymTable    The synonym table associated with the query.
        * @param    intResult       The intermediate result table for the group.
        * @return   The results of the evaluation
        */
        ClauseResult evaluateRelationClause(RelationClause &relationClause,
            std::unordered_map<std::string, DesignEntity> &synonymTable,
            ClauseResult& intResult);

        /**
        * Evaluates a single pattern clause
        *
        * @param    patternClause   The pattern clause to evaluate
        * @param    synonymTable    The synonym table associated with the query.
        * @param    intResult       The intermediate result table for the group.
        * @return   The results of the evaluation
        */
        ClauseResult evaluatePatternClause(PatternClause &patternClause,
            std::unordered_map<std::string, DesignEntity> &synonymTable,
            ClauseResult& intResult);

        /**
        * Evaluates a single with clause
        *
        * @param    withClause      The with clause to evaluate
        * @param    synonymTable    The synonym table associated with the query.
        * @param    intResult       The intermediate result table for the group.
        * @return   The results of the evaluation
        */
        ClauseResult evaluateWithClause(WithClause& withClause,
            std::unordered_map<std::string, DesignEntity>& synonymTable,
            ClauseResult& intResult);

        /**
        * Summarizes the combined results of the clauses into the results needed to answer the given query.
        *
        * @param    query   The query to evaluate.
        * @param    combinedResults The combined results of all the clauses.
        * @return   The results necessary to answer the given query.
        */
        ClauseResult extractQueryResults(Query &query, ClauseResult &combinedResults);

        /**
        * Combines two clause result entries that have identical values for all common synonyms.
        *
        * @param    entry1  The first result entry.
        * @param    entry2  The second result entry.
        * @param    combStruct  The combined structure of the two result entries.
        * @return   The combined result entries.
        */
        ClauseResultEntry combineTwoClauseResultEntries(ClauseResultEntry& entry1, ClauseResultEntry& entry2,
            std::vector<std::pair<int, int> >& combStruct);

        /**
        * Checks if two clause result entries have identical values for all common synonyms.
        *
        * @param    entry1  The first result entry.
        * @param    entry2  The second result entry.
        * @param    commonSynonyms  The synonyms that are common between the two entries.
        * @return   True if all all common synonyms have identical values, false otherwise.
        */
        bool checkCommonSynonyms(ClauseResultEntry& entry1, ClauseResultEntry& entry2,
            std::vector<std::pair<int, int> >& commonSynonyms);

        /**
        * Combines two sets of clause result entries.
        * 
        * @param    clauseResults1  The first set of clause result entries.
        * @param    clauseResults2  The first set of clause result entries.
        * @param    toDelete        Synonyms to be removed from the combined clause result.
        * @return   The combined set of clause result entries.
        */
        ClauseResult combineTwoClauseResults(ClauseResult clauseResults1, ClauseResult clauseResults2, std::vector<Synonym> toDelete);

        /**
        * Combines a list of clause results.
        * 
        * @param    clauseResults   The list of clause results to combine.
        * @return   The combined clause result.
        */
        ClauseResult combineClauseResults(std::vector<ClauseResult> clauseResults);

        /**
        * Constructs a ClauseResult structure with all values of a DesignEntity for a given synonym.
        *
        * @param    synonym         The synonym to construct the ClauseResult for.
        * @param    designEntity    The DesignEntity of the synonym
        * @return   The construct ClauseResult structure.
        */
        ClauseResult getClauseResultWithAllValues(Synonym synonym, DesignEntity designEntity);
    };
}
