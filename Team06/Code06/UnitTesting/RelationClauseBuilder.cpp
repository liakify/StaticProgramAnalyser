#include "RelationClauseBuilder.h"

namespace UnitTesting {

    RelationClauseBuilder& RelationClauseBuilder::setRelationType(RelationType relationType) {
        relationClause.type = relationType;
        return *this;
    }

    RelationClauseBuilder& RelationClauseBuilder::setFirstStmt(ArgType argType, StmtRef stmtRef) {
        relationClause.firstStmt = std::make_pair(argType, stmtRef);
        return *this;
    }

    RelationClauseBuilder& RelationClauseBuilder::setSecondStmt(ArgType argType, StmtRef stmtRef) {
        relationClause.secondStmt = std::make_pair(argType, stmtRef);
        return *this;
    }

    RelationClauseBuilder& RelationClauseBuilder::setFirstEnt(ArgType argType, EntityRef entRef) {
        relationClause.firstEnt = std::make_pair(argType, entRef);
        return *this;
    }

    RelationClauseBuilder& RelationClauseBuilder::setSecondEnt(ArgType argType, EntityRef entRef) {
        relationClause.secondEnt = std::make_pair(argType, entRef);
        return *this;
    }

    PQL::RelationClause RelationClauseBuilder::getClause() {
        return relationClause;
    }


}