#pragma once

#include "PQLTypes.h"
#include "Types.h"

namespace UnitTesting {

    /**
    * The RelationClauseBuilder class assists in the construction of a relation clause for unit tests.
    */
    class RelationClauseBuilder {
    public:
        /**
        * Sets the relation type for the clause.
        *
        * @param    relationType    The relation type for the clause 
        */
        RelationClauseBuilder& setRelationType(RelationType relationType);
        
        /**
        * Sets the first parameter of the clause, if the first parameter of the clause is a statement reference.
        *
        * @param    argType     The argument type
        * @param    stmtRef     The parameter for the clause
        */
        RelationClauseBuilder& setFirstStmt(ArgType argType, StmtRef stmtRef);

        /**
        * Sets the second parameter of the clause, if the second parameter of the clause is a statement reference.
        *
        * @param    argType     The argument type
        * @param    stmtRef     The parameter for the clause
        */
        RelationClauseBuilder& setSecondStmt(ArgType argType, StmtRef stmtRef);

        /**
        * Sets the first parameter of the clause, if the first parameter of the clause is an entity reference.
        *
        * @param    argType     The argument type
        * @param    entRef      The parameter for the clause
        */
        RelationClauseBuilder& setFirstEnt(ArgType argType, EntityRef entRef);

        /**
        * Sets the second parameter of the clause, if the second parameter of the clause is an entity reference.
        *
        * @param    argType     The argument type
        * @param    entRef      The parameter for the clause
        */
        RelationClauseBuilder& setSecondEnt(ArgType argType, EntityRef entRef);

        /**
        * Returns the constructed relation clause.
        *
        * @return   The constructed relation clause.
        */
        PQL::RelationClause getClause();

    private:
        PQL::RelationClause relationClause;

    };

}