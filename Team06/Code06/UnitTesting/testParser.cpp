#include "stdafx.h"
#include "Parser.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestParser) {
		std::string VALID_SRC = "procedure p{read x;}";
		std::string INVALID_SRC = "procedure p{read 1;}";
		std::string VALID_EXP = "(x+1)*2%3/(zz+9)+10-5*y";
		std::string VALID_EXP2 = "(  ( x+1)*2%3/( zz+ 9)  +10 -(5*y))";
		std::string INVALID_EXP = "(x+1)*2%3/(zz+9)+10-5*";
		FrontEnd::Parser parser = FrontEnd::Parser();

		TEST_METHOD(ParseSimpleTest) {
			PKB::PKB pkb = PKB::PKB();
			try {
				parser.parseSimple(VALID_SRC, pkb);
			}
			catch (std::invalid_argument&) {
				Assert::IsTrue(false);
			}
			Assert::ExpectException<std::invalid_argument>([this, &pkb] {parser.parseSimple(INVALID_SRC, pkb); });
		}

		TEST_METHOD(ParseExpressionTest) {
			FrontEnd::Parser expParser = FrontEnd::Parser();
			Expression exp = expParser.parseExpression(VALID_EXP);
			std::string expected = "((((((x+1)*2)%3)/(zz+9))+10)-(5*y))";
			Assert::AreEqual(expected, exp.getStr());
			Expression exp2 = expParser.parseExpression(VALID_EXP2);
			Assert::AreEqual(expected, exp2.getStr());
		}
	};
}