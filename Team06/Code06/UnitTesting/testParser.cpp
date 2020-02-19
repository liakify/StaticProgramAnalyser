#include "stdafx.h"
#include "Parser.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestParser) {
		std::vector<std::string> VALID_SRC = {
			"procedure p{read x;}",
			"procedure p{print x;} procedure pp{call p;eo=n;}",
			"procedure p{x = c+3;  while((x<4) && (x/3 == 4)) {print x;} }",
			"procedure asdf{z=3+c *6;    if(!(x==y)) then {print x;} else{read y;} }"
		};
		std::vector<std::string> INVALID_SRC = {
			"",
			"procedure p{}",
			"procedure p{read 1;}",
			"Procedure p{x = c+3;  while((x<4) && (x/3 == 4)) {print x;} }",
			"procedure asdf{z=3+c *6;    iF(!(x==y)) then {print x;} else{read y;} }"
		};
		std::string VALID_EXP_CONST = "1";
		std::string VALID_EXP_VAR = "bb";
		std::string VALID_EXP = "(x+1)*2%3/(zz+9)+10-5*y";
		std::string VALID_EXP2 = "(  ( x+1)*2%3/( zz+ 9)  +10 -(5*y))";
		std::string INVALID_EXP = "(x+1)*2%3/(zz+9)+10-5*";
		FrontEnd::Parser parser = FrontEnd::Parser();

		TEST_METHOD(ParseSimpleTest) {
			try {
				for (std::string src : VALID_SRC) {
					parser.parseSimple(src);
				}
			}
			catch (std::invalid_argument&) {
				Assert::IsTrue(false);
			}
			for (std::string src : INVALID_SRC) {
				Assert::ExpectException<std::invalid_argument>([this, &src] {parser.parseSimple(src); });
			}
		}

		TEST_METHOD(ParseExpressionTest) {
			Expression exp = parser.parseExpression(VALID_EXP_CONST);
			Assert::AreEqual(VALID_EXP_CONST, exp.getStr());
			Expression exp2 = parser.parseExpression(VALID_EXP_VAR);
			Assert::AreEqual(VALID_EXP_VAR, exp2.getStr());

			std::string expected = "((((((x+1)*2)%3)/(zz+9))+10)-(5*y))";
			Expression exp3 = parser.parseExpression(VALID_EXP);
			Assert::AreEqual(expected, exp3.getStr());
			Expression exp4 = parser.parseExpression(VALID_EXP2);
			Assert::AreEqual(expected, exp4.getStr());
		}
	};
}