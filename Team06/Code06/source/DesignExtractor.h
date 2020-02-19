#pragma once

#include <unordered_set>

#include "Types.h"
#include "PKB.h"

namespace FrontEnd {
	/**
	* The DesignExtractor class is used for extracting secondary relationships
	* from the the parsed SIMPLE code, and populates the PKB accordingly.
	*/
	class DesignExtractor {
	public:
		/**
		* Design Extractor constructor.
		*
		* @param	pkb		Reference to PKB to be updated.
		*/
		DesignExtractor(PKB::PKB& pkb);

		/**
		* Updates the PKB with secondary relationships.
		* Relationships handled:
		*	Follows*
		*	Parent*
		*/
		void run();

	private:
		PKB::PKB pkb;

		void populateFollowStar();

		void populateParentStar();
	};
}