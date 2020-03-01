#include "stdafx.h"
#include "Parser.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestParser) {
		std::vector<std::string> VALID_SRC = {
			"   procedure p{read x;}\n   ",
			"\nprocedure p{print x;} procedure pp{call p;eo=n;}",
			"procedure p3245{\n while(1==1) {print z;} }",
			"procedure p{x = c+3*adfgf+25434; \n while(!((x!=4) && (x/3 == 4))) {print z;} }",
			"procedure p{x = c+3; \n while((x>4) && (x/3 < 4)) {print x;} }",
			"procedure asdf{z=3+c *6;    if( (!(x<=y)) \n || (y>=z)) then {print x;} else{read y;} }"
		};
		std::vector<std::string> INVALID_SRC = {
			"",
			"proc ",
			"procedure p{}",
			"procedure p{read 1;}",
			"procedure p{1x = 1;}",
			"procedure p{x = 1;} asdf",
			"procedure p{x = 1;} procedure p{y = 2;} ",
			"Procedure p{x = c+3;  while((x<4) && (x/3 == 4)) {print x;} }",
			"procedure pee{while((x<4) && (x/3 == 4)) {} }",
			"procedure pee{while(x<4 && x/3 == 4) {e=2;} }",
			"procedure pee{while(!x<4) {e=2;} }",
			"procedure pee{while(y) {e=2;} }",
			"procedure pee{while((1==y)) {e=2;} }",
			"procedure asdf{z=3+c *6;    if(!(x==y)) then {} else{read y;} }",
			"procedure asdf{z=3+c *6;    if(!(x==y)) then {read z;} }",
			"procedure asdf{z=3+c *6;    if((!(x==y))) then {print x;} else{read y;} }"
		};
		std::string VALID_EXP_CONST = "1";
		std::string VALID_EXP_VAR = "bb";
		std::string VALID_EXP = "(x+1)*2%3/(zz+9)+10-5*y";
		std::string VALID_EXP2 = "(  ( x+1)*2%3/( zz+ 9)  +10 -(5*y))";
		std::string INVALID_EXP = "(x+1)*2%3/(zz+9)+10-5*";
		std::string INVALID_EXP2 = "1x";
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
			Assert::AreEqual(VALID_EXP_CONST, parser.parseExpression(VALID_EXP_CONST));
			Assert::AreEqual(VALID_EXP_VAR, parser.parseExpression(VALID_EXP_VAR));

			std::string expected = "((((((x+1)*2)%3)/(zz+9))+10)-(5*y))";
			Assert::AreEqual(expected, parser.parseExpression(VALID_EXP));
			Assert::AreEqual(expected, parser.parseExpression(VALID_EXP2));

			Assert::ExpectException<std::invalid_argument>([this] {parser.parseExpression(INVALID_EXP); });
			Assert::ExpectException<std::invalid_argument>([this] {parser.parseExpression(INVALID_EXP2); });
		}
	};
}