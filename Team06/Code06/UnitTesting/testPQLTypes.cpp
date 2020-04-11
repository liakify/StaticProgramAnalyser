#include "stdafx.h"

#include "PQLTypes.h"

using std::pair;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

    TEST_CLASS(TestPQLTypes) {
    public:
        TEST_METHOD(relationClauseEquality) {
            PQL::RelationClause relation = { "not Modifies(w, \"i\")", true, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "w" }, { ArgType::IDENTIFIER, "i" } };
            PQL::RelationClause relation2 = { "not Modifies(w, \"i\")", true, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "w" }, { ArgType::IDENTIFIER, "i" } };

            Assert::IsTrue(relation == relation);
            Assert::IsTrue(relation == relation2);

            PQL::RelationClause diffClause = { "not Modifies(w, i)", true, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "w" }, { ArgType::IDENTIFIER, "i" } };
            PQL::RelationClause diffNegation = { "not Modifies(w, \"i\")", false, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "w" }, { ArgType::IDENTIFIER, "i" } };
            PQL::RelationClause diffType = { "not Modifies(w, \"i\")", true, RelationType::USESS,
                { ArgType::SYNONYM, "w" }, { ArgType::IDENTIFIER, "i" } };
            PQL::RelationClause diffArg1 = { "not Modifies(w, \"i\")", true, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "while" }, { ArgType::IDENTIFIER, "i" } };
            PQL::RelationClause diffArg2 = { "not Modifies(w, \"i\")", true, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "w" }, { ArgType::SYNONYM, "i" } };

            Assert::IsFalse(relation == diffClause);
            Assert::IsFalse(relation == diffNegation);
            Assert::IsFalse(relation == diffType);
            Assert::IsFalse(relation == diffArg1);
            Assert::IsFalse(relation == diffArg2);
        }

        TEST_METHOD(patternClauseEquality) {
            PQL::PatternClause pattern = { "a(v, _\"i + 1\"_)", false, PatternType::ASSIGN_PATTERN, "a",
                { ArgType::SYNONYM, "v" }, { ArgType::INCLUSIVE_PATTERN , "_(i+1)_" } };
            PQL::PatternClause pattern2 = { "a(v, _\"i + 1\"_)", false, PatternType::ASSIGN_PATTERN, "a",
                { ArgType::SYNONYM, "v" }, { ArgType::INCLUSIVE_PATTERN , "_(i+1)_" } };

            Assert::IsTrue(pattern == pattern);
            Assert::IsTrue(pattern == pattern2);

            PQL::PatternClause diffClause = { "a(v, \"i + 1\")", false, PatternType::ASSIGN_PATTERN, "a",
                { ArgType::SYNONYM, "v" }, { ArgType::INCLUSIVE_PATTERN , "_(i+1)_" } };
            PQL::PatternClause diffNegation = { "a(v, _\"i + 1\"_)", true, PatternType::ASSIGN_PATTERN, "a",
                { ArgType::SYNONYM, "v" }, { ArgType::INCLUSIVE_PATTERN , "_(i+1)_" } };
            PQL::PatternClause diffType = { "a(v, _\"i + 1\"_)", false, PatternType::IF_PATTERN, "a",
                { ArgType::SYNONYM, "v" }, { ArgType::INCLUSIVE_PATTERN , "_(i+1)_" } };
            PQL::PatternClause diffSynonym = { "a(v, _\"i + 1\"_)", false, PatternType::ASSIGN_PATTERN, "a2",
                { ArgType::SYNONYM, "v" }, { ArgType::INCLUSIVE_PATTERN , "_(i+1)_" } };
            PQL::PatternClause diffTargetArg = { "a(v, _\"i + 1\"_)", false, PatternType::ASSIGN_PATTERN, "a",
                { ArgType::IDENTIFIER, "v" }, { ArgType::INCLUSIVE_PATTERN , "_(i+1)_" } };
            PQL::PatternClause diffPatternArg = { "a(v, _\"i + 1\"_)", false, PatternType::ASSIGN_PATTERN, "a",
                { ArgType::SYNONYM, "v" }, { ArgType::INCLUSIVE_PATTERN , "(i+1)" } };

            Assert::IsFalse(pattern == diffClause);
            Assert::IsFalse(pattern == diffNegation);
            Assert::IsFalse(pattern == diffType);
            Assert::IsFalse(pattern == diffSynonym);
            Assert::IsFalse(pattern == diffTargetArg);
            Assert::IsFalse(pattern == diffPatternArg);
        }

        TEST_METHOD(withClauseEquality) {
            PQL::WithClause equality = { "not 2020 = a.stmt#", true, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, { "2020", AttrType::NONE } },
                { ArgType::ATTRIBUTE, { "a", AttrType::STMT_NUM } } };
            PQL::WithClause equality2 = { "not 2020 = a.stmt#", true, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, { "2020", AttrType::NONE } },
                { ArgType::ATTRIBUTE, { "a", AttrType::STMT_NUM } } };

            Assert::IsTrue(equality == equality);
            Assert::IsTrue(equality == equality2);

            PQL::WithClause diffClause = { "not a.stmt# = 2020", true, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, { "2020", AttrType::NONE } },
                { ArgType::ATTRIBUTE, { "a", AttrType::STMT_NUM } } };
            PQL::WithClause diffNegation = { "not 2020 = a.stmt#", false, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, { "2020", AttrType::NONE } },
                { ArgType::ATTRIBUTE, { "a", AttrType::STMT_NUM } } };
            PQL::WithClause diffType = { "not 2020 = a.stmt#", true, WithType::IDENTIFIER_EQUAL,
                { ArgType::INTEGER, { "2020", AttrType::NONE } },
                { ArgType::ATTRIBUTE, { "a", AttrType::STMT_NUM } } };
            PQL::WithClause diffLHS = { "not 2020 = a.stmt#", true, WithType::INTEGER_EQUAL,
                { ArgType::WILDCARD, { "2020", AttrType::NONE } },
                { ArgType::ATTRIBUTE, { "a", AttrType::STMT_NUM } } };
            PQL::WithClause diffRHSArg = { "not 2020 = a.stmt#", true, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, { "2020", AttrType::NONE } },
                { ArgType::ATTRIBUTE, { "cl", AttrType::STMT_NUM } } };
            PQL::WithClause diffRHSAttrType = { "not 2020 = a.stmt#", true, WithType::INTEGER_EQUAL,
                { ArgType::INTEGER, { "2020", AttrType::NONE } },
                { ArgType::ATTRIBUTE, { "a", AttrType::VALUE } } };

            Assert::IsFalse(equality == diffClause);
            Assert::IsFalse(equality == diffNegation);
            Assert::IsFalse(equality == diffType);
            Assert::IsFalse(equality == diffLHS);
            Assert::IsFalse(equality == diffRHSArg);
            Assert::IsFalse(equality == diffRHSAttrType);
        }

        TEST_METHOD(clauseGetters) {
            PQL::RelationClause relationSS = { "Affects*(1, a)", false, RelationType::AFFECTS,
                { ArgType::INTEGER, "1" }, { ArgType::SYNONYM, "a" } };
            PQL::RelationClause relationSE = { "not Uses(a, \"x\")", true, RelationType::USESS,
                { ArgType::SYNONYM, "a" }, { ArgType::IDENTIFIER, "x" } };

            Assert::IsTrue(relationSS.asString() == "Affects*(1, a)");
            Assert::IsTrue(relationSS.isNegatedClause() == false);
            Assert::IsTrue(relationSE.isNegatedClause() == true);
            Assert::IsTrue(relationSE.getClauseType() == ClauseType::RELATION);
            Assert::IsTrue(relationSS.getRelationType() == RelationType::AFFECTS);
            Assert::IsTrue(relationSS.getArgs().first == std::pair<ArgType, StmtEntRef>{ ArgType::INTEGER, "1" });
            Assert::IsTrue(relationSS.getArgs().second == std::pair<ArgType, StmtEntRef>{ ArgType::SYNONYM, "a" });
            Assert::IsTrue(relationSE.getArgs().first == std::pair<ArgType, StmtEntRef>{ ArgType::SYNONYM, "a" });
            Assert::IsTrue(relationSE.getArgs().second == std::pair<ArgType, StmtEntRef>{ ArgType::IDENTIFIER, "x" });

            PQL::PatternClause patternA = { "a(v, _\"x\"_)", false, PatternType::ASSIGN_PATTERN, "a",
                { ArgType::SYNONYM, "v" }, { ArgType::INCLUSIVE_PATTERN, "_x_" } };
            PQL::PatternClause patternI = { "not ifs(\"i\", _, _)", true, PatternType::IF_PATTERN, "ifs",
                { ArgType::IDENTIFIER, "i" }, { ArgType::WILDCARD, "_" } };
            PQL::PatternClause patternW = { "w(_, _)", false, PatternType::WHILE_PATTERN, "w",
                { ArgType::WILDCARD, "_" }, { ArgType::WILDCARD, "_" } };

            Assert::IsTrue(patternA.asString() == "a(v, _\"x\"_)");
            Assert::IsTrue(patternI.isNegatedClause() == true);
            Assert::IsTrue(patternW.isNegatedClause() == false);
            Assert::IsTrue(patternI.getClauseType() == ClauseType::PATTERN);
            Assert::IsTrue(patternW.getPatternType() == PatternType::WHILE_PATTERN);
            Assert::IsTrue(patternA.getArgs().first == std::pair<ArgType, EntityRef>{ ArgType::SYNONYM, "v" });
            Assert::IsTrue(patternA.getArgs().second == std::pair<ArgType, Pattern>{ ArgType::INCLUSIVE_PATTERN, "_x_" });
            Assert::IsTrue(patternI.getArgs().first == std::pair<ArgType, EntityRef>{ ArgType::IDENTIFIER, "i" });
            Assert::IsTrue(patternI.getArgs().second == std::pair<ArgType, Pattern>{ ArgType::WILDCARD, "_" });
            Assert::IsTrue(patternW.getArgs().first == std::pair<ArgType, EntityRef>{ ArgType::WILDCARD, "_" });
            Assert::IsTrue(patternW.getArgs().second == std::pair<ArgType, Pattern>{ ArgType::WILDCARD, "_" });

            PQL::WithClause equalityLit = { "not 1234 = 4321", true, WithType::LITERAL_EQUAL,
                { ArgType::INTEGER, { "1234", AttrType::NONE } }, { ArgType::INTEGER, { "4321", AttrType::NONE } } };
            PQL::WithClause equalityInt = { "69 = l", false, WithType::UNKNOWN_EQUAL,
                { ArgType::INTEGER, { "69", AttrType::NONE } }, { ArgType::SYNONYM, { "l", AttrType::NONE } } };
            PQL::WithClause equalityIdent = { "not cl.procName = \"bob\"", true, WithType::UNKNOWN_EQUAL,
                { ArgType::ATTRIBUTE, { "cl", AttrType::PROC_NAME } }, { ArgType::IDENTIFIER, { "bob", AttrType::NONE } } };

            Assert::IsTrue(equalityLit.asString() == "not 1234 = 4321");
            Assert::IsTrue(equalityInt.isNegatedClause() == false);
            Assert::IsTrue(equalityIdent.isNegatedClause() == true);
            Assert::IsTrue(equalityInt.getClauseType() == ClauseType::WITH);
            Assert::IsTrue(equalityIdent.getWithType() == WithType::UNKNOWN_EQUAL);
            Assert::IsTrue(equalityLit.getArgs().first == std::pair<ArgType, Ref>{ ArgType::INTEGER, { "1234", AttrType::NONE } });
            Assert::IsTrue(equalityLit.getArgs().second == std::pair<ArgType, Ref>{ ArgType::INTEGER, { "4321", AttrType::NONE } });
            Assert::IsTrue(equalityInt.getArgs().first == std::pair<ArgType, Ref>{ ArgType::INTEGER, { "69", AttrType::NONE } });
            Assert::IsTrue(equalityInt.getArgs().second == std::pair<ArgType, Ref>{ ArgType::SYNONYM, { "l", AttrType::NONE } });
            Assert::IsTrue(equalityIdent.getArgs().first == std::pair<ArgType, Ref>{ ArgType::ATTRIBUTE, { "cl", AttrType::PROC_NAME } });
            Assert::IsTrue(equalityIdent.getArgs().second == std::pair<ArgType, Ref>{ ArgType::IDENTIFIER, { "bob", AttrType::NONE } });
        }

        TEST_METHOD(setProcedureVariant) {
            PQL::RelationClause relationSS = { "Next*(_, a)", false, RelationType::NEXTT,
                { ArgType::WILDCARD, "_" }, { ArgType::SYNONYM, "a" } };
            PQL::RelationClause relationUsesP = { "Uses(_, v)", false, RelationType::USESS,
                { ArgType::SYNONYM, "p" }, { ArgType::IDENTIFIER, "x" } };
            PQL::RelationClause relationModifiesP = { "Modifies(p, \"i\")", false, RelationType::MODIFIESS,
                { ArgType::SYNONYM, "p" }, { ArgType::IDENTIFIER, "x" } };

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
                { ArgType::IDENTIFIER, { "orange", AttrType::NONE } }, { ArgType::IDENTIFIER, { "black", AttrType::NONE } } };
            PQL::WithClause equalityInt = { "l = 42", false, WithType::UNKNOWN_EQUAL,
               { ArgType::SYNONYM, { "l", AttrType::NONE } }, { ArgType::INTEGER, { "42", AttrType::NONE } } };
            PQL::WithClause equalityIdent = { "\"count\" = rd.varName", false, WithType::UNKNOWN_EQUAL,
                { ArgType::IDENTIFIER, { "count", AttrType::NONE } }, { ArgType::ATTRIBUTE, { "rd", AttrType::VAR_NAME } } };

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

        TEST_METHOD(queryEquality) {
            std::string queryString = "prog_line l; assign a; while w; call cl; Select <w, a, cl.procName> such that Follows*(1, l) and Next*(a, w) and Parent(w, cl) pattern a(v, \"0\") and w(v, _) with cl.procName = \"solve\" and w.stmt# = l";

            PQL::Query query = {
                PQL::STATUS_SUCCESS, queryString, true,
                {   // List of return types in tuple
                    { "w", AttrType::NONE },
                    { "a", AttrType::NONE },
                    { "cl", AttrType::PROC_NAME }
                },
                {   // List of declared synonyms
                    { "l", DesignEntity::PROG_LINE },
                    { "a", DesignEntity::ASSIGN },
                    { "w", DesignEntity::WHILE },
                    { "cl", DesignEntity::CALL }
                },
                {   // List of parsed relation clauses
                    { "Follows*(1, l)", false, RelationType::FOLLOWST,
                        { ArgType::INTEGER, "1" }, { ArgType::SYNONYM, "l" } },
                    { "not Next*(a, w)", true, RelationType::NEXTT,
                        { ArgType::SYNONYM, "a" }, { ArgType::SYNONYM, "w" } },
                    { "Parent(w, cl)", false, RelationType::PARENT,
                        { ArgType::SYNONYM, "w" }, { ArgType::SYNONYM, "cl" } }
                },
                {   // List of parsed pattern clauses
                    { "not a(v, \"0\")", true, PatternType::ASSIGN_PATTERN, "a",
                        { ArgType::SYNONYM, "v" }, { ArgType::EXACT_PATTERN, "0" } },
                    { "w(v, _)", false, PatternType::WHILE_PATTERN, "w",
                        { ArgType::SYNONYM, "v" }, { ArgType::WILDCARD, "_" } }
                },
                {   // List of parsed with (equality) clauses
                    { "cl.procName = \"solve\"", false, WithType::IDENTIFIER_EQUAL,
                        { ArgType::ATTRIBUTE, { "cl", AttrType::PROC_NAME } }, { ArgType::IDENTIFIER, { "solve", AttrType::NONE } } },
                    { "not w.stmt# = l", true, WithType::INTEGER_EQUAL,
                        { ArgType::ATTRIBUTE, { "w", AttrType::STMT_NUM } }, { ArgType::SYNONYM, { "l", AttrType::NONE } } }
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
            modifiedTarget.targetEntities.at(1).second = AttrType::STMT_NUM;

            PQL::Query extraTarget = query;
            extraTarget.targetEntities.push_back({ "l", AttrType::NONE });

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
                modifiedRelation.relations.begin() + 1,
                { "not Next*(a, w)", true, RelationType::NEXT, { ArgType::SYNONYM, "a" }, { ArgType::SYNONYM, "w" } });

            PQL::Query extraRelation = query;
            extraRelation.relations.push_back(
                { "Uses(\"main\", v)", false, RelationType::USESP, { ArgType::IDENTIFIER, "main"}, { ArgType::SYNONYM, "v" } });

            PQL::Query missingPattern = query;
            missingPattern.patterns.pop_back();

            PQL::Query reorderedPatterns = query;
            std::swap(reorderedPatterns.patterns.at(0), reorderedPatterns.patterns.at(1));

            // Not possible to arbitrarily modify PatternClause instance after created
            PQL::Query modifiedPattern = missingPattern;
            modifiedPattern.patterns.push_back(
                { "w(v, _)", false, PatternType::WHILE_PATTERN, "w2", { ArgType::SYNONYM, "v" }, { ArgType::WILDCARD, "_" } });

            PQL::Query extraPattern = query;
            extraPattern.patterns.insert(
                extraPattern.patterns.begin(),
                { "not ifs(v, _)", true, PatternType::IF_PATTERN, "ifs", { ArgType::SYNONYM, "v" }, { ArgType::WILDCARD, "_" } });

            PQL::Query missingEquality = query;
            missingEquality.equalities.erase(missingEquality.equalities.begin());

            PQL::Query reorderedEqualities = query;
            std::swap(reorderedEqualities.equalities.at(0), reorderedEqualities.equalities.at(1));

            PQL::Query modifiedEquality = missingEquality;
            modifiedEquality.equalities.insert(
                modifiedEquality.equalities.begin(),
                { "cl.procName = \"solve\"", false, WithType::IDENTIFIER_EQUAL,
                    { ArgType::ATTRIBUTE, { "cl", AttrType::STMT_NUM } }, { ArgType::IDENTIFIER, { "solve", AttrType::NONE } } });

            PQL::Query extraEquality = query;
            extraEquality.equalities.insert(
                extraEquality.equalities.begin() + 1,
                { "not \"COVID19\" = \"sux\"", true, WithType::LITERAL_EQUAL,
                    { ArgType::ATTRIBUTE, { "cl", AttrType::PROC_NAME } }, { ArgType::IDENTIFIER, { "solve", AttrType::NONE } } });

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
