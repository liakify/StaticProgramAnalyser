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
        * @param    stmt1   The first statement in the Follows* relationship
        * @param    stmts   The list of statements for the second statement in the Follows* relationship
        */
        PKBBuilder& addFollowsStarRS(StmtId stmt1, std::unordered_set<StmtId> stmts);

        /**
        * Adds Follows* Relationships to the PKB, such that Follows*(stmt1, stmts) holds.
        *
        * @param    stmts   The list of statements for the first statement in the Follows* relationship
        * @param    stmt2   The first statement in the Follows* relationship
        */
        PKBBuilder& addFollowsStarRS(std::unordered_set<StmtId> stmts, StmtId stmt2);

        /**
        * Adds a Parent Relationship to the PKB, such that Parent(stmt1, stmt2) holds.
        *
        * @param    stmt1   The first statement in the Parent relationship
        * @param    stmt2   The second statement in the Parent relationship
        */
        PKBBuilder& addParentRS(StmtId stmt1, StmtId stmt2);

        /**
        * Adds Parent* Relationships to the PKB, such that Parent*(stmt1, stmts) holds.
        *
        * @param    stmt1   The first statement in the Parent* relationship
        * @param    stmts   The list of statements for the second statement in the Parent* relationship
        */
        PKBBuilder& addParentStarRS(StmtId stmt1, std::unordered_set<StmtId> stmts);

        /**
        * Adds Parent* Relationships to the PKB, such that Parent*(stmt1, stmts) holds.
        *
        * @param    stmts   The list of statements for the first statement in the Parent* relationship
        * @param    stmt2   The first statement in the Parent* relationship
        */
        PKBBuilder& addParentStarRS(std::unordered_set<StmtId> stmts, StmtId stmt2);

        /**
        * Adds a Modifies Relationship to the PKB, such that Modifies(stmt, var) holds.
        *
        * @param    stmt   The statement in the Modifies relationship
        * @param    var    The variable in the Modifies relationship
        */
        PKBBuilder& addStmtModifiesRS(StmtId stmt, VarName var);

        /**
        * Adds a Modifies Relationship to the PKB, such that Modifies(proc, var) holds.
        *
        * @param    proc   The procedure in the Modifies relationship
        * @param    var    The variable in the Modifies relationship
        */
        PKBBuilder& addProcModifiesRS(ProcName proc, VarName var);
        
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