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
        std::vector<std::string> VALID_EXP = {
            "1",
            "bb",
            "(x+1)*2%3/(zz+9)+10-5*y",
            "(  ( x+1)*2%3/( zz+ 9)  +10 -(5*y))",
            "x0 + y0 + x * x - y * y",
            "x-x-x-x-x-x",
            "x-x-x-x-x/x",
            "x-x-x-x/x-x",
            "x-x-x-x/x/x",
            "x-x-x/x-x-x",
            "x-x-x/x-x/x",
            "x-x-x/x/x-x",
            "x-x-x/x/x/x",
            "x-x/x-x-x-x",
            "x-x/x-x-x/x",
            "x-x/x-x/x-x",
            "x-x/x-x/x/x",
            "x-x/x/x-x-x",
            "x-x/x/x-x/x",
            "x-x/x/x/x-x",
            "x-x/x/x/x/x",
            "x/x-x-x-x-x",
            "x/x-x-x-x/x",
            "x/x-x-x/x-x",
            "x/x-x-x/x/x",
            "x/x-x/x-x-x",
            "x/x-x/x-x/x",
            "x/x-x/x/x-x",
            "x/x-x/x/x/x",
            "x/x/x-x-x-x",
            "x/x/x-x-x/x",
            "x/x/x-x/x-x",
            "x/x/x-x/x/x",
            "x/x/x/x-x-x",
            "x/x/x/x-x/x",
            "x/x/x/x/x-x",
            "x/x/x/x/x/x"
        };
        std::vector<std::string> EXPECTED_EXP_STR = {
            "1",
            "bb",
            "((((((x+1)*2)%3)/(zz+9))+10)-(5*y))",
            "((((((x+1)*2)%3)/(zz+9))+10)-(5*y))",
            "(((x0+y0)+(x*x))-(y*y))",
            "(((((x-x)-x)-x)-x)-x)",
            "((((x-x)-x)-x)-(x/x))",
            "((((x-x)-x)-(x/x))-x)",
            "(((x-x)-x)-((x/x)/x))",
            "((((x-x)-(x/x))-x)-x)",
            "(((x-x)-(x/x))-(x/x))",
            "(((x-x)-((x/x)/x))-x)",
            "((x-x)-(((x/x)/x)/x))",
            "((((x-(x/x))-x)-x)-x)",
            "(((x-(x/x))-x)-(x/x))",
            "(((x-(x/x))-(x/x))-x)",
            "((x-(x/x))-((x/x)/x))",
            "(((x-((x/x)/x))-x)-x)",
            "((x-((x/x)/x))-(x/x))",
            "((x-(((x/x)/x)/x))-x)",
            "(x-((((x/x)/x)/x)/x))",
            "(((((x/x)-x)-x)-x)-x)",
            "((((x/x)-x)-x)-(x/x))",
            "((((x/x)-x)-(x/x))-x)",
            "(((x/x)-x)-((x/x)/x))",
            "((((x/x)-(x/x))-x)-x)",
            "(((x/x)-(x/x))-(x/x))",
            "(((x/x)-((x/x)/x))-x)",
            "((x/x)-(((x/x)/x)/x))",
            "(((((x/x)/x)-x)-x)-x)",
            "((((x/x)/x)-x)-(x/x))",
            "((((x/x)/x)-(x/x))-x)",
            "(((x/x)/x)-((x/x)/x))",
            "(((((x/x)/x)/x)-x)-x)",
            "((((x/x)/x)/x)-(x/x))",
            "(((((x/x)/x)/x)/x)-x)",
            "(((((x/x)/x)/x)/x)/x)"
        };
        std::vector<std::string> INVALID_EXP = {
            "(x+1)*2%3/(zz+9)+10-5*",
            "1x",
            "(x+1",
            "y-2*z)",
            "a$b",
            "01",
            "(23 + 4) * ((3 - 5)*4))",
            "(23 + 4) * ((3 - 5)*(4)"
        };
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
            for (size_t i = 0; i < VALID_EXP.size(); i++) {
                Assert::AreEqual(EXPECTED_EXP_STR[i], parser.parseExpression(VALID_EXP[i]));
            }
            for (std::string expr : INVALID_EXP) {
                Assert::ExpectException<std::invalid_argument>([this, expr] {parser.parseExpression(expr); });
            }
        }
    };
}