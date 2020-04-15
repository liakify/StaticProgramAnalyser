#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "AffectsEvaluator.h"
#include "AffectsStarEvaluator.h"
#include "AssignPatternEvaluator.h"
#include "CallsEvaluator.h"
#include "CallsStarEvaluator.h"
#include "FollowsEvaluator.h"
#include "FollowsStarEvaluator.h"
#include "IfPatternEvaluator.h"
#include "LoggingUtils.h"
#include "ModifiesEvaluator.h"
#include "NextEvaluator.h"
#include "NextStarEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentStarEvaluator.h"
#include "QueryEvaluator.h"
#include "QueryOptimiser.h"
#include "UsesEvaluator.h"
#include "TypeUtils.h"
#include "WhilePatternEvaluator.h"
#include "WithEvaluator.h"

namespace PQL {

    QueryEvaluator::QueryEvaluator(PKB::PKB &database) {
        this->database = database;
    }

    ClauseResult QueryEvaluator::getClauseResultWithAllValues(Synonym synonym, DesignEntity designEntity) {

        ClauseResult clauseResult;
        clauseResult.syns.emplace_back(synonym);

        switch (designEntity) {
        case DesignEntity::ASSIGN:
        case DesignEntity::CALL:
        case DesignEntity::IF:
        case DesignEntity::PRINT:
        case DesignEntity::READ:
        case DesignEntity::WHILE: {
            std::unordered_set<StmtId> stmts = database.stmtTable.getStmtsByType(SPA::TypeUtils::getStmtTypeFromDesignEntity(designEntity));
            for (StmtId stmt : stmts) {
                ClauseResultEntry resultEntry;
                resultEntry.emplace_back(std::to_string(stmt));
                clauseResult.rows.emplace_back(resultEntry);
            }
            return clauseResult;
            break;
        }
        case DesignEntity::PROG_LINE:
        case DesignEntity::STATEMENT: {
            for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                ClauseResultEntry resultEntry;
                resultEntry.emplace_back(std::to_string(i));
                clauseResult.rows.emplace_back(resultEntry);
            }
            return clauseResult;
            break;
        }
        case DesignEntity::CONSTANT: {
            for (ConstId i = 1; i <= database.constTable.size(); i++) {
                ClauseResultEntry resultEntry;
                resultEntry.emplace_back(database.constTable.get(i));
                clauseResult.rows.emplace_back(resultEntry);
            }
            return clauseResult;
            break;
        }
        case DesignEntity::PROCEDURE: {
            for (ProcId i = 1; i <= database.procTable.size(); i++) {
                ClauseResultEntry resultEntry;
                resultEntry.emplace_back(database.procTable.get(i).getName());
                clauseResult.rows.emplace_back(resultEntry);
            }
            return clauseResult;
            break;
        }
        case DesignEntity::VARIABLE: {
            std::unordered_set<VarName> allVars = database.varTable.getAllVars();
            for (VarName var : allVars) {
                ClauseResultEntry resultEntry;
                resultEntry.emplace_back(var);
                clauseResult.rows.emplace_back(resultEntry);
            }
            return clauseResult;
            break;
        }
        }
        return clauseResult;
    }

    ClauseResult QueryEvaluator::evaluateQuery(Query &query) {

        // Optimise Query
        OptimisedQuery optQuery = QueryOptimiser::optimiseQuery(query);

        // Maintain one ClauseResult structure for each group
        // +1 in case there are no groups at all
        std::vector<ClauseResult> clauseResults = std::vector<ClauseResult>(optQuery.groups.size() + 1);

        // Add table containing "TRUE" to every group
        for (ClauseResult& clauseResult : clauseResults) {
            clauseResult.trueResult = true;
        }

        // Construct set of all target synonyms
        std::unordered_set<Synonym> targetSynonyms;
        for (Ref ref : query.targetEntities) {
            targetSynonyms.insert(ref.first);
        }

        // Evaluate clauses
        for (int i = 0; i < optQuery.clauses.size(); i++) {
            ClauseResult result;
            Clause* clause = optQuery.clauses[i];
            if (clause->getClauseType() == ClauseType::RELATION) {
                RelationClause* relation = static_cast<RelationClause*>(clause);
                result = evaluateRelationClause(*relation, query.synonymTable);
                // Remove all present synonyms from target synonyms
                std::pair<ArgType, std::string> arg1, arg2;
                arg1 = relation->getArgs().first;
                arg2 = relation->getArgs().second;
                if (arg1.first == ArgType::SYNONYM) {
                    targetSynonyms.erase(arg1.second);
                }
                if (arg2.first == ArgType::SYNONYM) {
                    targetSynonyms.erase(arg2.second);
                }
            } else if (clause->getClauseType() == ClauseType::PATTERN) {
                PatternClause* pattern = static_cast<PatternClause*>(clause);
                result = evaluatePatternClause(*pattern, query.synonymTable);
                // Remove all present synonyms from target synonyms
                std::pair<ArgType, std::string> arg1, arg2;
                arg1 = pattern->getArgs().first;
                arg2 = pattern->getArgs().second;
                targetSynonyms.erase(pattern->getSynonym());
                if (arg1.first == ArgType::SYNONYM) {
                    targetSynonyms.erase(arg1.second);
                }
                if (arg2.first == ArgType::SYNONYM) {
                    targetSynonyms.erase(arg2.second);
                }
            } else if (clause->getClauseType() == ClauseType::WITH) {
                WithClause* with = static_cast<WithClause*>(clause);
                result = evaluateWithClause(*with, query.synonymTable);
                // Remove all present synonyms from target synonyms
                std::pair<ArgType, Ref> arg1, arg2;
                arg1 = with->getArgs().first;
                arg2 = with->getArgs().second;
                if (arg1.first == ArgType::SYNONYM) {
                    targetSynonyms.erase(arg1.second.first);
                }
                if (arg2.first == ArgType::SYNONYM) {
                    targetSynonyms.erase(arg2.second.first);
                }
            }

            // If the result is empty, we can stop evaluation immediately
            if (result.rows.empty()) {
                return extractQueryResults(query, ClauseResult());
            }

            // Merge this table with the intermediate table of the corresponding group
            clauseResults[optQuery.groups[i]] = combineTwoClauseResults(result, clauseResults[optQuery.groups[i]]);

            // If the merged table is empty, stop evaluation immediately
            if (clauseResults[optQuery.groups[i]].rows.empty()) {
                return extractQueryResults(query, ClauseResult());
            }
        }

        // Merge a table for each synonym not present in any clause
        for (Synonym synonym : targetSynonyms) {
            clauseResults.emplace_back(getClauseResultWithAllValues(synonym, query.synonymTable[synonym]));
        }

        // Combine all results
        ClauseResult combinedResult = combineClauseResults(clauseResults);

        // Extract necessary results to answer query
        ClauseResult result = extractQueryResults(query, combinedResult);

        return result;
    }

    ClauseResult QueryEvaluator::extractQueryResults(Query &query, ClauseResult& combinedResult) {

        if (query.returnsBool) {
            ClauseResult result;
            ClauseResultEntry resultEntry;
            if (combinedResult.rows.empty()) {
                resultEntry.emplace_back("FALSE");
            } else {
                resultEntry.emplace_back("TRUE");
            }
            result.rows.emplace_back(resultEntry);
            return result;
        } else if (combinedResult.rows.empty()) {
            SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::extractQueryResults: Empty Result!\n");
            return ClauseResult();
        } else {
            std::vector<Ref> &targets = query.targetEntities;
            std::vector<std::pair<Ref, int> > mapper;
            for (int i = 0; i < targets.size(); i++) {
                mapper.emplace_back(std::make_pair(targets[i], i));
            }
            std::sort(mapper.begin(), mapper.end());

            ClauseResult finalResult;
            for (ClauseResultEntry& resultEntry : combinedResult.rows) {
                ClauseResultEntry finalResultEntry(targets.size());
                // Two pointers method to extract results
                int j;
                for (int i = 0; i < mapper.size(); i++) {
                    while (combinedResult.syns[j] != mapper[i].first.first) {
                        j++;
                    }
                    Synonym targetSyn = mapper[i].first.first;
                    AttrType targetAttr = mapper[i].first.second;
                    int position = mapper[i].second;

                    if (targetAttr == AttrType::NONE) {
                        finalResultEntry[position] = resultEntry[j];
                    } else if (targetAttr == AttrType::STMT_NUM) {
                        finalResultEntry[position] = resultEntry[j];
                    } else if (targetAttr == AttrType::VALUE) {
                        finalResultEntry[position] = resultEntry[j];
                    } else if (targetAttr == AttrType::PROC_NAME) {
                        if (query.synonymTable[targetSyn] == DesignEntity::PROCEDURE) {
                            finalResultEntry[position] = resultEntry[j];
                        } else {
                            CallStmt *callStmt = dynamic_cast<CallStmt*>(database.stmtTable.get(std::stoi(resultEntry[j])).get());
                            finalResultEntry[position] = callStmt->getProc();
                        }
                    } else if (targetAttr == AttrType::VAR_NAME) {
                        if (query.synonymTable[targetSyn] == DesignEntity::READ) {
                            ReadStmt* readStmt = dynamic_cast<ReadStmt*>(database.stmtTable.get(std::stoi(resultEntry[j])).get());
                            finalResultEntry[position] = database.varTable.get(readStmt->getVar());
                        } else if (query.synonymTable[targetSyn] == DesignEntity::PRINT) {
                            PrintStmt* printStmt = dynamic_cast<PrintStmt*>(database.stmtTable.get(std::stoi(resultEntry[j])).get());
                            finalResultEntry[position] = database.varTable.get(printStmt->getVar());
                        } else {
                            finalResultEntry[position] = resultEntry[j];
                        }
                    }
                }
                finalResult.rows.emplace_back(finalResultEntry);
            }
            return finalResult;
        }
    }

    ClauseResultEntry QueryEvaluator::combineTwoClauseResultEntries(ClauseResultEntry &entry1, ClauseResultEntry &entry2,
        std::vector<std::pair<int, int> > &combStruct) {

        ClauseResultEntry combinedEntry;
        for (auto entry : combStruct) {
            if (entry.second == 0) {
                combinedEntry.emplace_back(entry1[entry.first]);
            } else {
                combinedEntry.emplace_back(entry2[entry.first]);
            }
        }

        return combinedEntry;
    }

    bool QueryEvaluator::checkCommonSynonyms(ClauseResultEntry &entry1, ClauseResultEntry &entry2,
        std::vector<std::pair<int, int> > &commonSynonyms) {
        for (std::pair<int, int> synonym : commonSynonyms) {
            if (entry1[synonym.first] != entry2[synonym.second]) {
                return false;
            }
        }
        return true;
    }

    ClauseResult QueryEvaluator::combineTwoClauseResults(ClauseResult clauseResults1, ClauseResult clauseResults2) {
        if (clauseResults1.trueResult && clauseResults2.trueResult) {
            return clauseResults1;
        } else if (clauseResults1.trueResult && !clauseResults2.rows.empty()) {
            return clauseResults2;
        } else if (!clauseResults1.rows.empty() && clauseResults2.trueResult) {
            return clauseResults1;
        }

        if (clauseResults1.rows.empty() || clauseResults2.rows.empty()) {
            return clauseResults1;
        }

        // Extract common synonyms and get structure of combined result
        std::vector<std::pair<int, int> > commonSynonyms;
        std::vector<std::pair<int, int> > combStruct;
        int i = 0, j = 0;
        while (i < clauseResults1.syns.size()) {
            while (j < clauseResults2.syns.size() && clauseResults2.syns[j] < clauseResults1.syns[i]) {
                combStruct.emplace_back(std::make_pair(j, 1));
                j++;
            }
            if (j >= clauseResults2.syns.size()) {
                break;
            }
            if (clauseResults1.syns[i] == clauseResults2.syns[j]) {
                commonSynonyms.emplace_back(std::make_pair(i, j));
                combStruct.emplace_back(std::make_pair(i, 0));
                j++;
            }
            combStruct.emplace_back(std::make_pair(i, 0));
            i++;
        }
        while (i < clauseResults1.syns.size()) {
            combStruct.emplace_back(std::make_pair(i, 0));
            i++;
        }
        while (j < clauseResults2.syns.size()) {
            combStruct.emplace_back(std::make_pair(j, 1));
            j++;
        }

        ClauseResult combinedResult;
        // Populate synonym list
        for (int i = 0; i < combStruct.size(); i++) {
            if (combStruct[i].second == 0) {
                combinedResult.syns.emplace_back(clauseResults1.syns[combStruct[i].first]);
            } else {
                combinedResult.syns.emplace_back(clauseResults2.syns[combStruct[i].first]);
            }
        }

        // Perform a Cartesian Product
        for (ClauseResultEntry& entry1 : clauseResults1.rows) {
            for (ClauseResultEntry& entry2 : clauseResults2.rows) {
                if (checkCommonSynonyms(entry1, entry2, commonSynonyms)) {
                    combinedResult.rows.emplace_back(combineTwoClauseResultEntries(entry1, entry2, combStruct));
                }
            }
        }

        return combinedResult;
    }

    ClauseResult QueryEvaluator::combineClauseResults(std::vector<ClauseResult> clauseResults) {
        if (clauseResults.size() == 1) {
            return clauseResults[0];
        }
        if (clauseResults.size() < 1) {
            SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::combineClauseResults: clauseResults empty!");
            return {};
        }
        std::vector<ClauseResult> left;
        std::vector<ClauseResult> right;
        for (int i = 0; i < static_cast<int>(clauseResults.size()) / 2; i++) {
            left.emplace_back(clauseResults[i]);
        }
        for (int i = clauseResults.size() / 2; i < static_cast<int>(clauseResults.size()); i++) {
            right.emplace_back(clauseResults[i]);
        }
        ClauseResult leftResult = combineClauseResults(left);
        ClauseResult rightResult = combineClauseResults(right);
        ClauseResult combinedResults = combineTwoClauseResults(leftResult, rightResult);
        return combinedResults;
    }

    ClauseResult QueryEvaluator::evaluateRelationClause(RelationClause &relationClause,
        std::unordered_map<std::string, DesignEntity> &synonymTable) {
        switch (relationClause.getRelationType()) {
        case RelationType::FOLLOWS:
            return FollowsEvaluator::evaluateFollowsClause(this->database, relationClause, synonymTable);
            break;
        case RelationType::FOLLOWST:
            return FollowsStarEvaluator::evaluateFollowsStarClause(this->database, relationClause, synonymTable);
            break;
        case RelationType::PARENT:
            return ParentEvaluator::evaluateParentClause(this->database, relationClause, synonymTable);
            break;
        case RelationType::PARENTT:
            return ParentStarEvaluator::evaluateParentStarClause(this->database, relationClause, synonymTable);
            break;
        case RelationType::MODIFIESS:
        case RelationType::MODIFIESP:
            return ModifiesEvaluator::evaluateModifiesClause(this->database, relationClause, synonymTable);
            break;
        case RelationType::USESS:
        case RelationType::USESP:
            return UsesEvaluator::evaluateUsesClause(this->database, relationClause, synonymTable);
            break;
        case RelationType::CALLS:
            return CallsEvaluator::evaluateCallsClause(this->database, relationClause, synonymTable);
            break;
        case RelationType::CALLST:
            return CallsStarEvaluator::evaluateCallsStarClause(this->database, relationClause, synonymTable);
            break;
        case RelationType::NEXT:
            return NextEvaluator::evaluateNextClause(this->database, relationClause, synonymTable);
            break;
        case RelationType::NEXTT:
            return NextStarEvaluator::evaluateNextStarClause(this->database, relationClause, synonymTable);
            break;
        case RelationType::AFFECTS:
            return AffectsEvaluator::evaluateAffectsClause(this->database, relationClause, synonymTable);
            break;
        case RelationType::AFFECTST:
            return AffectsStarEvaluator::evaluateAffectsStarClause(this->database, relationClause, synonymTable);
            break;
        default:
            SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::evaluateRelationClause: Unknown relation type %d\n", relationClause.getRelationType());
            return {};
        }
    }

    ClauseResult QueryEvaluator::evaluatePatternClause(PatternClause &patternClause,
        std::unordered_map<std::string, DesignEntity> &synonymTable) {

        switch (patternClause.getPatternType()) {
        case PatternType::ASSIGN_PATTERN:
            return AssignPatternEvaluator::evaluateAssignPatternClause(this->database, patternClause, synonymTable);
            break;
        case PatternType::IF_PATTERN:
            return IfPatternEvaluator::evaluateIfPatternClause(this->database, patternClause, synonymTable);
            break;
        case PatternType::WHILE_PATTERN:
            return WhilePatternEvaluator::evaluateWhilePatternClause(this->database, patternClause, synonymTable);
            break;
        default:
            SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::evaluatePatternClause: Unknown pattern type %d\n", patternClause.getPatternType());
            return {};
        }

    }

    ClauseResult QueryEvaluator::evaluateWithClause(WithClause& withClause,
        std::unordered_map<std::string, DesignEntity>& synonymTable) {

        return WithEvaluator::evaluateWithClause(this->database, withClause, synonymTable);

    }

}
