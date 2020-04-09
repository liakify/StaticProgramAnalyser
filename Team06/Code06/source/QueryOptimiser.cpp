#include <algorithm>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "QueryOptimiser.h"

namespace PQL {

    namespace QueryOptimiser {

        struct ClauseNode {
            Clause* clause;
            int id;
            int group = -1;
            std::vector<Synonym> synonyms;
            bool booleanResult = false;
            bool advancedRelation = false;

            int getSynonymCount() {
                return static_cast<int>(synonyms.size());
            }

            bool containsSynonym(Synonym syn) {
                return std::find(synonyms.begin(), synonyms.end(), syn) != synonyms.end();
            }

            bool operator<(const ClauseNode& other) const {
                // Prioritise basic relations
                if (!advancedRelation && other.advancedRelation) {
                    return true;
                }
                if (!other.advancedRelation && advancedRelation) {
                    return false;
                }
                // Prioritise clauses that return boolean results
                if (booleanResult && !other.booleanResult) {
                    return true;
                }
                if (other.booleanResult && !booleanResult) {
                    return false;
                }
                // Prioritise clauses with less synonyms
                if (synonyms.size() < other.synonyms.size()) {
                    return true;
                }
                if (other.synonyms.size() < synonyms.size()) {
                    return false;
                }
                // Prioritise with clauses
                if (clause->getClauseType() == ClauseType::WITH && other.clause->getClauseType() != ClauseType::WITH) {
                    return true;
                }
                if (other.clause->getClauseType() == ClauseType::WITH && clause->getClauseType() != ClauseType::WITH) {
                    return false;
                }

                return id < other.id;
            }
        };

        ClauseNode constructRelationClauseNode(RelationClause& relation) {
            ClauseNode node;

            node.clause = &relation;

            // Extract all synonyms
            std::pair<ArgType, std::string> arg1, arg2;
            arg1 = relation.getArgs().first;
            arg2 = relation.getArgs().second;
            if (arg1.first == ArgType::SYNONYM) {
                node.synonyms.emplace_back(arg1.second);
            }
            if (arg2.first == ArgType::SYNONYM) {
                node.synonyms.emplace_back(arg2.second);
            }

            // Remove duplicate synonyms
            node.synonyms.resize(std::distance(node.synonyms.begin(), std::unique(node.synonyms.begin(), node.synonyms.end())));

            // Check if relation is an advanced relation
            // Advanced relation clauses are Next*, Affects, Affects*
            if (relation.getRelationType() == RelationType::NEXTT || relation.getRelationType() == RelationType::AFFECTS
                || relation.getRelationType() == RelationType::AFFECTST) {
                node.advancedRelation = true;
            }

            // Checks if clause returns boolean
            if (arg1.first != ArgType::SYNONYM && arg2.first != ArgType::SYNONYM) {
                node.booleanResult = true;
            }

            return node;
        }

        ClauseNode constructPatternClauseNode(PatternClause& pattern) {
            ClauseNode node;

            node.clause = &pattern;

            // Extract all synonyms
            std::pair<ArgType, std::string> arg1, arg2;
            arg1 = pattern.getArgs().first;
            arg2 = pattern.getArgs().second;
            node.synonyms.emplace_back(pattern.getSynonym());
            if (arg1.first == ArgType::SYNONYM) {
                node.synonyms.emplace_back(arg1.second);
            }
            if (arg2.first == ArgType::SYNONYM) {
                node.synonyms.emplace_back(arg2.second);
            }

            // Remove duplicate synonyms
            std::sort(node.synonyms.begin(), node.synonyms.end());
            node.synonyms.resize(std::distance(node.synonyms.begin(), std::unique(node.synonyms.begin(), node.synonyms.end())));

            // Pattern clauses are not relation clauses
            node.advancedRelation = false;

            // Pattern clauses will never return boolean
            node.booleanResult = false;

            return node;
        }

        ClauseNode constructWithClauseNode(WithClause& with) {
            ClauseNode node;

            node.clause = &with;

            // Extract all synonyms
            std::pair<ArgType, Ref> arg1, arg2;
            arg1 = with.getArgs().first;
            arg2 = with.getArgs().second;

            if (arg1.first == ArgType::SYNONYM) {
                node.synonyms.emplace_back(arg1.second.first);
            }
            if (arg2.first == ArgType::SYNONYM) {
                node.synonyms.emplace_back(arg2.second.first);
            }

            // Remove duplicate synonyms
            node.synonyms.resize(std::distance(node.synonyms.begin(), std::unique(node.synonyms.begin(), node.synonyms.end())));

            // With clauses are not relation clauses
            node.advancedRelation = false;

            if (arg1.first != ArgType::SYNONYM && arg2.first != ArgType::SYNONYM) {
                node.booleanResult = true;
            }

            return node;
        }

