#include "QueryGenerator.h"

/**
returns a relRef with randomised arguments
*/

std::string QueryGenerator::getRelRef() {

  int relRefId;
  std::string thisRelRef;

  switch (iteration) {
  case 1:
    relRefId = rand() % relRefBasic.size();
    thisRelRef = relRefBasic[relRefId];
    break;
  case 2:
  case 3:
    relRefId = rand() % relRefAdv.size();
    thisRelRef = relRefAdv[relRefId];
  }
  return thisRelRef + getRelRefArgs() + " ";
}

inline std::string QueryGenerator::getRelRefArgs() {
  return "(" + getRandomArg() + ", " + getRandomArg() + ")";
}

/**
returns ‘”’ IDENT ‘”’ | INTEGER | attrRef | synonym | _
*/

inline std::string QueryGenerator::getRandomArg() {
  int random = rand() % 10;
  if (random < 1) {
    // synonym attribute
    return getRandomName() + "." + getAttr();
  }
  else if (random < 2) {
    // _
    return "_";
  }
  else if (random < 3) {
    // INTEGER
    return std::to_string(rand() % HIGHEST_CONSTANT);
  }
  else {
    // synonym
    return getRandomName();
  }
}

/**
returns ‘procName’| ‘varName’ | ‘value’ | ‘stmt#’
*/

inline std::string QueryGenerator::getAttr() {
  return attrNames[rand() % attrNames.size()];
}

inline std::string QueryGenerator::getRandomName() {
  int random = rand() % 26;
  char c;
  if (rand() % 2 == 0) {
    c = random + 'a';
  }
  else {
    c = random + 'A';
  }
  std::string name = c + recursiveGetLetterOrDigit();
  listOfSynonyms.push_back(name);
  return name;
}

/**
returns ( LETTER | DIGIT )*
*/

inline std::string QueryGenerator::recursiveGetLetterOrDigit() {
  if (rand() % 2 == 0) {
    int random = rand() % 62;
    char c;
    if (random < 10) {
      c = '1' + random;
    }
    else if (random < 36) {
      c = 'a' + random - 10;
    }
    else {
      c = 'A' + random - 36;
    }
    return c + recursiveGetLetterOrDigit();
  }
  else {
    return " ";
  }
}

/**
returns ( suchthat-cl | with-cl | pattern-cl | and suchthat-cl | and with-cl | and pattern-cl )*
*/

inline std::string QueryGenerator::RecursiveGetClauses() {
  if (rand() % 5 != 1) {
    if (rand() % 2 == 1) {
      return "and " + GetClause();
    }
    else {
      return GetClause();
    }
  }
  else {
    return "";
  }
}

/**
returns ( suchthat-cl | with-cl | pattern-cl )
*/

inline std::string QueryGenerator::GetClause() {
  int random = rand() % 3;
  switch (random) {
  case 0:
    return "such that " + getRelRef() + " ";
  case 1:
    return "with " + getAttrCond() + " ";
  case 2:
    return "pattern " + getPatternCond() + " ";
  }
  return "";
}

/**
attrCond : attrCompare ( ‘and’ attrCompare )*
*/

inline std::string QueryGenerator::getAttrCond() {
  return getAttrCompare() + getRecursiveAttrCompare();
}

inline std::string QueryGenerator::getRecursiveAttrCompare() {
  return rand() % 2 == 1 ? "and " + getAttrCompare() + getRecursiveAttrCompare() : "";
}

inline std::string QueryGenerator::getAttrCompare() {
  return getRandomArg() + " = " + getRandomArg() + " ";
}

/**
patternCond : pattern
*/

inline std::string QueryGenerator::getPatternCond() {

  switch (iteration) {
  case 1:
    // assign only
    return getRandomArg() + "(" + getRandomArg() + "," + getExpressionSpec() + ")";
    break;
  case 2:
  case 3:
    switch (rand() % 3) {
    case 0: // assign
      return getRandomArg() + "(" + getRandomArg() + "," + getExpressionSpec() + ")";
    case 1: // while
      return getRandomArg() + "(" + getRandomArg() + ",_ )";
    case 2: // if
      return getRandomArg() + "(" + getRandomArg() + ",_,_)";
    }
  }
  return "";
}

inline std::string QueryGenerator::getExpressionSpec() {
  switch (rand() % 6) {
  case 0:
    return "_";
  case 1:
    return "_" + getExpr();
  case 2:
    return getExpr() + "_";
  case 3:
  case 4:
  case 5:
    return "_" + getExpr() + "_";
  }
  return "";
}

inline std::string QueryGenerator::getExpr() {
  switch (iteration) {
  case 1:
    return getRandomSymbol();
  case 2:
  case 3:
    return "\"" + RecursiveGetExpr() + "\"";
  }
  return "";
}

inline std::string QueryGenerator::RecursiveGetExpr() {
  return rand() % 5 == 0 ? "" : getRandomSymbol();
}

inline std::string QueryGenerator::getRandomSymbol() {
  return symbols[rand() % symbols.size()];
}

/**
returns ( ‘and’ pattern )*
*/

inline std::string QueryGenerator::getRecursivePatternCond() {
  return rand() % 2 == 1 ? "and " + getPatternCond() + getRecursivePatternCond() : "";
}

/**
returns design-entity : ‘stmt’ | ‘read’ | ‘print’ | ‘call’ | ‘while’ | ‘if’ | ‘assign’
| ‘variable’ | ‘constant’ | ‘prog_line’ | ‘procedure’
*/

inline std::string QueryGenerator::DeclarationGen() {
  std::string declaration = "";
  for (auto syn : listOfSynonyms) {

    switch (iteration) {
    case 1:
      declaration += designEntitiesBasic[rand() % designEntitiesBasic.size()] + " " + syn + "; ";
      break;
    case 2:
    case 3:
      declaration += designEntitiesAdv[rand() % designEntitiesAdv.size()] + " " + syn + "; ";
    }

  }
  listOfSynonyms.clear();
  return declaration;
}

// TODO: Implemement tuples

inline std::string QueryGenerator::getResultClause() {
  return rand() % 10 == 0 ? "BOOLEAN" : getRandomArg();
}

inline QueryGenerator::QueryGenerator(int iteration) {
  this->iteration = iteration;
}

inline std::string QueryGenerator::GenerateOne() {
  std::string query = "Select " + getResultClause() + " "
    + RecursiveGetClauses();

  std::string declaration = DeclarationGen() + "\n";
  return declaration + query + "\n";
}

inline std::string QueryGenerator::GenerateN(int n) {
  std::string queries = "";
  for (int i = 0; i < n; i++) {
    queries.append(GenerateOne() + "\n");
  }
  return queries;
}
