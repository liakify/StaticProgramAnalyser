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
        pair<ArgType, StmtEntRef> firstArg, pair<ArgType, StmtEntRef> secondArg)
        : Clause(clause, ClauseType::RELATION), type(type), firstArg(firstArg), secondArg(secondArg) {
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

    pair<pair<ArgType, EntityRef>, pair<ArgType, Pattern>> PatternClause::getArgs() {
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
