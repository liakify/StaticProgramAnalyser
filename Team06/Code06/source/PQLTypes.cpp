#include "PQLTypes.h"

using std::string;
using std::pair;

namespace PQL {

    Clause::Clause(string clause, ClauseType clauseType)
        : clause(clause), clauseType(clauseType) {
    }

    string Clause::asString() {
        return this->clause;
    }

    ClauseType Clause::getClauseType() {
        return this->clauseType;
    }

    RelationClause::RelationClause(string clause, RelationType type,
        pair<ArgType, StmtRef> firstStmt, pair<ArgType, StmtRef> secondStmt,
        pair<ArgType, EntityRef> firstEnt, pair<ArgType, EntityRef> secondEnt)
        : Clause(clause, ClauseType::RELATION), type(type), firstStmt(firstStmt),
            secondStmt(secondStmt), firstEnt(firstEnt), secondEnt(secondEnt) {
    }

    bool RelationClause::setProcedureVariant() {
        if (this->type != RelationType::USESS && this->type != RelationType::MODIFIESS) {
            return false;
        }

        // Current relation type is either Uses or Modifies for statements
        // Amend it to the procedure variant, modifying the positional arguments as well
        this->type = this->type == RelationType::USESS
            ? RelationType::USESP
            : RelationType::MODIFIESP;
        this->firstEnt = this->firstStmt;
        this->firstStmt = INVALID_ARG;

        return true;
    }

    RelationType RelationClause::getRelationType() {
        return this->type;
    }

    pair<pair<ArgType, string>, pair<ArgType, string>> RelationClause::getArgs() {
        switch (this->type) {
        case RelationType::FOLLOWS:
        case RelationType::FOLLOWST:
        case RelationType::PARENT:
        case RelationType::PARENTT:
        case RelationType::NEXT:
        case RelationType::NEXTT:
        case RelationType::AFFECTS:
        case RelationType::AFFECTST:
            return { this->firstStmt, this->secondStmt };
            break;
        case RelationType::USESS:
        case RelationType::MODIFIESS:
            return { this->firstStmt, this->secondEnt };
            break;
        case RelationType::USESP:
        case RelationType::MODIFIESP:
        case RelationType::CALLS:
        case RelationType::CALLST:
            return { this->firstEnt, this->secondEnt };
        default:
            return { INVALID_ARG, INVALID_ARG };
        }
    }

    PatternClause::PatternClause(string clause, PatternType type, string synonym,
        pair<ArgType, EntityRef> targetArg, pair<ArgType, Pattern> patternArg)
        : Clause(clause, ClauseType::PATTERN), type(type), synonym(synonym),
            targetArg(targetArg), patternArg(patternArg) {
    }

    PatternType PatternClause::getPatternType() {
        return this->type;
    }

    string PatternClause::getSynonym() {
        return this->synonym;
    }

    pair<pair<ArgType, string>, pair<ArgType, string>> PatternClause::getArgs() {
        return { this->targetArg, this->patternArg };
    }

    WithClause::WithClause(string clause, WithType type,
        pair<ArgType, Ref> leftArg, pair<ArgType, Ref> rightArg)
        : Clause(clause, ClauseType::WITH), type(type), leftArg(leftArg), rightArg(rightArg) {
    }

    bool WithClause::setWithType(WithType type) {
        if (this->type != WithType::UNKNOWN_EQUAL) {
            return false;
        }

        this->type = type;
        return true;
    }

    WithType WithClause::getWithType() {
        return this->type;
    }

    pair<pair<ArgType, Ref>, pair<ArgType, Ref>> WithClause::getArgs() {
        return { this->leftArg, this->rightArg };
    }

};