        OptimisedQuery optimiseQuery(Query& query) {

            // Create list of all clauses
            std::vector<ClauseNode> nodes;
            for (RelationClause& relation : query.relations) {
                nodes.emplace_back(constructRelationClauseNode(relation));
            }
            for (PatternClause& pattern : query.patterns) {
                nodes.emplace_back(constructPatternClauseNode(pattern));
            }
            for (WithClause& with : query.equalities) {
                nodes.emplace_back(constructWithClauseNode(with));
            }

            // Label all clause nodes with an id
            for (int i = 0; i < nodes.size(); i++) {
                nodes[i].id = i;
            }

            // Construct adjacency list
            // ClauseNodes in this adjacency list are identified by their index in the 'nodes' vector
            std::vector<std::vector<int> > graph(nodes.size(), std::vector<int>());

            for (int i = 0; i < nodes.size(); i++) {
                for (int j = 0; j < nodes.size(); j++) {
                    // Connect with bidirectional edge if the two clauses share any synonyms
                    for (Synonym synonym : nodes[i].synonyms) {
                        if (nodes[j].containsSynonym(synonym)) {
                            graph[i].emplace_back(j);
                            graph[j].emplace_back(i);
                        }
                    }
                }
            }

            // Perform a BFS to label every clause with a group id
            int numGroups = 0;
            for (ClauseNode &node : nodes) {
                if (node.group == -1) {  // -1 indicates no group
                    std::queue<int> q;
                    q.emplace(node.id);
                    while (!q.empty()) {
                        int front = q.front();
                        q.pop();
                        if (nodes[front].group != -1) {
                            continue;
                        }
                        nodes[front].group = numGroups;
                        for (int neighbour : graph[front]) {
                            q.emplace(neighbour);
                        }
                    }
                    numGroups++;
                }
            }

            // Initialise OptimisedQuery struct
            OptimisedQuery optQuery;
            optQuery.last = std::vector<int>(numGroups);

            // To check if a clause has been added to the OptimisedQuery yet
            std::vector<bool> added(nodes.size(), false);

            // All clauses that evaluate to a boolean should be evaluated first
            // They are also in a group alone
            for (ClauseNode& node : nodes) {
                if (node.booleanResult) {
                    optQuery.clauses.emplace_back(node.clause);
                    optQuery.groups.emplace_back(node.group);
                    optQuery.last[node.group] = optQuery.clauses.size() - 1;
                    added[node.id] = true;
                }
            }

            // Perform a Multi-Source Best First Search. Start with clauses containing single synonyms.
            auto compare = [](ClauseNode* first, ClauseNode* second) {
                return *first < *second;
            };

            // We need each group to have at least one clause in the priority queue.
            // This is so that in case we get stuck on an advanced relation in all groups containing single-synonym
            // clauses, it might be better to proceed with a non-advanced relation clause containing two synonyms.
            std::priority_queue<ClauseNode*, std::vector<ClauseNode*>, decltype(compare)> pq(compare);

            // Find a starting clause for every group
            std::vector<ClauseNode*> startingNode = std::vector<ClauseNode*>(numGroups, nullptr);
            for (ClauseNode& node : nodes) {
                if (startingNode[node.group] == nullptr) {
                    startingNode[node.group] = &node;
                } else if (node < *startingNode[node.group]) {
                    // If the evicted clause is a single-synonym basic relation, add it anyway
                    if (!startingNode[node.group]->advancedRelation && startingNode[node.group]->getSynonymCount() == 1) {
                        pq.emplace(startingNode[node.group]);
                    }
                    startingNode[node.group] = &node;

                }
            }

            // Add all starting clauses into the priority queue, except those that evaluate to a boolean.
            for (ClauseNode* node : startingNode) {
                if (!node->booleanResult) {
                    pq.emplace(node);
                }
            }

            // Perform the BFS
            while (!pq.empty()) {
                ClauseNode* firstNode = pq.top();
                pq.pop();
                if (added[firstNode->id]) {
                    continue;
                }
                optQuery.clauses.emplace_back(firstNode->clause);
                optQuery.groups.emplace_back(firstNode->group);
                optQuery.last[firstNode->group] = optQuery.clauses.size() - 1;
                added[firstNode->id] = true;
                for (int neighbour : graph[firstNode->id]) {
                    if (!added[neighbour]) {
                        pq.emplace(&nodes[neighbour]);
                    }
                }
            }

            return optQuery;
        }

    }
}