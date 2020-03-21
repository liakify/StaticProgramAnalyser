#include "TestWrapper.h"
#include "LoggingUtils.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
    if (wrapper == 0) wrapper = new TestWrapper;
    return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper()
    : frontEnd(), pkb(), pql(pkb) {
}

/**
 *  Accepts the name of a text file containing the SIMPLE source program and parses
 *  it to populate the PKB.
 *
 *  @param      filename    filename of SIMPLE source program.
 */
void TestWrapper::parse(std::string filename) {
    // Create input file stream and char iterator
    std::ifstream ifs(filename);
    std::string program((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    try {
        this->pkb = frontEnd.parseSimple(program);
        this->pql = PQL::PQLManager(this->pkb);
    }
    catch (std::invalid_argument& e) {
        SPA::LoggingUtils::LogErrorMessage("%s", e.what());
        throw e;
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
    // Evaluate query and store result into provided list
    this->pql.evaluateQuery(query, results);
    this->pkb.clear();
}
