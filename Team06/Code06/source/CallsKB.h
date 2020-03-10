#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Types.h"
#include "Constants.h"

struct callsRS {
    std::unordered_set<ProcId> directCallers;
    std::unordered_set<ProcId> directCallees;
    std::unordered_set<ProcId> allCallers;
    std::unordered_set<ProcId> allCallees;
};

class CallsKB {
 public:
    /*
    *   Adds Calls(p1, p2) to the callsTable where p1 directly calls p2
    */
    void addCalls(ProcId p1, ProcId p2);

    /*
    *   Returns TRUE if p1 directly calls p2, FALSE otherwise
    */
    bool calls(ProcId p1, ProcId p2);

    /*
    *   Returns TRUE if p1 directly or indirectly calls p2, FALSE otherwise
    */
    bool callStar(ProcId p1, ProcId p2);

    /*
        Returns a reference to directCallees/directCallers of p for NodeType SUCCESSOR and PREDECESSOR respectively
    */
    const std::unordered_set<ProcId>& getDirectNodes(ProcId p, NodeType type);

    /*
        Returns a reference to allCallees/allCallers of p for NodeType SUCCESSOR and PREDECESSOR respectively
    */
    const std::unordered_set<ProcId>& getAllNodes(ProcId p, NodeType type);

    /*
        Adds proc to allCallees/allCallers of p for NodeType SUCCESSOR and PREDECESSOR respectively
    */
    void addToAll(ProcId p, ProcId proc, NodeType type);

    /*
        Adds procs to allCallees/allCallers of p for NodeType SUCCESSOR and PREDECESSOR respectively
    */
    void addToAll(ProcId p, const std::unordered_set<ProcId>& procs, NodeType type);

    /*
        Returns TRUE if directCallee of p is non-empty, FALSE otherwise
    */
    bool hasCallee(ProcId p);

    /*
        Returns TRUE if directCaller of p is non-empty, FALSE otherwise
    */
    bool hasCaller(ProcId p);

    /*
        Returns TRUE if a Calls relation exists, FALSE otherwise
    */
    bool hasCallsRelation();

    /*
        Returns all procedures that call another procedure
    */
    const std::unordered_set<ProcId>& getAllCallers();

    /*
        Returns all procedures called by another procedure
    */
    const std::unordered_set<ProcId>& getAllCallees();

 private:
    std::unordered_map<ProcId, callsRS> callsTable;

    std::unordered_set<ProcId> allProcCallers;
    std::unordered_set<ProcId> allProcCallees;
};

