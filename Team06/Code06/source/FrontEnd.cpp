#include "FrontEnd.h"

namespace FrontEnd {
	FrontEndManager::FrontEndManager()
		: parser(Parser()), de(DesignExtractor()) {
	}

	PKB::PKB FrontEndManager::parseSimple(string source) {
		PKB::PKB pkb = parser.parseSimple(source);
		return de.run(pkb);
	}
	Expression FrontEndManager::parseExpression(string exp) {
		return parser.parseExpression(exp);
	}
}