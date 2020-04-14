#include "PQLTypes.h"

using std::string;
using std::pair;
using std::vector;

namespace PQL {

    Clause::Clause(string clause, bool isNegated, ClauseType clauseType)
        : clause(clause), isNegated(isNegated), clauseType(clauseType) {
    }

    bool Clause::operator==(const Clause& other) const {
        return this->clause == other.clause && this->isNegated == other.isNegated &&
            this->clauseType == other.clauseType;
    }

    string Clause::asString() {
        return this->clause;
    }

    bool Clause::isNegatedClause() {
        return this->isNegated;
    }

    ClauseType Clause::getClauseType() {
        return this->clauseType;
    }

    RelationClause::RelationClause(string clause, bool isNegated, RelationType type,
        pair<ArgType, StmtEntRef> firstArg, pair<ArgType, StmtEntRef> secondArg)
        : Clause(clause, isNegated, ClauseType::RELATION), type(type),
            firstArg(firstArg), secondArg(secondArg) {
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

    pair<pair<ArgType, StmtEntRef>, pair<ArgType, StmtEntRef>> RelationClause::getArgs() {
        return { this->firstArg, this->secondArg };
    }

    PatternClause::PatternClause(string clause, bool isNegated, PatternType type, string synonym,
        pair<ArgType, EntityRef> targetArg, pair<ArgType, Pattern> patternArg)
        : Clause(clause, isNegated, ClauseType::PATTERN), type(type), synonym(synonym),
            targetArg(targetArg), patternArg(patternArg) {
    }

    bool PatternClause::operator==(const PatternClause& other) const {
        return Clause::operator==(other) &&
            this->type == other.type && this->synonym == other.synonym &&
            this->targetArg == other.targetArg && this->patternArg == other.patternArg;
    }

    PatternType PatternClause::getPatternType() {
        return this->type;
    }

    string PatternClause::getSynonym() {
        return this->synonym;
    }

    pair<pair<ArgType, EntityRef>, pair<ArgType, Pattern>> PatternClause::getArgs() {
        return { this->targetArg, this->patternArg };
    }

    WithClause::WithClause(string clause, bool isNegated, WithType type,
        pair<ArgType, Ref> leftArg, pair<ArgType, Ref> rightArg)
        : Clause(clause, isNegated, ClauseType::WITH), type(type),
            leftArg(leftArg), rightArg(rightArg) {
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

    pair<pair<ArgType, Ref>, pair<ArgType, Ref>> WithClause::getArgs() {
        return { this->leftArg, this->rightArg };
    }

    bool Query::operator==(const Query& other) const {
        return this->status == other.status && this->queryString == other.queryString &&
            this->returnsBool == other.returnsBool && this->targetEntities == other.targetEntities &&
            this->synonymTable == other.synonymTable && this->relations == other.relations &&
            this->patterns == other.patterns && this->equalities == other.equalities;
    }

};
