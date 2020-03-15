#pragma once

#include "PKB.h"
#include "Types.h"

namespace UnitTesting {

    /**
    * The PKBBuilder class assists in the construction of a PKB for unit tests.
    */
    class PKBBuilder {
    public:
        /**
        * Adds a variable to the PKB.
        * 
        * @param    var     The variable to add.
        */
        PKBBuilder& addVar(VarName var);

        /**
        * Adds a print statement to the PKB.
        *
        * @param    var     The variable used by the print statement.
        */
        PKBBuilder& addPrintStmt(VarName var);
        
        /**
        * Adds a print statement to the PKB.
        *
        * @param    var     The variable modified by the read statement.
        */
        PKBBuilder& addReadStmt(VarName var);

        /**
        * Adds an assign statement to the PKB. TODO: Support expressions.
        *
        * @param    var     The variable modified by the assign statement.
        */
        PKBBuilder& addAssignStmt(VarName var);

        /**
        * Adds a if statement to the PKB. TODO: Support expressions.
        *
        * @param    var     The variable modified by the assign statement.
        */
        PKBBuilder& addIfStmt(VarName var);
        
        /**
        * Adds a while statement to the PKB. TODO: Support expressions.
        *
        * @param    var     The variable modified by the assign statement.
        */
        PKBBuilder& addWhileStmt(VarName var);

        /**
        * Adds a call statement to the PKB.
        *
        * @param    proc    The procedure called by the call statement.
        */
        PKBBuilder& addCallStmt(ProcName proc);

        /**
        * Adds a Follows Relationship to the PKB, such that Follows(stmt1, stmt2) holds.
        *
        * @param    stmt1   The first statement in the Follows relationship
        * @param    stmt2   The second statement in the Follows relationship
        */
        PKBBuilder& addFollowsRS(StmtId stmt1, StmtId stmt2);

        /**
        * Adds Follows* Relationships to the PKB, such that Follows*(stmt1, stmts) holds.
        *
        * @param    stmt1   The first statement in the follows relationship
        * @param    stmts   The list of statements for the second statement in the Follows* relationship
        */
        PKBBuilder& addFollowsStarRS(StmtId stmt1, std::unordered_set<StmtId> stmts);

        /**
        * Adds Follows* Relationships to the PKB, such that Follows*(stmt1, stmts) holds.
        *
        * @param    stmts   The list of statements for the first statement in the Follows* relationship
        * @param    stmt2   The first statement in the follows relationship
        */
        PKBBuilder& addFollowsStarRS(std::unordered_set<StmtId> stmts, StmtId stmt2);
        
        /**
        * Returns the constructed PKB.
        *
        * @return   The constructed PKB
        */
        PKB::PKB& getPKB();

    private:
        PKB::PKB pkb;

    };

}