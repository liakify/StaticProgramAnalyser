#pragma once
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <array>

class QueryGenerator
{
  int iteration;

  std::list<std::string> listOfSynonyms;

  std::array<std::string, 10> designEntitiesBasic = {
    "stmt", "read", "print", "call", "while", "if",
    "assign", "variable", "constant", "procedure"
  };

  std::array<std::string, 11> designEntitiesAdv = {
    "stmt", "read", "print", "call", "while", "if",
    "assign", "variable", "constant", "procedure", "prog_line"
  };

  std::array<std::string, 6> relRefBasic = {
      "Follows", "Follows*", "Parent", "Parent*",
      "Modifies", "Uses"
  };

  std::array<std::string, 12> relRefAdv = {
      "Follows", "Follows*", "Parent", "Parent*",
      "Modifies", "Uses", "Calls", "Calls*",
      "Next", "NextT", "Affects", "Affects*"
  };

  std::array<std::string, 4> attrNames = {
    "stmt#", "procName", "varName", "value"
  };

  std::array<std::string, 15> symbols = {
    "+", "-", "*", "/", "%", "(", ")",
    "x", "y", "Z", "a", "B2", "1", "2", "3"
  };

  const int HIGHEST_CONSTANT = 50;

  /**
    returns a relRef with randomised arguments
  */
  std::string getRelRef();

  std::string getRelRefArgs();

  /**
     returns ‘”’ IDENT ‘”’ | INTEGER | attrRef | synonym | _
  */
  std::string getRandomArg();
  /**
    returns ‘procName’| ‘varName’ | ‘value’ | ‘stmt#’
  */
  std::string getAttr();

  std::string getRandomName();

  /**
    returns ( LETTER | DIGIT )*
  */
  std::string recursiveGetLetterOrDigit();

  /**
    returns ( suchthat-cl | with-cl | pattern-cl | and suchthat-cl | and with-cl | and pattern-cl )*
  */
  std::string RecursiveGetClauses();

  /**
    returns ( suchthat-cl | with-cl | pattern-cl )
  */
  std::string GetClause();
  /**
    attrCond : attrCompare ( ‘and’ attrCompare )*
  */
  std::string getAttrCond();

  std::string getRecursiveAttrCompare();

  std::string getAttrCompare();

  /**
    patternCond : pattern
  */
  std::string getPatternCond();

  std::string getExpressionSpec();

  std::string getExpr();

  std::string RecursiveGetExpr();

  std::string getRandomSymbol();

  /**
    returns ( ‘and’ pattern )*
  */
  std::string getRecursivePatternCond();

  /**
    returns design-entity : ‘stmt’ | ‘read’ | ‘print’ | ‘call’ | ‘while’ | ‘if’ | ‘assign’
    | ‘variable’ | ‘constant’ | ‘prog_line’ | ‘procedure’
  */
  std::string DeclarationGen();

  // TODO: Implemement tuples
  std::string getResultClause();

public:

  QueryGenerator(int iteration);

  std::string GenerateOne();

  std::string GenerateN(int n);
};

int main() {
  std::ofstream myfile;
  // argument for Query Generator is based on Iteration Number.
  QueryGenerator qg = QueryGenerator(1);

  myfile.open("PQL-1-10000.txt");

  myfile << qg.GenerateN(10000);

  myfile.close();
  return 0;
}
