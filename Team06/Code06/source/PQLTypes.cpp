#include "PQLTypes.h"

using std::string;
using std::pair;
using std::vector;

namespace PQL {

    bool ReturnType::operator==(const ReturnType& other) const {
        return this->synonym == other.synonym && this->synonymId == other.synonymId &&
            this->attrType == other.attrType;
    }

    bool Argument::operator==(const Argument& other) const {
        return this->type == other.type && this->value == other.value &&
            this->synonymId == other.synonymId && this->attrType == other.attrType;
    }

    Clause::Clause(string clause, ClauseType clauseType)
        : clause(clause), clauseType(clauseType) {
    }

    bool Clause::operator==(const Clause& other) const {
        return this->clause == other.clause && this->clauseType == other.clauseType;
    }

    string Clause::asString() {
        return this->clause;
    }

    ClauseType Clause::getClauseType() {
        return this->clauseType;
    }

    RelationClause::RelationClause(string clause, RelationType type,
        Argument firstArg, Argument secondArg)
        : Clause(clause, ClauseType::RELATION), type(type), firstArg(firstArg), secondArg(secondArg) {
    }

    bool RelationClause::operator==(const RelationClause& other) const {
        return Clause::operator==(other) && this->type == other.type &&
            this->firstArg == other.firstArg && this->secondArg == other.secondArg;
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
        return true;
    }

    RelationType RelationClause::getRelationType() {
        return this->type;
    }

    pair<Argument, Argument> RelationClause::getArgs() {
        return { this->firstArg, this->secondArg };
    }

    PatternClause::PatternClause(string clause, PatternType type, Argument synonymArg,
        Argument targetArg, Argument patternArg)
        : Clause(clause, ClauseType::PATTERN), type(type), synonymArg(synonymArg),
            targetArg(targetArg), patternArg(patternArg) {
    }

    bool PatternClause::operator==(const PatternClause& other) const {
        return Clause::operator==(other) &&
            this->type == other.type && this->synonymArg == other.synonymArg &&
            this->targetArg == other.targetArg && this->patternArg == other.patternArg;
    }

    PatternType PatternClause::getPatternType() {
        return this->type;
    }

    Argument PatternClause::getSynonym() {
        return this->synonymArg;
    }

    pair<Argument, Argument> PatternClause::getArgs() {
        return { this->targetArg, this->patternArg };
    }

    WithClause::WithClause(string clause, WithType type, Argument leftArg, Argument rightArg)
        : Clause(clause, ClauseType::WITH), type(type), leftArg(leftArg), rightArg(rightArg) {
    }

    bool WithClause::operator==(const WithClause& other) const {
        return Clause::operator==(other) && this->type == other.type &&
            this->leftArg == other.leftArg && this->rightArg == other.rightArg;
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

    pair<Argument, Argument> WithClause::getArgs() {
        return { this->leftArg, this->rightArg };
    }

    bool Query::operator==(const Query& other) const {
        return this->status == other.status && this->queryString == other.queryString &&
            this->returnsBool == other.returnsBool && this->targetEntities == other.targetEntities &&
            this->synonymTable == other.synonymTable && this->relations == other.relations &&
            this->patterns == other.patterns && this->equalities == other.equalities;
    }

};
