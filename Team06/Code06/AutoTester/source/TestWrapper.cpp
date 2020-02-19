#include <fstream>
#include <stdexcept>

#include "TestWrapper.h"
#include "LoggingUtils.h"
#include "Parser.h"
#include "PQL.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

/**
 *  Accepts the name of a text file containing the SIMPLE source program and parses
 *  it to populate the PKB.
 *
 *  @param      filename    filename of SIMPLE source program.
 */
// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// Create input file stream and char iterator
    std::ifstream ifs(filename);
    std::string program((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());;
    try {
        parser.parseSimple(program, pkb);
        // Expression e = parser.parseExpression(program);
    }
    catch (std::invalid_argument& e) {
        SPA::LoggingUtils::LogErrorMessage("%s", e.what());
    }
}

/**
 *  Evaluates a PQL query string against a populated PKB and inserts the query result
 *  into the (initially empty) list of query result strings.
 *
 *  @param      query       input PQL query string.
 *  @param      results     reference to list of query result strings.
 */
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
    PQL::PQLManager pql = PQL::PQLManager(pkb);

    // Evaluate query and store result
    results.push_back(pql.evaluateQuery(query));
}
