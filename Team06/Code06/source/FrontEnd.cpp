#include "FrontEnd.h"

namespace FrontEnd {
	FrontEndManager::FrontEndManager()
		: parser(Parser()), de(DesignExtractor()) {
	}

	PKB::PKB FrontEndManager::parseSimple(string source) {
		PKB::PKB pkb = PKB::PKB();
		parser.parseSimple(source, pkb);
		de.run(pkb);
		return pkb;
	}
	Expression FrontEndManager::parseExpression(string exp) {
		return parser.parseExpression(exp);
	}
}