#pragma once

#include <unordered_set>

#include "Types.h"
#include "PKB.h"
#include "Simple.h"

using namespace SIMPLE;

namespace FrontEnd {
	/**
	* The DesignExtractor class is used for extracting secondary relationships
	* from the the parsed SIMPLE code, and populates the PKB accordingly.
	*/
	class DesignExtractor {
	public:
		/**
		* Updates the PKB with secondary relationships.
		* Relationships handled:
		*	Follows*
		*	Parent*
		*
		* @param	pkb			Reference to PKB to be updated.
		* @return	PKB::PKB	The updated PKB instance.
		*/
		PKB::PKB run(PKB::PKB& pkb);

	private:
		PKB::PKB pkb;

		void populateFollows();
		void populateFollowStar();
		void populateParent();
		void populateParentStar();

		void populateParentKB(StmtId stmtId, StmtListId stmtLstId);
	};
}