#pragma once

#include "PKB.h"
#include "Parser.h"
#include "DesignExtractor.h"

namespace FrontEnd {
	/**
	 *  The FrontEndManager class is the facade for the entire front end component.
	 *  It relies on its sub-components (Parser, Design Extractor) in order to
	 *  fully parse the SIMPLE source code and populate the PKB accordingly.
	 */
	class FrontEndManager {
	public:
		/**
		* Constructor for FrontEndManager.
		*/
		FrontEndManager();

		/**
		* Parses full SIMPLE source and returns a populated PKB instance.
		*
		* @param	source					SIMPLE program string for parsing.
		* @return	PKB::PKB				The populated PKB instance
		* @throws	std::invalid_argument	if the SIMPLE source has syntax errors.
		*/
		PKB::PKB parseSimple(string source);

		/**
		* Parses standalone SIMPLE expressions.
		*
		* @param	exp						SIMPLE expression to be parsed.
		* @throws	std::invalid_argument	if the SIMPLE source has syntax errors.
		*/
		Expression parseExpression(string exp);
	private:
		Parser parser;
		DesignExtractor de;
	};
}