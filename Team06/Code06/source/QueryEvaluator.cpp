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
                resultEntry[synonym] = std::to_string(stmt);
                clauseResult.emplace_back(resultEntry);
            }
            return clauseResult;
            break;
        }
        case DesignEntity::PROG_LINE:
        case DesignEntity::STATEMENT: {
            for (StmtId i = 1; i <= database.stmtTable.size(); i++) {
                ClauseResultEntry resultEntry;
                resultEntry[synonym] = std::to_string(i);
                clauseResult.emplace_back(resultEntry);
            }
            return clauseResult;
            break;
        }
        case DesignEntity::CONSTANT: {
            for (ConstId i = 1; i <= database.constTable.size(); i++) {
                ClauseResultEntry resultEntry;
                resultEntry[synonym] = database.constTable.get(i);
                clauseResult.emplace_back(resultEntry);
            }
            return clauseResult;
            break;
        }
        case DesignEntity::PROCEDURE: {
            for (ProcId i = 1; i <= database.procTable.size(); i++) {
                ClauseResultEntry resultEntry;
                resultEntry[synonym] = database.procTable.get(i).getName();
                clauseResult.emplace_back(resultEntry);
            }
            return clauseResult;
            break;
        }
        case DesignEntity::VARIABLE: {
            std::unordered_set<VarName> allVars = database.varTable.getAllVars();
            for (VarName var : allVars) {
                ClauseResultEntry resultEntry;
                resultEntry[synonym] = var;
                clauseResult.emplace_back(resultEntry);
            }
            return clauseResult;
            break;
        }
        }
        return {};
    }

    ClauseResult QueryEvaluator::evaluateQuery(Query &query) {

        // Results of Clauses
        std::vector<ClauseResult> clauseResults;

        // Add table containing "TRUE"
        ClauseResult trueResult;
        ClauseResultEntry trueResultEntry;
        trueResultEntry["_RESULT"] = "TRUE";
        trueResult.emplace_back(trueResultEntry);
        clauseResults.emplace_back(trueResult);

        // Construct set of all target synonyms
        std::unordered_set<Synonym> targetSynonyms;
        for (Ref ref : query.targetEntities) {
            targetSynonyms.insert(ref.first);
        }

        // Evaluate Relation Clauses
        for (RelationClause relation : query.relations) {
            clauseResults.emplace_back(evaluateRelationClause(relation, query.synonymTable));
            // Remove all present synonyms from target synonyms
            std::pair<ArgType, std::string> arg1, arg2;
            arg1 = relation.getArgs().first;
            arg2 = relation.getArgs().second;
            if (arg1.first == ArgType::SYNONYM) {
                targetSynonyms.erase(arg1.second);
            }
            if (arg2.first == ArgType::SYNONYM) {
                targetSynonyms.erase(arg2.second);
            }
        }

        // Evaluate Pattern Clauses
        for (PatternClause pattern : query.patterns) {
            clauseResults.emplace_back(evaluatePatternClause(pattern, query.synonymTable));
            // Remove all present synonyms from target synonyms
            std::pair<ArgType, std::string> arg1, arg2;
            arg1 = pattern.getArgs().first;
            arg2 = pattern.getArgs().second;
            targetSynonyms.erase(pattern.getSynonym());
            if (arg1.first == ArgType::SYNONYM) {
                targetSynonyms.erase(arg1.second);
            }
            if (arg2.first == ArgType::SYNONYM) {
                targetSynonyms.erase(arg2.second);
            }
        }

        // Evaluate With Clauses
        for (WithClause with : query.equalities) {
            clauseResults.emplace_back(evaluateWithClause(with, query.synonymTable));
            // Remove all present synonyms from target synonyms
            std::pair<ArgType, Ref> arg1, arg2;
            arg1 = with.getArgs().first;
            arg2 = with.getArgs().second;
            if (arg1.first == ArgType::SYNONYM) {
                targetSynonyms.erase(arg1.second.first);
            }
            if (arg2.first == ArgType::SYNONYM) {
                targetSynonyms.erase(arg2.second.first);
            }
        }

        // Add a table for each synonym not present in any clause
        for (Synonym synonym : targetSynonyms) {
            clauseResults.emplace_back(getClauseResultWithAllValues(synonym, query.synonymTable[synonym]));
        }

        // Combine Results
        ClauseResult combinedResult = combineClauseResults(clauseResults);

        // Extract necessary results to answer query
        ClauseResult result = extractQueryResults(query, combinedResult);

        return result;
    }

    ClauseResult QueryEvaluator::extractQueryResults(Query &query, ClauseResult& combinedResult) {

        if (query.returnsBool) {
            ClauseResult result;
            ClauseResultEntry resultEntry;
            if (combinedResult.empty()) {
                resultEntry["_BOOLEAN"] = "FALSE";
            } else {
                resultEntry["_BOOLEAN"] = "TRUE";
            }
            result.emplace_back(resultEntry);
            return result;
        } else if (combinedResult.empty()) {
            SPA::LoggingUtils::LogErrorMessage("QueryEvaluator::extractQueryResults: Empty Result!\n");
            return {};
        } else {
            std::vector<Ref> &targets = query.targetEntities;
            ClauseResult finalResult;
            for (ClauseResultEntry resultEntry : combinedResult) {
                ClauseResultEntry finalResultEntry;
                for (Ref target : targets) {
                    if (target.second == AttrType::NONE) {
                        finalResultEntry[target.first] = resultEntry[target.first];
                    } else if (target.second == AttrType::STMT_NUM) {
                        finalResultEntry[target.first + ".stmt#"] = resultEntry[target.first];
                    } else if (target.second == AttrType::VALUE) {
                        finalResultEntry[target.first + ".value"] = resultEntry[target.first];
                    } else if (target.second == AttrType::PROC_NAME) {
                        if (query.synonymTable[target.first] == DesignEntity::PROCEDURE) {
                            finalResultEntry[target.first + ".procName"] = resultEntry[target.first];
                        } else {
                            CallStmt *callStmt = dynamic_cast<CallStmt*>(database.stmtTable.get(std::stoi(resultEntry[target.first])).get());
                            finalResultEntry[target.first + ".procName"] = callStmt->getProc();
                        }
                    } else if (target.second == AttrType::VAR_NAME) {
                        if (query.synonymTable[target.first] == DesignEntity::READ) {
                            ReadStmt* readStmt = dynamic_cast<ReadStmt*>(database.stmtTable.get(std::stoi(resultEntry[target.first])).get());
                            finalResultEntry[target.first + ".varName"] = database.varTable.get(readStmt->getVar());
                        } else if (query.synonymTable[target.first] == DesignEntity::PRINT) {
                            PrintStmt* printStmt = dynamic_cast<PrintStmt*>(database.stmtTable.get(std::stoi(resultEntry[target.first])).get());
                            finalResultEntry[target.first + ".varName"] = database.varTable.get(printStmt->getVar());
                        } else {
                            finalResultEntry[target.first + ".varName"] = resultEntry[target.first];
                        }
                    }
                }
                finalResult.emplace_back(finalResultEntry);
            }
            return finalResult;
        }
    }

    ClauseResultEntry QueryEvaluator::combineTwoClauseResultEntries(ClauseResultEntry &entry1, ClauseResultEntry &entry2,
        std::unordered_set<Synonym> &commonSynonyms) {

        ClauseResultEntry combinedEntry;
        for (std::pair<std::string, std::string> field : entry1) {
            combinedEntry.insert(field);
        }

        for (std::pair<std::string, std::string> field : entry2) {
            if (commonSynonyms.find(field.first) == commonSynonyms.end()) {
                combinedEntry.insert(field);
            }
        }

        return combinedEntry;
    }

    bool QueryEvaluator::checkCommonSynonyms(ClauseResultEntry &entry1, ClauseResultEntry &entry2,
        std::unordered_set<Synonym> &commonSynonyms) {
        for (Synonym synonym : commonSynonyms) {
            if (entry1[synonym] != entry2[synonym]) {
                return false;
            }
        }
        return true;
    }

    ClauseResult QueryEvaluator::combineTwoClauseResults(ClauseResult clauseResults1, ClauseResult clauseResults2) {
        if (clauseResults1.empty() || clauseResults2.empty()) {
            return {};
        }

        // Extract common synonyms
        std::unordered_set<Synonym> commonSynonyms;
        for (std::pair<std::string, std::string> entry : clauseResults1[0]) {
            if (clauseResults2[0].find(entry.first) != clauseResults2[0].end()) {
                commonSynonyms.insert(entry.first);
            }
        }

        ClauseResult combinedResult;
        // Perform a Cartesian Product
        for (ClauseResultEntry entry1 : clauseResults1) {
            for (ClauseResultEntry entry2 : clauseResults2) {
                if (checkCommonSynonyms(entry1, entry2, commonSynonyms)) {
                    combinedResult.emplace_back(combineTwoClauseResultEntries(entry1, entry2, commonSynonyms));
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
