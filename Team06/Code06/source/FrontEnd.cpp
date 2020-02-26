#include "FrontEnd.h"

using std::string;

namespace FrontEnd {
	FrontEndManager::FrontEndManager()
		: parser(Parser()), de(DesignExtractor()) {
	}

	PKB::PKB FrontEndManager::parseSimple(string source) {
		PKB::PKB pkb = parser.parseSimple(source);
		return de.run(pkb);
	}
	string FrontEndManager::parseExpression(string exp) {
		return parser.parseExpression(exp);
	}
}