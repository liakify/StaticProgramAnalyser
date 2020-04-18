#include "stdafx.h"

#include "PQLTypes.h"

using std::pair;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

    TEST_CLASS(TestPQLTypes) {
    public:
        TEST_METHOD(returnTypeEquality) {
            PQL::ReturnType target = { "rd", PQL::UNSET_SYNONYM_ID, AttrType::NONE };
            PQL::ReturnType target2 = { "rd", PQL::UNSET_SYNONYM_ID, AttrType::NONE };

            Assert::IsTrue(target == target);
            Assert::IsTrue(target == target2);

            PQL::ReturnType diffSynonym = { "r", PQL::UNSET_SYNONYM_ID, AttrType::NONE };
            PQL::ReturnType diffSynonymId = { "rd", 0, AttrType::NONE };
            PQL::ReturnType diffAttrType = { "rd", PQL::UNSET_SYNONYM_ID, AttrType::VAR_NAME };

            Assert::IsFalse(target == diffSynonym);
            Assert::IsFalse(target == diffSynonymId);
            Assert::IsFalse(target == diffAttrType);
        }

        TEST_METHOD(returnTypeLessThan) {
            PQL::ReturnType target = { "cl", PQL::UNSET_SYNONYM_ID, AttrType::NONE };

            Assert::IsFalse(target < target);

            PQL::ReturnType diffSynonymId = { "cl", 0, AttrType::NONE };

            // Two return types with different synonym ID are not comparable
            Assert::IsFalse(target < diffSynonymId);
            Assert::IsFalse(diffSynonymId < target);

            PQL::ReturnType ltSynonym1 = { "c", PQL::UNSET_SYNONYM_ID, AttrType::NONE };
            PQL::ReturnType ltSynonym2 = { "ck", PQL::UNSET_SYNONYM_ID, AttrType::NONE };
            PQL::ReturnType gtSynonym1 = { "cla", PQL::UNSET_SYNONYM_ID, AttrType::NONE };
            PQL::ReturnType gtSynonym2 = { "cm", PQL::UNSET_SYNONYM_ID, AttrType::NONE };

            Assert::IsFalse(target < ltSynonym1);
            Assert::IsTrue(ltSynonym1 < ltSynonym2);
            Assert::IsFalse(target < ltSynonym2);
            Assert::IsTrue(target < gtSynonym1);
            Assert::IsTrue(gtSynonym1 < gtSynonym2);
            Assert::IsTrue(target < gtSynonym2);

            PQL::ReturnType gtAttrType1 = { "cl", PQL::UNSET_SYNONYM_ID, AttrType::PROC_NAME };
            PQL::ReturnType gtAttrType2 = { "cl", PQL::UNSET_SYNONYM_ID, AttrType::VAR_NAME };
            PQL::ReturnType gtAttrType3 = { "cl", PQL::UNSET_SYNONYM_ID, AttrType::VALUE };
            PQL::ReturnType gtAttrType4 = { "cl", PQL::UNSET_SYNONYM_ID, AttrType::STMT_NUM };

            // Less than is a transitive relation
            Assert::IsTrue(target < gtAttrType1);
            Assert::IsTrue(gtAttrType1 < gtAttrType2);
            Assert::IsTrue(gtAttrType2 < gtAttrType3);
            Assert::IsTrue(gtAttrType3 < gtAttrType4);
        }

        TEST_METHOD(argumentEquality) {
            PQL::Argument arg = { ArgType::SYNONYM, "l", PQL::UNSET_SYNONYM_ID, AttrType::NONE };
            PQL::Argument arg2 = { ArgType::SYNONYM, "l", PQL::UNSET_SYNONYM_ID, AttrType::NONE };

            Assert::IsTrue(arg == arg);
            Assert::IsTrue(arg == arg2);

            PQL::Argument diffType = { ArgType::IDENTIFIER, "l", PQL::UNSET_SYNONYM_ID, AttrType::NONE };
            PQL::Argument diffValue = { ArgType::SYNONYM, "l2", PQL::UNSET_SYNONYM_ID, AttrType::NONE };
            PQL::Argument diffSynonymId = { ArgType::SYNONYM, "l", 3203, AttrType::NONE };
            PQL::Argument diffAttrType = { ArgType::SYNONYM, "l", PQL::UNSET_SYNONYM_ID, AttrType::INVALID };

            Assert::IsFalse(arg == diffType);
            Assert::IsFalse(arg == diffValue);
            Assert::IsFalse(arg == diffSynonymId);
            Assert::IsFalse(arg == diffAttrType);
        }

        TEST_METHOD(relationClauseEquality) {
            PQL::RelationClause relation = { "not Modifies(w, \"i\")", true, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "w", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "i", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::RelationClause relation2 = { "not Modifies(w, \"i\")", true, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "w", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "i", PQL::NON_SYNONYM_ID, AttrType::INVALID } };

            Assert::IsTrue(relation == relation);
            Assert::IsTrue(relation == relation2);

            PQL::RelationClause diffClause = { "not Modifies(w, i)", true, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "w", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "i", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::RelationClause diffNegation = { "not Modifies(w, \"i\")", false, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "w", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "i", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::RelationClause diffType = { "not Modifies(w, \"i\")", true, RelationType::USESS,
                { ArgType::SYNONYM, "w", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "i", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::RelationClause diffArg1 = { "not Modifies(w, \"i\")", true, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "while", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "i", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::RelationClause diffArg2 = { "not Modifies(w, \"i\")", true, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "w", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "i", PQL::NON_SYNONYM_ID, AttrType::INVALID } };

            Assert::IsFalse(relation == diffClause);
            Assert::IsFalse(relation == diffNegation);
            Assert::IsFalse(relation == diffType);
            Assert::IsFalse(relation == diffArg1);
            Assert::IsFalse(relation == diffArg2);
        }

        TEST_METHOD(patternClauseEquality) {
            PQL::PatternClause pattern = { "a(v, _\"i + 1\"_)", false, PatternType::ASSIGN_PATTERN,
                { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::INCLUSIVE_PATTERN, "_(i+1)_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::PatternClause pattern2 = { "a(v, _\"i + 1\"_)", false, PatternType::ASSIGN_PATTERN,
                { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::INCLUSIVE_PATTERN, "_(i+1)_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };

            Assert::IsTrue(pattern == pattern);
            Assert::IsTrue(pattern == pattern2);

            PQL::PatternClause diffClause = { "a(v, \"i + 1\")", false, PatternType::ASSIGN_PATTERN,
                { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::INCLUSIVE_PATTERN, "_(i+1)_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::PatternClause diffNegation = { "a(v, _\"i + 1\"_)", true, PatternType::ASSIGN_PATTERN,
                { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::INCLUSIVE_PATTERN, "_(i+1)_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::PatternClause diffType = { "a(v, _\"i + 1\"_)", false, PatternType::IF_PATTERN,
                { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::INCLUSIVE_PATTERN, "_(i+1)_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::PatternClause diffSynonym = { "a(v, _\"i + 1\"_)", false, PatternType::ASSIGN_PATTERN,
                { ArgType::SYNONYM, "a2", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::INCLUSIVE_PATTERN, "_(i+1)_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::PatternClause diffTargetArg = { "a(v, _\"i + 1\"_)", false, PatternType::ASSIGN_PATTERN,
                { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::INCLUSIVE_PATTERN, "_(i+1)_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::PatternClause diffPatternArg = { "a(v, _\"i + 1\"_)", false, PatternType::ASSIGN_PATTERN,
                { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::INCLUSIVE_PATTERN, "(i+1)", PQL::NON_SYNONYM_ID, AttrType::INVALID } };

            Assert::IsFalse(pattern == diffClause);
            Assert::IsFalse(pattern == diffNegation);
            Assert::IsFalse(pattern == diffType);
            Assert::IsFalse(pattern == diffSynonym);
            Assert::IsFalse(pattern == diffTargetArg);
            Assert::IsFalse(pattern == diffPatternArg);
        }

        TEST_METHOD(withClauseEquality) {
            PQL::WithClause equality = { "not 2020 = a.stmt#", true, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, "2020", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::ATTRIBUTE, "a", PQL::UNSET_SYNONYM_ID, AttrType::STMT_NUM } };
            PQL::WithClause equality2 = { "not 2020 = a.stmt#", true, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, "2020", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::ATTRIBUTE, "a", PQL::UNSET_SYNONYM_ID, AttrType::STMT_NUM } };

            Assert::IsTrue(equality == equality);
            Assert::IsTrue(equality == equality2);

            PQL::WithClause diffClause = { "not a.stmt# = 2020", true, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, "2020", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::ATTRIBUTE, "a", PQL::UNSET_SYNONYM_ID, AttrType::STMT_NUM } };
            PQL::WithClause diffNegation = { "not 2020 = a.stmt#", false, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, "2020", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::ATTRIBUTE, "a", PQL::UNSET_SYNONYM_ID, AttrType::STMT_NUM } };
            PQL::WithClause diffType = { "not 2020 = a.stmt#", true, WithType::IDENTIFIER_EQUAL,
                { ArgType::INTEGER, "2020", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::ATTRIBUTE, "a", PQL::UNSET_SYNONYM_ID, AttrType::STMT_NUM } };
            PQL::WithClause diffLHS = { "not 2020 = a.stmt#", true, WithType::INTEGER_EQUAL,
                { ArgType::WILDCARD, "2020", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::ATTRIBUTE, "a", PQL::UNSET_SYNONYM_ID, AttrType::STMT_NUM } };
            PQL::WithClause diffRHSArg = { "not 2020 = a.stmt#", true, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, "2020", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::ATTRIBUTE, "cl", PQL::UNSET_SYNONYM_ID, AttrType::STMT_NUM } };
            PQL::WithClause diffRHSAttrType = { "not 2020 = a.stmt#", true, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, "2020", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::ATTRIBUTE, "a", PQL::UNSET_SYNONYM_ID, AttrType::VALUE } };

            Assert::IsFalse(equality == diffClause);
            Assert::IsFalse(equality == diffNegation);
            Assert::IsFalse(equality == diffType);
            Assert::IsFalse(equality == diffLHS);
            Assert::IsFalse(equality == diffRHSArg);
            Assert::IsFalse(equality == diffRHSAttrType);
        }

        TEST_METHOD(clauseGetters) {
            PQL::RelationClause relationSS = { "Affects*(1, a)", false, RelationType::AFFECTS,
                { ArgType::INTEGER, "1", PQL::NON_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID } };
            PQL::RelationClause relationSE = { "not Uses(a, \"x\")", true, RelationType::USESS,
                { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "x", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::Argument relArg1 = { ArgType::INTEGER, "1", PQL::NON_SYNONYM_ID, AttrType::INVALID };
            PQL::Argument relArg2 = { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID };
            PQL::Argument relArg3 = { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID };
            PQL::Argument relArg4 = { ArgType::IDENTIFIER, "x", PQL::NON_SYNONYM_ID, AttrType::INVALID };

            Assert::IsTrue(relationSS.asString() == "Affects*(1, a)");
            Assert::IsTrue(relationSS.isNegatedClause() == false);
            Assert::IsTrue(relationSE.isNegatedClause() == true);
            Assert::IsTrue(relationSE.getClauseType() == ClauseType::RELATION);
            Assert::IsTrue(relationSS.getRelationType() == RelationType::AFFECTS);
            Assert::IsTrue(relationSS.getArgs().first == relArg1);
            Assert::IsTrue(relationSS.getArgs().second == relArg2);
            Assert::IsTrue(relationSE.getArgs().first == relArg3);
            Assert::IsTrue(relationSE.getArgs().second == relArg4);

            PQL::PatternClause patternA = { "a(v, _\"x\"_)", false, PatternType::ASSIGN_PATTERN,
                { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::INCLUSIVE_PATTERN, "_x_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::PatternClause patternI = { "not ifs(\"i\", _, _)", true, PatternType::IF_PATTERN,
                { ArgType::SYNONYM, "ifs", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "i", PQL::NON_SYNONYM_ID, AttrType::INVALID },
                { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::PatternClause patternW = { "w(_, _)", false, PatternType::WHILE_PATTERN,
                { ArgType::SYNONYM, "w", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID },
                { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::Argument patArg1 = { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID };
            PQL::Argument patArg2 = { ArgType::INCLUSIVE_PATTERN, "_x_", PQL::NON_SYNONYM_ID, AttrType::INVALID };
            PQL::Argument patArg3 = { ArgType::IDENTIFIER, "i", PQL::NON_SYNONYM_ID, AttrType::INVALID };
            PQL::Argument patArg4 = { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID };
            PQL::Argument patArg5 = { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID };
            PQL::Argument patArg6 = { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID };

            Assert::IsTrue(patternA.asString() == "a(v, _\"x\"_)");
            Assert::IsTrue(patternI.isNegatedClause() == true);
            Assert::IsTrue(patternW.isNegatedClause() == false);
            Assert::IsTrue(patternI.getClauseType() == ClauseType::PATTERN);
            Assert::IsTrue(patternW.getPatternType() == PatternType::WHILE_PATTERN);
            Assert::IsTrue(patternA.getArgs().first == patArg1);
            Assert::IsTrue(patternA.getArgs().second == patArg2);
            Assert::IsTrue(patternI.getArgs().first == patArg3);
            Assert::IsTrue(patternI.getArgs().second == patArg4);
            Assert::IsTrue(patternW.getArgs().first == patArg5);
            Assert::IsTrue(patternW.getArgs().second == patArg6);

            PQL::WithClause equalityLit = { "not 1234 = 4321", true, WithType::LITERAL_EQUAL,
                { ArgType::INTEGER, "1234", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::INTEGER, "4321", PQL::NON_SYNONYM_ID, AttrType::NONE } };
            PQL::WithClause equalityInt = { "69 = l", false, WithType::UNKNOWN_EQUAL,
                { ArgType::INTEGER, "69", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::SYNONYM, "l", PQL::UNSET_SYNONYM_ID, AttrType::NONE } };
            PQL::WithClause equalityIdent = { "not cl.procName = \"bob\"", true, WithType::UNKNOWN_EQUAL,
                { ArgType::ATTRIBUTE, "cl", PQL::UNSET_SYNONYM_ID, AttrType::PROC_NAME },
                { ArgType::IDENTIFIER, "bob", PQL::NON_SYNONYM_ID, AttrType::NONE } };
            PQL::Argument eqArg1 = { ArgType::INTEGER, "1234", PQL::NON_SYNONYM_ID, AttrType::NONE };
            PQL::Argument eqArg2 = { ArgType::INTEGER, "4321", PQL::NON_SYNONYM_ID, AttrType::NONE };
            PQL::Argument eqArg3 = { ArgType::INTEGER, "69", PQL::NON_SYNONYM_ID, AttrType::NONE };
            PQL::Argument eqArg4 = { ArgType::SYNONYM, "l", PQL::UNSET_SYNONYM_ID, AttrType::NONE };
            PQL::Argument eqArg5 = { ArgType::ATTRIBUTE, "cl", PQL::UNSET_SYNONYM_ID, AttrType::PROC_NAME };
            PQL::Argument eqArg6 = { ArgType::IDENTIFIER, "bob", PQL::NON_SYNONYM_ID, AttrType::NONE };

            Assert::IsTrue(equalityLit.asString() == "not 1234 = 4321");
            Assert::IsTrue(equalityInt.isNegatedClause() == false);
            Assert::IsTrue(equalityIdent.isNegatedClause() == true);
            Assert::IsTrue(equalityInt.getClauseType() == ClauseType::WITH);
            Assert::IsTrue(equalityIdent.getWithType() == WithType::UNKNOWN_EQUAL);
            Assert::IsTrue(equalityLit.getArgs().first == eqArg1);
            Assert::IsTrue(equalityLit.getArgs().second == eqArg2);
            Assert::IsTrue(equalityInt.getArgs().first == eqArg3);
            Assert::IsTrue(equalityInt.getArgs().second == eqArg4);
            Assert::IsTrue(equalityIdent.getArgs().first == eqArg5);
            Assert::IsTrue(equalityIdent.getArgs().second == eqArg6);
        }

        TEST_METHOD(setProcedureVariant) {
            PQL::RelationClause relationSS = { "Next*(_, a)", false, RelationType::NEXTT,
                { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID } };
            PQL::RelationClause relationUsesP = { "Uses(_, v)", false, RelationType::USESS,
                { ArgType::SYNONYM, "p", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "x", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::RelationClause relationModifiesP = { "Modifies(p, \"i\")", false, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "p", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "x", PQL::NON_SYNONYM_ID, AttrType::INVALID } };

            Assert::IsFalse(relationSS.setProcedureVariant());
            Assert::IsTrue(relationSS.getRelationType() == RelationType::NEXTT);

            Assert::IsTrue(relationUsesP.getRelationType() == RelationType::USESS);
            Assert::IsTrue(relationUsesP.setProcedureVariant());
            Assert::IsTrue(relationUsesP.getRelationType() == RelationType::USESP);
            Assert::IsFalse(relationUsesP.setProcedureVariant());
            Assert::IsTrue(relationUsesP.getRelationType() == RelationType::USESP);

            Assert::IsTrue(relationModifiesP.getRelationType() == RelationType::MODIFIESS);
            Assert::IsTrue(relationModifiesP.setProcedureVariant());
            Assert::IsTrue(relationModifiesP.getRelationType() == RelationType::MODIFIESP);
            Assert::IsFalse(relationModifiesP.setProcedureVariant());
            Assert::IsTrue(relationModifiesP.getRelationType() == RelationType::MODIFIESP);
        }

        TEST_METHOD(setWithType) {
            PQL::WithClause equalityLit = { "\"orange\" = \"black\"", false, WithType::LITERAL_EQUAL,
                { ArgType::IDENTIFIER, "orange", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::IDENTIFIER, "black", PQL::NON_SYNONYM_ID, AttrType::NONE } };
            PQL::WithClause equalityInt = { "l = 42", false, WithType::UNKNOWN_EQUAL,
               { ArgType::SYNONYM, "l", PQL::UNSET_SYNONYM_ID, AttrType::NONE },
               { ArgType::INTEGER, "42", PQL::NON_SYNONYM_ID, AttrType::NONE } };
            PQL::WithClause equalityIdent = { "\"count\" = rd.varName", false, WithType::UNKNOWN_EQUAL,
                { ArgType::IDENTIFIER, "count", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::ATTRIBUTE, "rd", PQL::UNSET_SYNONYM_ID, AttrType::VAR_NAME } };

            Assert::IsFalse(equalityLit.setWithType(WithType::INTEGER_EQUAL));
            Assert::IsTrue(equalityLit.getWithType() == WithType::LITERAL_EQUAL);

            Assert::IsTrue(equalityInt.getWithType() == WithType::UNKNOWN_EQUAL);
            Assert::IsTrue(equalityInt.setWithType(WithType::INTEGER_EQUAL));
            Assert::IsTrue(equalityInt.getWithType() == WithType::INTEGER_EQUAL);
            Assert::IsFalse(equalityInt.setWithType(WithType::UNKNOWN_EQUAL));
            Assert::IsTrue(equalityInt.getWithType() == WithType::INTEGER_EQUAL);

            Assert::IsTrue(equalityIdent.getWithType() == WithType::UNKNOWN_EQUAL);
            Assert::IsTrue(equalityIdent.setWithType(WithType::IDENTIFIER_EQUAL));
            Assert::IsTrue(equalityIdent.getWithType() == WithType::IDENTIFIER_EQUAL);
            Assert::IsFalse(equalityIdent.setWithType(WithType::UNKNOWN_EQUAL));
            Assert::IsTrue(equalityIdent.getWithType() == WithType::IDENTIFIER_EQUAL);
        }

        TEST_METHOD(synonymIdSetters) {
            PQL::RelationClause relationSynInt = { "Affects*(l, 16)", false, RelationType::AFFECTST,
                { ArgType::SYNONYM, "l", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::INTEGER, "16", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::RelationClause relationIdentSyn = { "Uses(\"main\", v)", false, RelationType::USESP,
                { ArgType::IDENTIFIER, "f", PQL::NON_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID } };
            PQL::RelationClause relationIntWild = { "Follows(1, _)", false, RelationType::FOLLOWS,
                { ArgType::INTEGER, "1", PQL::NON_SYNONYM_ID, AttrType::INVALID },
                { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::RelationClause relationWildIdent = { "Calls(_, \"f\")", false, RelationType::CALLS,
                { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "f", PQL::NON_SYNONYM_ID, AttrType::INVALID } };

            Assert::IsFalse(relationSynInt.setFirstSynonymId(-3203));
            Assert::IsTrue(relationSynInt.setFirstSynonymId(0));
            Assert::IsFalse(relationSynInt.setFirstSynonymId(1));

            Assert::IsFalse(relationIdentSyn.setSecondSynonymId(-1997));
            Assert::IsTrue(relationIdentSyn.setSecondSynonymId(2));
            Assert::IsFalse(relationIdentSyn.setSecondSynonymId(3));

            Assert::IsFalse(relationSynInt.setSecondSynonymId(1101));
            Assert::IsFalse(relationIdentSyn.setFirstSynonymId(1231));
            Assert::IsFalse(relationIntWild.setFirstSynonymId(2030));
            Assert::IsFalse(relationIntWild.setSecondSynonymId(2040));
            Assert::IsFalse(relationWildIdent.setFirstSynonymId(2100));
            Assert::IsFalse(relationWildIdent.setSecondSynonymId(2105));

            PQL::PatternClause patternIdentExact = { "a1(\"i\", \"0\")", false, PatternType::ASSIGN_PATTERN,
                { ArgType::SYNONYM, "a1", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::IDENTIFIER, "i", PQL::NON_SYNONYM_ID, AttrType::INVALID },
                { ArgType::EXACT_PATTERN, "_0_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::PatternClause patternVarWild = { "a2(v, _)", false, PatternType::ASSIGN_PATTERN,
                { ArgType::SYNONYM, "a2", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };
            PQL::PatternClause patternWildInclusive = { "a3(_, _\"i + 1\"_)", false, PatternType::ASSIGN_PATTERN,
                { ArgType::SYNONYM, "a3", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID },
                { ArgType::INCLUSIVE_PATTERN, "_(i+1)_", PQL::NON_SYNONYM_ID, AttrType::INVALID } };

            Assert::IsFalse(patternIdentExact.setPatternSynonymId(-995));
            Assert::IsTrue(patternIdentExact.setPatternSynonymId(4));
            Assert::IsFalse(patternIdentExact.setPatternSynonymId(5));

            Assert::IsFalse(patternVarWild.setFirstSynonymId(-999));
            Assert::IsTrue(patternVarWild.setFirstSynonymId(6));
            Assert::IsFalse(patternVarWild.setFirstSynonymId(7));

            Assert::IsFalse(patternIdentExact.setFirstSynonymId(2106));
            Assert::IsFalse(patternWildInclusive.setFirstSynonymId(3203));

            PQL::WithClause equalityIntSyn = { "369 = l", false, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, "369", PQL::NON_SYNONYM_ID, AttrType::NONE },
                { ArgType::SYNONYM, "l", PQL::UNSET_SYNONYM_ID, AttrType::NONE } };
            PQL::WithClause equalitySynInt = { "pizzaHut = 62353535", false, WithType::INTEGER_EQUAL,
                { ArgType::SYNONYM, "pizzaHut", PQL::UNSET_SYNONYM_ID, AttrType::NONE },
                { ArgType::INTEGER, "62353535", PQL::NON_SYNONYM_ID, AttrType::NONE } };
            PQL::WithClause equalityAttrIdent = { "show.procName = \"BROOKLYN99\"", false, WithType::IDENTIFIER_EQUAL,
               { ArgType::ATTRIBUTE, "show", PQL::UNSET_SYNONYM_ID, AttrType::PROC_NAME },
               { ArgType::IDENTIFIER, "BROOKLYN99", PQL::NON_SYNONYM_ID, AttrType::NONE } };
            PQL::WithClause equalityIdentAttr = { "\"COVID19\" = kills.value", false, WithType::IDENTIFIER_EQUAL,
               { ArgType::IDENTIFIER, "COVID19", PQL::NON_SYNONYM_ID, AttrType::NONE },
               { ArgType::SYNONYM, "kills", PQL::UNSET_SYNONYM_ID, AttrType::VALUE } };

            Assert::IsFalse(equalitySynInt.setLeftSynonymId(-314159));
            Assert::IsTrue(equalitySynInt.setLeftSynonymId(6));
            Assert::IsFalse(equalitySynInt.setLeftSynonymId(7));
            Assert::IsFalse(equalityAttrIdent.setLeftSynonymId(-271828));
            Assert::IsTrue(equalityAttrIdent.setLeftSynonymId(8));
            Assert::IsFalse(equalityAttrIdent.setLeftSynonymId(9));

            Assert::IsFalse(equalityIntSyn.setRightSynonymId(-404));
            Assert::IsTrue(equalityIntSyn.setRightSynonymId(10));
            Assert::IsFalse(equalityIntSyn.setRightSynonymId(11));
            Assert::IsFalse(equalityIdentAttr.setRightSynonymId(-400));
            Assert::IsTrue(equalityIdentAttr.setRightSynonymId(12));
            Assert::IsFalse(equalityIdentAttr.setRightSynonymId(13));

            Assert::IsFalse(equalitySynInt.setRightSynonymId(3210));
            Assert::IsFalse(equalityIntSyn.setLeftSynonymId(3230));
            Assert::IsFalse(equalityAttrIdent.setRightSynonymId(4231));
            Assert::IsFalse(equalityIdentAttr.setLeftSynonymId(4232));
        }

        TEST_METHOD(queryEquality) {
            std::string queryString = "prog_line l; assign a; while w; call cl; Select <w, a, cl.procName> such that Follows*(1, l) and Next*(a, w) and Parent(w, cl) pattern a(v, \"0\") and w(v, _) with cl.procName = \"solve\" and w.stmt# = l";

            PQL::Query query = {
                PQL::STATUS_SUCCESS, queryString, true,
                {   // List of return types in tuple
                    { "w", PQL::UNSET_SYNONYM_ID, AttrType::NONE },
                    { "a", PQL::UNSET_SYNONYM_ID, AttrType::NONE },
                    { "cl", PQL::UNSET_SYNONYM_ID, AttrType::PROC_NAME }
                },
                {   // List of declared synonyms
                    { "l", DesignEntity::PROG_LINE },
                    { "a", DesignEntity::ASSIGN },
                    { "w", DesignEntity::WHILE },
                    { "cl", DesignEntity::CALL }
                },
                {   // List of parsed relation clauses
                    { "Follows*(1, l)", false, RelationType::FOLLOWST,
                        { ArgType::INTEGER, "1", PQL::NON_SYNONYM_ID, AttrType::INVALID },
                        { ArgType::SYNONYM, "l", PQL::UNSET_SYNONYM_ID, AttrType::INVALID } },
                    { "not Next*(a, w)", true, RelationType::NEXTT,
                        { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                        { ArgType::SYNONYM, "w", PQL::UNSET_SYNONYM_ID, AttrType::INVALID } },
                    { "Parent(w, cl)", false, RelationType::PARENT,
                        { ArgType::SYNONYM, "w", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                        { ArgType::SYNONYM, "cl", PQL::UNSET_SYNONYM_ID, AttrType::INVALID } }
                },
                {   // List of parsed pattern clauses
                    { "not a(v, \"0\")", true, PatternType::ASSIGN_PATTERN,
                        { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                        { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                        { ArgType::EXACT_PATTERN, "0", PQL::NON_SYNONYM_ID, AttrType::INVALID } },
                    { "w(v, _)", false, PatternType::WHILE_PATTERN,
                        { ArgType::SYNONYM, "w", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                        { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                        { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID } }
                },
                {   // List of parsed with (equality) clauses
                    { "cl.procName = \"solve\"", false, WithType::IDENTIFIER_EQUAL,
                        { ArgType::ATTRIBUTE, "cl", PQL::UNSET_SYNONYM_ID, AttrType::PROC_NAME },
                        { ArgType::IDENTIFIER, "solve", PQL::NON_SYNONYM_ID, AttrType::NONE } },
                    { "not w.stmt# = l", true, WithType::INTEGER_EQUAL,
                        { ArgType::ATTRIBUTE, "w", PQL::UNSET_SYNONYM_ID, AttrType::STMT_NUM },
                        { ArgType::SYNONYM, "l", PQL::UNSET_SYNONYM_ID, AttrType::NONE } }
                }
            };

            // C++ compiler will automatically generate the copy assignment operator (operator=)
            // for all if not defined explicitly
            PQL::Query queryCopy = query;

            Assert::IsTrue(query == query);
            Assert::IsTrue(query == queryCopy);

            // Modify contents of copy to deviate from the original in exactly one way for the
            // equality comparison to fail, for all possible ways to do so

            PQL::Query diffStatus = query;
            diffStatus.status = PQL::STATUS_OK;

            PQL::Query diffQueryString = query;
            diffQueryString.queryString = "";

            PQL::Query diffBool = query;
            diffBool.returnsBool = false;

            PQL::Query missingTarget = query;
            missingTarget.targetEntities.pop_back();

            PQL::Query reorderedTargets = query;
            std::swap(reorderedTargets.targetEntities.at(0), reorderedTargets.targetEntities.at(2));

            PQL::Query modifiedTarget = query;
            modifiedTarget.targetEntities.at(1).attrType = AttrType::STMT_NUM;

            PQL::Query extraTarget = query;
            extraTarget.targetEntities.push_back({ "l", PQL::UNSET_SYNONYM_ID, AttrType::NONE });

            PQL::Query missingMapping = query;
            missingMapping.synonymTable.erase("w");

            PQL::Query modifiedMappingKey = missingMapping;
            modifiedMappingKey.synonymTable["w2"] = DesignEntity::WHILE;

            PQL::Query modifiedMappingValue = query;
            modifiedMappingValue.synonymTable.at("cl") = DesignEntity::CONSTANT;

            PQL::Query extraMapping = query;
            extraMapping.synonymTable.insert({ "rd", DesignEntity::READ });

            PQL::Query missingRelation = query;
            missingRelation.relations.erase(missingRelation.relations.begin() + 1);

            PQL::Query reorderedRelations = query;
            std::swap(reorderedRelations.relations.at(0), reorderedRelations.relations.at(1));

            // Not possible to arbitrarily modify RelationClause instance after created
            PQL::Query modifiedRelation = missingRelation;
            modifiedRelation.relations.insert(
                modifiedRelation.relations.begin() + 1, { "not Next*(a, w)", true, RelationType::NEXT,
                    { ArgType::SYNONYM, "a", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                    { ArgType::SYNONYM, "w", PQL::UNSET_SYNONYM_ID, AttrType::INVALID } });

            PQL::Query extraRelation = query;
            extraRelation.relations.push_back({ "Uses(\"main\", v)", false, RelationType::USESP,
                    { ArgType::IDENTIFIER, "main", PQL::NON_SYNONYM_ID, AttrType::INVALID },
                    { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID } });

            PQL::Query missingPattern = query;
            missingPattern.patterns.pop_back();

            PQL::Query reorderedPatterns = query;
            std::swap(reorderedPatterns.patterns.at(0), reorderedPatterns.patterns.at(1));

            // Not possible to arbitrarily modify PatternClause instance after created
            PQL::Query modifiedPattern = missingPattern;
            modifiedPattern.patterns.push_back({ "w(v, _)", false, PatternType::WHILE_PATTERN,
                { ArgType::SYNONYM, "w2", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID } });

            PQL::Query extraPattern = query;
            extraPattern.patterns.insert(
                extraPattern.patterns.begin(), { "not ifs(v, _)", true, PatternType::IF_PATTERN,
                { ArgType::SYNONYM, "ifs", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::SYNONYM, "v", PQL::UNSET_SYNONYM_ID, AttrType::INVALID },
                { ArgType::WILDCARD, "_", PQL::NON_SYNONYM_ID, AttrType::INVALID } });

            PQL::Query missingEquality = query;
            missingEquality.equalities.erase(missingEquality.equalities.begin());

            PQL::Query reorderedEqualities = query;
            std::swap(reorderedEqualities.equalities.at(0), reorderedEqualities.equalities.at(1));

            PQL::Query modifiedEquality = missingEquality;
            modifiedEquality.equalities.insert(
                modifiedEquality.equalities.begin(), { "cl.procName = \"solve\"", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, "cl", PQL::UNSET_SYNONYM_ID, AttrType::STMT_NUM },
                    { ArgType::IDENTIFIER, "solve", PQL::NON_SYNONYM_ID, AttrType::NONE } });

            PQL::Query extraEquality = query;
            extraEquality.equalities.insert(
                extraEquality.equalities.begin() + 1, { "not \"COVID19\" = \"sux\"", true, WithType::LITERAL_EQUAL,
                    { ArgType::ATTRIBUTE, "cl", PQL::UNSET_SYNONYM_ID, AttrType::PROC_NAME },
                    { ArgType::IDENTIFIER, "solve", PQL::NON_SYNONYM_ID, AttrType::NONE } });

            std::vector<PQL::Query> diffQueries = {
                diffStatus, diffQueryString, diffBool,
                missingTarget, reorderedTargets, modifiedTarget, extraTarget,
                missingMapping, modifiedMappingKey, modifiedMappingValue, extraMapping,
                missingRelation, reorderedRelations, modifiedRelation, extraRelation,
                missingPattern, reorderedPatterns, modifiedPattern, extraPattern,
                missingEquality, reorderedEqualities, modifiedEquality, extraEquality
            };

            for (auto diff : diffQueries) {
                Assert::IsFalse(query == diff);
            }
        }
    };

}
