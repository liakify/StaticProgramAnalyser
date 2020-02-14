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

		TEST_METHOD(AnalyseTest) {
			Assert::AreEqual(0, Parser::analyse(VALID_SRC));
			Assert::ExpectException<std::invalid_argument>([this] {Parser::analyse(INVALID_SRC); });
		}

		TEST_METHOD(ParserType) {
			Parser::Parser expParser = Parser::Parser();
			Parser::Parser simpleParser1 = Parser::Parser(VALID_SRC, PKB::PKB());
			Parser::Parser simpleParser2 = Parser::Parser(INVALID_SRC, PKB::PKB());
			
			Assert::ExpectException<std::logic_error>([&expParser] {expParser.parse(); });
			auto f = [this, &simpleParser1] {simpleParser1.parseExpression(INVALID_SRC); };
			Assert::ExpectException<std::logic_error>(f);
			auto g = [this, &simpleParser2] {simpleParser2.parseExpression(VALID_SRC); };
			Assert::ExpectException<std::logic_error>(g);
			auto h = [this, &expParser] {expParser.parseExpression(INVALID_EXP); };
			Assert::ExpectException<std::invalid_argument>(h);
			Assert::ExpectException<std::invalid_argument>([&simpleParser2] {simpleParser2.parse(); });

		}
		TEST_METHOD(ExprParser) {
			Parser::Parser expParser = Parser::Parser();
			Expression exp = expParser.parseExpression(VALID_EXP);
			std::string expected = "((((((x+1)*2)%3)/(zz+9))+10)-(5*y))";
			Assert::AreEqual(expected, exp.getStr());
			Expression exp2 = expParser.parseExpression(VALID_EXP2);
			Assert::AreEqual(expected, exp2.getStr());
		}
	};
}