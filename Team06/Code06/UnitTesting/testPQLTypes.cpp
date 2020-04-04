#include "stdafx.h"
#include "PQLTypes.h"

using std::pair;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

    TEST_CLASS(TestPQLTypes) {
    public:
        TEST_METHOD(clauseGetters) {
            PQL::RelationClause relationSS = { "Affects*(1, a)", RelationType::AFFECTS,
                { ArgType::INTEGER, "1" }, { ArgType::SYNONYM, "a" } };
            PQL::RelationClause relationSE = { "Uses(a, \"x\")", RelationType::USESS,
                { ArgType::SYNONYM, "a" }, { ArgType::IDENTIFIER, "x" } };

            Assert::IsTrue(relationSS.asString() == "Affects*(1, a)");
            Assert::IsTrue(relationSE.getClauseType() == ClauseType::RELATION);
            Assert::IsTrue(relationSS.getRelationType() == RelationType::AFFECTS);
            Assert::IsTrue(relationSS.getArgs().first == std::pair<ArgType, StmtEntRef>{ ArgType::INTEGER, "1" });
            Assert::IsTrue(relationSS.getArgs().second == std::pair<ArgType, StmtEntRef>{ ArgType::SYNONYM, "a" });
            Assert::IsTrue(relationSE.getArgs().first == std::pair<ArgType, StmtEntRef>{ ArgType::SYNONYM, "a" });
            Assert::IsTrue(relationSE.getArgs().second == std::pair<ArgType, StmtEntRef>{ ArgType::IDENTIFIER, "x" });

            PQL::PatternClause patternA = { "a(v, _\"x\"_)", PatternType::ASSIGN_PATTERN, "a",
                { ArgType::SYNONYM, "v" }, { ArgType::INCLUSIVE_PATTERN, "_x_" } };
            PQL::PatternClause patternI = { "ifs(\"i\", _, _)", PatternType::IF_PATTERN, "ifs",
                { ArgType::IDENTIFIER, "i" }, { ArgType::WILDCARD, "_" } };
            PQL::PatternClause patternW = { "w(_, _)", PatternType::WHILE_PATTERN, "w",
                { ArgType::WILDCARD, "_" }, { ArgType::WILDCARD, "_" } };

            Assert::IsTrue(patternA.asString() == "a(v, _\"x\"_)");
            Assert::IsTrue(patternI.getClauseType() == ClauseType::PATTERN);
            Assert::IsTrue(patternW.getPatternType() == PatternType::WHILE_PATTERN);
            Assert::IsTrue(patternA.getArgs().first == std::pair<ArgType, EntityRef>{ ArgType::SYNONYM, "v" });
            Assert::IsTrue(patternA.getArgs().second == std::pair<ArgType, Pattern>{ ArgType::INCLUSIVE_PATTERN, "_x_" });
            Assert::IsTrue(patternI.getArgs().first == std::pair<ArgType, EntityRef>{ ArgType::IDENTIFIER, "i" });
            Assert::IsTrue(patternI.getArgs().second == std::pair<ArgType, Pattern>{ ArgType::WILDCARD, "_" });
            Assert::IsTrue(patternW.getArgs().first == std::pair<ArgType, EntityRef>{ ArgType::WILDCARD, "_" });
            Assert::IsTrue(patternW.getArgs().second == std::pair<ArgType, Pattern>{ ArgType::WILDCARD, "_" });

            PQL::WithClause equalityLit = { "1234 = 4321", WithType::LITERAL_EQUAL,
                { ArgType::INTEGER, { "1234", AttrType::NONE } }, { ArgType::INTEGER, { "4321", AttrType::NONE } } };
            PQL::WithClause equalityInt = { "69 = l", WithType::UNKNOWN_EQUAL,
                { ArgType::INTEGER, { "69", AttrType::NONE } }, { ArgType::SYNONYM, { "l", AttrType::NONE } } };
            PQL::WithClause equalityIdent = { "cl.procName = \"bob\"", WithType::UNKNOWN_EQUAL,
                { ArgType::ATTRIBUTE, { "cl", AttrType::PROC_NAME } }, { ArgType::IDENTIFIER, { "bob", AttrType::NONE } } };

            Assert::IsTrue(equalityLit.asString() == "1234 = 4321");
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
            PQL::RelationClause relationSS = { "Next*(_, a)", RelationType::NEXTT,
                { ArgType::WILDCARD, "_" }, { ArgType::SYNONYM, "a" } };
            PQL::RelationClause relationUsesP = { "Uses(_, v)", RelationType::USESS,
                { ArgType::SYNONYM, "p" }, { ArgType::IDENTIFIER, "x" } };
            PQL::RelationClause relationModifiesP = { "Modifies(p, \"i\")", RelationType::MODIFIESS,
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
            PQL::WithClause equalityLit = { "\"orange\" = \"black\"", WithType::LITERAL_EQUAL,
                { ArgType::IDENTIFIER, { "orange", AttrType::NONE } }, { ArgType::IDENTIFIER, { "black", AttrType::NONE } } };
            PQL::WithClause equalityInt = { "l = 42", WithType::UNKNOWN_EQUAL,
               { ArgType::SYNONYM, { "l", AttrType::NONE } }, { ArgType::INTEGER, { "42", AttrType::NONE } } };
            PQL::WithClause equalityIdent = { "\"count\" = rd.varName", WithType::UNKNOWN_EQUAL,
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
    };

}
