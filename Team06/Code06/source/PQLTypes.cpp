#include "PQLTypes.h"

using std::string;
using std::pair;
using std::vector;

namespace PQL {

    bool ReturnType::operator==(const ReturnType& other) const {
        return this->synonym == other.synonym && this->synonymId == other.synonymId &&
            this->attrType == other.attrType;
    }

    bool ReturnType::operator<(const ReturnType& other) const {
        // Compare two return types lexicographically by synonym, breaking ties by attribute type
        return this->synonym != other.synonym
            ? this->synonym < other.synonym
            : this->attrType < other.attrType;
    }

    bool Argument::operator==(const Argument& other) const {
        return this->type == other.type && this->value == other.value &&
            this->synonymId == other.synonymId && this->attrType == other.attrType;
    }

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
        Argument firstArg, Argument secondArg)
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

    bool RelationClause::setFirstSynonymId(int synonymId) {
        if (synonymId < 0 || this->firstArg.type != ArgType::SYNONYM) {
            // Synonym ID supplied is negative or argument is not a synonym => reject
            return false;
        } else if (this->firstArg.synonymId != UNSET_SYNONYM_ID) {
            // Synonym ID has been previously set => reject modification
            return false;
        }

        // Otherwise accept the modification and set this synonym argument's synonym ID
        this->firstArg.synonymId = synonymId;
        return true;
    }

    bool RelationClause::setSecondSynonymId(int synonymId) {
        if (synonymId < 0 || this->secondArg.type != ArgType::SYNONYM) {
            // Synonym ID supplied is negative or argument is not a synonym => reject
            return false;
        } else if (this->secondArg.synonymId != UNSET_SYNONYM_ID) {
            // Synonym ID has been previously set => reject modification
            return false;
        }

        // Otherwise accept the modification and set this synonym argument's synonym ID
        this->secondArg.synonymId = synonymId;
        return true;
    }

    RelationType RelationClause::getRelationType() {
        return this->type;
    }

    pair<Argument, Argument> RelationClause::getArgs() {
        return { this->firstArg, this->secondArg };
    }

    PatternClause::PatternClause(string clause, bool isNegated, PatternType type,
        Argument synonymArg, Argument targetArg, Argument patternArg)
        : Clause(clause, isNegated, ClauseType::PATTERN), type(type), synonymArg(synonymArg),
            targetArg(targetArg), patternArg(patternArg) {
    }

    bool PatternClause::operator==(const PatternClause& other) const {
        return Clause::operator==(other) &&
            this->type == other.type && this->synonymArg == other.synonymArg &&
            this->targetArg == other.targetArg && this->patternArg == other.patternArg;
    }

    bool PatternClause::setPatternSynonymId(int synonymId) {
        if (synonymId < 0 || this->synonymArg.synonymId != UNSET_SYNONYM_ID) {
            // Synonym ID supplied is negative or synonym ID has been previously set => reject
            return false;
        }

        // Otherwise accept the modification and set this synonym argument's synonym ID
        this->synonymArg.synonymId = synonymId;
        return true;
    }

    bool PatternClause::setFirstSynonymId(int synonymId) {
        if (synonymId < 0 || this->targetArg.type != ArgType::SYNONYM) {
            // Synonym ID supplied is negative or argument is not a synonym => reject
            return false;
        } else if (this->targetArg.synonymId != UNSET_SYNONYM_ID) {
            // Synonym ID has been previously set => reject modification
            return false;
        }

        // Otherwise accept the modification and set this synonym argument's synonym ID
        this->targetArg.synonymId = synonymId;
        return true;
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

    WithClause::WithClause(string clause, bool isNegated, WithType type,
        Argument leftArg, Argument rightArg)
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

    bool WithClause::setLeftSynonymId(int synonymId) {
        if (synonymId < 0) {
            // Synonym ID supplied is negative => reject modification
            return false;
        } else if (this->leftArg.type != ArgType::SYNONYM && this->leftArg.type != ArgType::ATTRIBUTE) {
            // Left argument is not a synonym or attribute => reject modification
            return false;
        } else if (this->leftArg.synonymId != UNSET_SYNONYM_ID) {
            // Synonym ID has been previously set => reject modification
            return false;
        }

        // Otherwise accept the modification and set this synonym argument's synonym ID
        this->leftArg.synonymId = synonymId;
        return true;
    }

    bool WithClause::setRightSynonymId(int synonymId) {
        if (synonymId < 0) {
            // Synonym ID supplied is negative => reject modification
            return false;
        } else if (this->rightArg.type != ArgType::SYNONYM && this->rightArg.type != ArgType::ATTRIBUTE) {
            // Left argument is not a synonym or attribute => reject modification
            return false;
        } else if (this->rightArg.synonymId != UNSET_SYNONYM_ID) {
            // Synonym ID has been previously set => reject modification
            return false;
        }

        // Otherwise accept the modification and set this synonym argument's synonym ID
        this->rightArg.synonymId = synonymId;
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
